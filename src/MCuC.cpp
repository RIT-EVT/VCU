#include <MCuC.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>
#include <tx_api.h>

namespace log = core::log;

namespace vcu {

MCuC::MCuC(vcu::MCuC::MCuC_GPIO gpios, io::CAN& can)
    : Initializable((char*) "MCuC"), accessoryCanDataUnsafeBuffer(), mutex((char*) "MCuC Mutex", true),
      powertrainCAN(can), accessoryCanDataSafeBuffer(), gpios(gpios) {
    model.initialize();
}

rtos::TXError MCuC::init(rtos::BytePoolBase& pool) {
    rtos::TXError status = mutex.init(pool);
    if (status != rtos::TXError::TXE_SUCCESS) {
        // we failed the mutex initialization
        return status;
    } else {
        return powertrainCAN.init(pool);
    }
}

CO_OBJ_T* MCuC::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t MCuC::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

uint8_t MCuC::getNodeID() {
    return NODE_ID;
}

void MCuC::handlePowertrainCanMessage(io::CANMessage& message) {
    switch (message.getId()) {
    case dev::PowertrainCAN::MC_INTERNAL_STATES_ID:
        mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
        mcState     = static_cast<MC_VSM_State>(powertrainCAN.parseMCState(message));
        mcDischarge = static_cast<MC_DC_State>(powertrainCAN.parseMCDischarge(message));
        mutex.put();
        break;
    case dev::PowertrainCAN::HIB_MESSAGE_ID:
        mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
        throttle      = powertrainCAN.parseHIBThrottle(message);
        forwardEnable = powertrainCAN.parseHIBForwardEnable(message);
        startPressed  = powertrainCAN.parseHIBStartPressed(message);
        mutex.put();
        break;
    case dev::PowertrainCAN::HARDMON_SELF_TEST_MESSAGE_ID:
        mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
        powertrainCANSelfTestIn = true;
        mutex.put();
        break;
    default:
        // do nothing, we don't care about this message
        break;
    }
}

rtos::TXError MCuC::sendToPowertrainQueue(io::CANMessage* messagePointer, uint32_t waitOption) {
    return powertrainCAN.queue.send(messagePointer, waitOption);
}

rtos::TXError MCuC::recieveFromPowertrainQueue(io::CANMessage* destination, uint32_t waitOption) {
    return powertrainCAN.queue.receive(destination, waitOption);
}

void MCuC::sendOutputDataToUnsafeBuffer() {
    mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    accessoryCanDataUnsafeBuffer.LVSS_out_EnableBoardSignal = accessoryCanDataSafeBuffer.LVSS_out_EnableBoardSignal;
    mutex.put();
}

void MCuC::sendInputDataToSafeBuffer() {
    mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    memcpy(&accessoryCanDataSafeBuffer, &accessoryCanDataUnsafeBuffer, sizeof(AccessoryCanData_t));
    mutex.put();
}

void MCuC::process() {
#ifdef EVT_CORE_LOG_ENABLE
    uint32_t halstart, halstep, halstepEnd, halpowerTrainCAN = 0, halmotorControllerCan, halend;

    halstart = core::time::millis();
#endif

    mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    // update Accessory Can Safe buffer
    sendInputDataToSafeBuffer();

    // brakeOn updated over CAN
    eStop = gpios.eStopGPIO.readPin() == io::GPIO::State::HIGH;
    // forwardEnable, startPressed, mcStateMachine, discharge updated over CAN
    ignitionOn = gpios.ignitionGPIO.readPin() == io::GPIO::State::HIGH;
    // hmFault = gpios.hmFaultGPIO.readPin() == io::GPIO::State::HIGH;
    // throttle updated over CAN
    // lvssOn = gpios.lvssStatusGPIO.readPin() == io::GPIO::State::HIGH;
    mcOn = gpios.mcStatusGPIO.readPin() == io::GPIO::State::HIGH;

    // set the inputs and step the model
    modelInputs.Ignition_LS_A    = ignitionOn;
    modelInputs.ESTOP_LS_A       = eStop;
    modelInputs.HM_Fault         = hmFault;
    modelInputs.MC_ON            = mcOn;
    modelInputs.Start_CAN        = startPressed;
    modelInputs.LVSS_ON_CAN      = lvssOn;
    modelInputs.Brake_CAN        = brakeOn;
    modelInputs.Forward_EN_CAN   = forwardEnable;
    modelInputs.MC_VSM_State_CAN = mcState;
    modelInputs.MC_DC_State_CAN  = mcDischarge;
    modelInputs.Throttle_CAN     = throttle;

#ifdef EVT_CORE_LOG_ENABLE
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "EStop: %d, Ignition %d", eStop, ignitionOn);
#endif

    mutex.put();

#ifdef EVT_CORE_LOG_ENABLE
    halstep = core::time::millis();
#endif

    model.setExternalInputs(&modelInputs);

    model.step();

