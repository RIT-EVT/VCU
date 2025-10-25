#include <MCuC.hpp>
#include <core/rtos/Threadx.hpp>
#include <core/utils/log.hpp>
#include <core/utils/time.hpp>

namespace log = core::log;

namespace vcu {

MCuC::MCuC(vcu::MCuC::MCuC_GPIO gpios, io::CAN& can)
    : Initializable((char*) "MCuC"), accessoryCanDataUnsafeBuffer(), bufferMutex((char*) "Data Buffer Mutex", true),
      hbMutex((char*) "Heartbeat Mutex", true), powertrainCAN(can), accessoryCanDataSafeBuffer(), gpios(gpios) {
    model.initialize();
}

rtos::TXError MCuC::init(rtos::BytePoolBase& pool) {
    Initializable* initializables[3] = {&bufferMutex, &hbMutex, &powertrainCAN};
    return core::rtos::bulkInitialize(initializables, 3, pool);
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
    bufferMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    switch (message.getId()) {
    case dev::PowertrainCAN::MC_INTERNAL_STATES_ID:
        mcState     = static_cast<MC_VSM_State>(powertrainCAN.parseMCState(message));
        mcDischarge = static_cast<MC_DC_State>(powertrainCAN.parseMCDischarge(message));
        break;
    case dev::PowertrainCAN::HIB_MESSAGE_ID:
        throttle      = powertrainCAN.parseHIBThrottle(message);
        forwardEnable = powertrainCAN.parseHIBForwardEnable(message);
        startPressed  = powertrainCAN.parseHIBStartPressed(message);
        break;
    case dev::PowertrainCAN::HARDMON_SELF_TEST_MESSAGE_ID:  // todo: fill these out
        break;
    case dev::PowertrainCAN::BMS_MESSAGE_ID:
        break;
    case dev::PowertrainCAN::GFDB_MESSAGE_ID:
        break;
    default:
        // do nothing, we don't care about this message
        break;
    }
    bufferMutex.put();
}

rtos::TXError MCuC::sendToPowertrainQueue(io::CANMessage* messagePointer, uint32_t waitOption) {
    return powertrainCAN.queue.send(messagePointer, waitOption);
}

rtos::TXError MCuC::receiveFromPowertrainQueue(io::CANMessage* destination, uint32_t waitOption) {
    return powertrainCAN.queue.receive(destination, waitOption);
}

void MCuC::sendOutputDataToUnsafeBuffer() {
    bufferMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    memcpy(&accessoryCanDataUnsafeBuffer.outputs,
           &accessoryCanDataSafeBuffer.outputs,
           sizeof(AccessoryCanData_t::outputs));
    bufferMutex.put();
}

void MCuC::sendInputDataToSafeBuffer() {
    bufferMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    memcpy(
        &accessoryCanDataSafeBuffer.inputs, &accessoryCanDataUnsafeBuffer.inputs, sizeof(AccessoryCanData_t::inputs));
    bufferMutex.put();
}

void MCuC::updateNodeHeartbeat(uint32_t nodeId) {
    int slot;

    // Convert board ID to array index
    switch (nodeId) {
        case LVSS_NODE_ID:
            slot = 0;
            break;
        case TMS_NODE_ID:
            slot = 1;
            break;
        case dev::PowertrainCAN::BMS_MESSAGE_ID:
            slot = 2;
            break;
        case dev::PowertrainCAN::GFDB_MESSAGE_ID:
            slot = 3;
            break;
        case dev::PowertrainCAN::HIB_MESSAGE_ID:
            slot = 4;
            break;
        default:
            return;  // Should never get here; means we received a message from an unknown board
    }

    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "HEARTBEAT: hb being increased");   // todo: remove when tested

    hbMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    heartbeatMessages[slot]++;
    hbMutex.put();
}

// todo: for testing purposes; remove when done
inline const char* stateToString(UC_State state) {
    switch (state) {
    case UC_State::Preset:             return "Preset";
    case UC_State::Key_Cycle:          return "Key_Cycle";
    case UC_State::MC_Off:             return "MC_Off";
    case UC_State::LVSS_MC_Startup:    return "LVSS_MC_Startup";
    case UC_State::MC_Init:            return "MC_Init";
    case UC_State::Contactor_Closed:   return "Contactor_Closed";
    case UC_State::MC_Ready:           return "MC_Ready";
    case UC_State::Contactor_Open:     return "Contactor_Open";
    case UC_State::MC_Discharging:     return "MC_Discharging";
    case UC_State::LVSS_MC_Shutdown:   return "LVSS_MC_Shutdown";
    case UC_State::MC_Active:          return "MC_Active";
    case UC_State::Estop:              return "Estop";
    case UC_State::Fault:              return "Fault";
    case UC_State::Super_Fault:        return "Super_Fault";
    default:                           return "Unknown_State";
    }
}

