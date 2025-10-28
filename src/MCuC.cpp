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
    static bool mcEnableLast = false;   // The mcEnable needs to do a pulse every time it switches, so need to remember last state
    // todo: debugging static vars for manually tricking simulink model into going through full state machine
    static bool firstStep = true;
    static bool seenMCInit = false;
    static bool forwardStatic = false;
    static int16_t throttleStatic = 0;

#ifdef EVT_CORE_LOG_ENABLE
    uint32_t halstart, halstep, halstepEnd, halpowerTrainCAN = 0, halmotorControllerCan, halend;

    halstart = core::time::millis();
#endif

    bufferMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);

    // update Accessory Can Safe buffer
    sendInputDataToSafeBuffer();

    // brakeOn updated over CAN
    eStop = gpios.eStopAGPIO.readPin() == io::GPIO::State::LOW; // active low
    // forwardEnable, startPressed, mcStateMachine, discharge updated over CAN
    ignitionOn = gpios.ignitionAGPIO.readPin() == io::GPIO::State::LOW; // active low
    // throttle updated over CAN
    // lvssOn = gpios.lvssStatusGPIO.readPin() == io::GPIO::State::HIGH;
    mcOn = gpios.mcStatusGPIO.readPin() == io::GPIO::State::HIGH;
    interlock = gpios.interlockGPIO.readPin() == io::GPIO::State::HIGH;

    // set the inputs and step the model
    modelInputs.Ignition_LS_A    = ignitionOn;
    modelInputs.ESTOP_LS_A       = eStop;
    modelInputs.MC_ON            = mcOn;
    modelInputs.Start_CAN        = startPressed;
    modelInputs.LVSS_ON_CAN      = lvssOn;
    modelInputs.Brake_CAN        = brakeOn;
    modelInputs.Forward_EN_CAN   = forwardEnable;
    modelInputs.MC_VSM_State_CAN = mcState;
    modelInputs.MC_DC_State_CAN  = mcDischarge;
    modelInputs.Throttle_CAN     = throttle;
    modelInputs.Interlock        = interlock;

    // todo: test hardcoding
    modelInputs.Interlock = true;
    modelInputs.LVSS_ON_CAN = false;
    modelInputs.MC_ON = false;
    modelInputs.BMS_Contactor_Closed_CAN = false;
    modelInputs.GFDB_Isolation_State_CAN = 0;
    modelInputs.MC_VSM_State_CAN = MC_VSM_State::Start;
    modelInputs.HIB_Comparison_Fault_CAN = false;

    if (firstStep) {
        firstStep = false;
        for (int i = 0; i < HB_SIZE; i++) {
            modelInputs.Heartbeats_CAN[i] = 0;
        }
    } else {
        modelInputs.BMS_Contactor_Closed_CAN = static_cast<int>(modelOutputs.BMS_Contactor_Command_uC_CAN) != 0;
        modelInputs.LVSS_ON_CAN = modelOutputs.LVSS_EN_uC;
        modelInputs.MC_ON = modelOutputs.MC_EN_uC;
    }

    // Big ass code block to fake inputs to test simulink model
    if (!firstStep) {
        for (int i = 0; i < HB_SIZE; i++) {
            modelInputs.Heartbeats_CAN[i]++;
        }

        if (modelOutputs.uC_State == UC_State::MC_Init || seenMCInit) {
            modelInputs.MC_VSM_State_CAN = MC_VSM_State::Ready;
            seenMCInit = true;
        }

        if (modelOutputs.uC_State == UC_State::Contactor_Closed) {
            modelInputs.Start_CAN = true;
        }

        if (modelOutputs.uC_State == UC_State::MC_Ready) {
            modelInputs.Brake_CAN = true;
            modelInputs.Throttle_CAN = 0;
            forwardStatic = true;
        }

        modelInputs.Forward_EN_CAN = forwardStatic;

        if (modelOutputs.uC_State == UC_State::MC_Active) {
            modelInputs.Throttle_CAN = throttleStatic++;
        }

        if (modelOutputs.uC_State == UC_State::MC_Discharging) {
            modelInputs.MC_DC_State_CAN = MC_DC_State::Complete;
        }

        if (modelOutputs.uC_State == UC_State::Contactor_Open) {
            forwardStatic = false;
            modelInputs.Forward_EN_CAN = false;
            modelInputs.MC_VSM_State_CAN = MC_VSM_State::Start;
            seenMCInit = false;
        }
    }

    hbMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    for (int i = 0; i < HB_SIZE; i++) {
//      modelInputs.Heartbeats_CAN[i] = heartbeatMessages[i]; // todo: untested, but should work when we actually connect other boards
    }

#ifdef EVT_CORE_LOG_ENABLE
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "heartbeats: %lu, %lu, %lu, %lu, %lu", modelInputs.Heartbeats_CAN[0], modelInputs.Heartbeats_CAN[1], modelInputs.Heartbeats_CAN[2], modelInputs.Heartbeats_CAN[3], modelInputs.Heartbeats_CAN[4]);
//    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "EStop: %d, Ignition %d", eStop, ignitionOn);
#endif
    hbMutex.put();

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
//    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Model Step Length: %lu ms", (halstepEnd - halstep));
    uint32_t halSection = core::time::millis(); // todo: remove when done debugging
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
    // set Motor Controller via the two gpios

    // If mcEnableUC switches to true mcTogglePositive should pulse high, and
    // if mcEnableUC switches to false mcToggleNegative should pulse high
    if (mcEnableLast != mcEnableUC) {
        if (mcEnableUC) {
            gpios.mcTogglePositiveGPIO.writePin(io::GPIO::State::HIGH);
            rtos::sleep(MS_TO_TICKS(10));
            gpios.mcTogglePositiveGPIO.writePin(io::GPIO::State::LOW);
        } else {
            gpios.mcToggleNegativeGPIO.writePin(io::GPIO::State::HIGH);
            rtos::sleep(MS_TO_TICKS(10));
            gpios.mcToggleNegativeGPIO.writePin(io::GPIO::State::LOW);
        }
        mcEnableLast = mcEnableUC;
    }

    // set torqueRequest before we send the message
    gpios.mcSelfTestGPIO.writePin(mcSelfTestOut ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    // We will send accessory CAN SelfTest message over CANopen
    // Send the powertrainCanSelfTest message

#ifdef EVT_CORE_LOG_ENABLE
    uint32_t halSectionEnd = core::time::millis();  // todo: remove when done debugging
#endif

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

    io::CAN::CANStatus mcMessageStatus = powertrainCAN.sendMCMessage();

#ifdef EVT_CORE_LOG_ENABLE
    if (mcMessageStatus != io::CAN::CANStatus::OK) {
        log::LOGGER.log(core::log::Logger::LogLevel::WARNING, "Motor Controller Message Failed with error %d", mcMessageStatus);
    }
#endif

    sendOutputDataToUnsafeBuffer();
    bufferMutex.put();

#ifdef EVT_CORE_LOG_ENABLE
    halend = core::time::millis();
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Process length: %lu ms", (halend-halstart));

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