    // TODO: in the future when the model is reworked so the inputs and outputs are in separate blocks of CAN & GPIO
    // inputs
    //  we can use unions for this and iterate through it.
    // get outputs
    modelOutputs = model.getExternalOutputs();
    // save outputs

#ifdef EVT_CORE_LOG_ENABLE
    halstepEnd = core::time::millis();
#endif

    mutex.get(rtos::TXW_WAIT_FOREVER);
    lvssEnable        = modelOutputs.LVSS_EN_uC;
    inverterEnable    = modelOutputs.Inverter_EN_uC_CAN;
    ucFault           = modelOutputs.Fault;
    watchdog          = modelOutputs.Watchdog;
    ucState.stateEnum = modelOutputs.uC_State;
    inverterDischarge = modelOutputs.Inverter_DC_uC_CAN;
    mcEnableUC        = modelOutputs.MC_EN_uC;
    torqueRequest     = modelOutputs.Torque_Request_CAN;
    mcSelfTestOut     = modelOutputs.Self_Test;

#ifdef EVT_CORE_LOG_ENABLE
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "MC State Machine State: %d", ucState.stateEnum);
#endif

    // use outputs
    gpios.lvssEnableGPIO.writePin(lvssEnable ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    // set inverterEnable before we send the message
    // gpios.ucFaultGPIO.writePin(ucFault ? io::GPIO::State::HIGH : io::GPIO::State::LOW); (gone)
    gpios.watchdogGPIO.writePin(watchdog ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateZeroGPIO.writePin(ucState.stateBit0 ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateOneGPIO.writePin(ucState.stateBit1 ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateTwoGPIO.writePin(ucState.stateBit2 ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateThreeGPIO.writePin(ucState.stateBit3 ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    // set inverterDisable before we send the message
    //  set Motor Controller via the two gpios
    //  If mcEnableUC is true, Positive should be High and Negative should be Low, otherwise they should be the
    //  opposite.
    gpios.mcTogglePositiveGPIO.writePin(mcEnableUC ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.mcToggleNegativeGPIO.writePin(mcEnableUC ? io::GPIO::State::LOW : io::GPIO::State::HIGH);
    // set torqueRequest before we send the message
    gpios.mcSelfTestGPIO.writePin(mcSelfTestOut ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    // Setting one of these might have fried the board...
    // gpios.estopSelfTestGPIO.writePin(estopSelfTestOut ? io::GPIO::State::HIGH : io::GPIO::State::LOW); (gone)
    // gpios.ignitionSelfTestGPIO.writePin(ignitionSelfTestOut ? io::GPIO::State::HIGH : io::GPIO::State::LOW); (gone)
    // We will send accessory CAN SelfTest message over CANopen
    // Send the powertrainCanSelfTest message

    if (powertrainCanSelfTestOut) {
#ifdef EVT_CORE_LOG_ENABLE
        halpowerTrainCAN = core::time::millis();
#endif

        powertrainCAN.sendUCSelfTestMessage();
    }

    // setting the CAN self test: only true when we are in ucState 10 (self test state)
    // TODO: unknown if this self test is true or not yet
    bool canSelfTest = ucState.stateEnum == UC_State::LVSS_MC_Shutdown;
    gpios.canSelfTestGPIO.writePin(canSelfTest ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

    // Send the Motor Controller CAN message (set values first)
    powertrainCAN.setMCInverterEnable(inverterEnable);
    powertrainCAN.setMCInverterDischarge(inverterDischarge);
    powertrainCAN.setMCTorque(torqueRequest);

#ifdef EVT_CORE_LOG_ENABLE
    halmotorControllerCan = core::time::millis();
#endif

    sendOutputDataToUnsafeBuffer();
    powertrainCAN.sendMCMessage();
    mutex.put();

#ifdef EVT_CORE_LOG_ENABLE
    halend = core::time::millis();

    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "MS Timing:");
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG,
                    "Starting: %d\n\r"
                    "Stepping: %d\n\r"
                    "Step Done: %d\n\r"
                    "Sending PT Can: %d\n\r",
                    halstart,
                    halstep,
                    halstepEnd,
                    halpowerTrainCAN);
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG,
                    "Sending Motor Can: %d\n\r"
                    "Ended: %d\n\r",
                    halend,
                    halmotorControllerCan);
#endif
}

void MCuC::imagineNeuteredProcess() {
    mutex.get(core::rtos::TXW_WAIT_FOREVER);
    // both estop and ignition are active low
    eStop      = gpios.eStopGPIO.readPin() == io::GPIO::State::LOW;
    ignitionOn = gpios.ignitionGPIO.readPin() == io::GPIO::State::LOW;
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Estop: %d, ignition: %d", eStop, ignitionOn);

    sendInputDataToSafeBuffer();
    if (!eStop && ignitionOn) {
        accessoryCanDataSafeBuffer.LVSS_out_EnableBoardSignal = 63;
        log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Telling LVSS to turn ON boards");
    } else {
        accessoryCanDataSafeBuffer.LVSS_out_EnableBoardSignal = 0;
        log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Telling LVSS to turn OFF boards");
    }
    sendOutputDataToUnsafeBuffer();
    mutex.put();
}

} // namespace vcu