void MCuC::process() {
#ifdef EVT_CORE_LOG_ENABLE
    uint32_t halstart, halstep, halstepEnd, halpowerTrainCAN = 0, halmotorControllerCan, halend;

    halstart = core::time::millis();
#endif

    bufferMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Start of process");    // todo: remove when done
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "STATE: %s", stateToString(modelOutputs.uC_State));
    // update Accessory Can Safe buffer
    sendInputDataToSafeBuffer();

    // brakeOn updated over CAN
    eStop = gpios.eStopAGPIO.readPin() == io::GPIO::State::LOW; // active low
    // forwardEnable, startPressed, mcStateMachine, discharge updated over CAN
    ignitionOn = gpios.ignitionAGPIO.readPin() == io::GPIO::State::LOW; // active low
    // hmFault = gpios.hmFaultGPIO.readPin() == io::GPIO::State::HIGH;  // todo: why are these commented out
    // throttle updated over CAN
    // lvssOn = gpios.lvssStatusGPIO.readPin() == io::GPIO::State::HIGH;
    mcOn = gpios.mcStatusGPIO.readPin() == io::GPIO::State::HIGH;
    interlock = gpios.interlockGPIO.readPin() == io::GPIO::State::HIGH;


    // set the inputs and step the model
    modelInputs.Ignition_LS_A    = ignitionOn;
    modelInputs.ESTOP_LS_A       = eStop;
//    modelInputs.HM_Fault         = hmFault;
    modelInputs.MC_ON            = mcOn;
    modelInputs.Start_CAN        = startPressed;
    modelInputs.LVSS_ON_CAN      = lvssOn;
    modelInputs.Brake_CAN        = brakeOn;
    modelInputs.Forward_EN_CAN   = forwardEnable;
    modelInputs.MC_VSM_State_CAN = mcState;
    modelInputs.MC_DC_State_CAN  = mcDischarge;
    modelInputs.Throttle_CAN     = throttle;

#ifdef EVT_CORE_LOG_ENABLE
//    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "EStop: %d, Ignition %d", eStop, ignitionOn);
#endif

    bufferMutex.put();

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

    bufferMutex.get(rtos::TXW_WAIT_FOREVER);

    lvssEnable        = modelOutputs.LVSS_EN_uC;
    inverterEnable    = modelOutputs.Inverter_EN_uC_CAN;
    ucFault           = modelOutputs.Fault;
    watchdog          = modelOutputs.Watchdog;
    ucState.stateEnum = modelOutputs.uC_State;
    inverterDischarge = modelOutputs.Inverter_DC_uC_CAN;
    mcEnableUC        = modelOutputs.MC_EN_uC;
    torqueRequest     = modelOutputs.Torque_Request_CAN;
    mcSelfTestOut     = modelOutputs.MC_Self_Test;


#ifdef EVT_CORE_LOG_ENABLE
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "STATE AFTER STEP: %s", stateToString(ucState.stateEnum));
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Model Step Length: %lu ms", (halstep - halstepEnd));
#endif

    // use outputs
    gpios.lvssEnableGPIO.writePin(lvssEnable ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    // set inverterEnable before we send the message
    // gpios.ucFaultGPIO.writePin(ucFault ? io::GPIO::State::HIGH : io::GPIO::State::LOW); (gone) // todo: check
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

    powertrainCAN.sendMCMessage();
    sendOutputDataToUnsafeBuffer();
    bufferMutex.put();

#ifdef EVT_CORE_LOG_ENABLE
    halend = core::time::millis();
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Full Process length: %lu ms", (halend - halstart));

//    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "MS Timing:");
//    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG,
//                    "Starting: %d\n\r"
//                    "Stepping: %d\n\r"
//                    "Step Done: %d\n\r"
//                    "Sending PT Can: %d\n\r",
//                    halstart,
//                    halstep,
//                    halstepEnd,
//                    halpowerTrainCAN);
//    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG,
//                    "Sending Motor Can: %d\n\r"
//                    "Ended: %d\n\r",
//                    halend,
//                    halmotorControllerCan);
#endif
}

} // namespace vcu
