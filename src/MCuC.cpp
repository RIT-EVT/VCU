#include <MCuC.hpp>
#include <BoardMessageParsers.hpp>
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
        case dev::PowertrainCAN::MC_INTERNAL_STATES_ID: {
            boards::MCInternalParsed ps = boards::parseMCInternalMessage(message);
            mcState = static_cast<MC_VSM_State>(ps.mcState);
            mcDischarge = static_cast<MC_DC_State>(ps.mcDischarge);
            break;
        }
        case dev::PowertrainCAN::HIB_MESSAGE_ID: {
            boards::HIBParsed ps = boards::parseHIBMessage(message);
            throttle = ps.throttle;
            forwardEnable = ps.forwardEn;
            startPressed = ps.startPressed;
            brakeOn = ps.brakeOn;
            hibComparisonFault = ps.comparisonFault;
            break;
        }
        case dev::PowertrainCAN::HARDMON_SELF_TEST_MESSAGE_ID: {
            boards::HardmonParsed ps = boards::parseHardmonMessage(message);
            powertrainCANSelfTestIn = ps.powertrainCANSelfTest;
            break;
        }
        case dev::PowertrainCAN::BMS_MESSAGE_ID: {
            boards::BMSParsed ps = boards::parseBMSMessage(message);
            bmsContactorClosed = ps.contactorClosed;
            // todo: V these will most def be sent in across multiple messages, figure out how to receive them
            memcpy(bmsCellTemps, ps.cellTemps, sizeof(bmsCellTemps));
            memcpy(bmsCellVoltages, ps.cellVolts, sizeof(bmsCellVoltages));
            break;
        }
        case dev::PowertrainCAN::GFDB_INCOMING_ID: {
            gfdbIsolationState = boards::parseGFDBMessage(message).isolationState;
            break;
        }
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

void MCuC::updateCanOpenNodeHeartbeat(uint32_t nodeId) {
    int slot;
    switch (nodeId) {
    case LVSS_NODE_ID:
        slot = 0;
        break;
    case TMS_NODE_ID:
        slot = 1;
        break;
    default:
        return;
    }

    heartbeatMessages[slot]++;
}

void MCuC::updateCanNodeHeartbeat(uint32_t nodeId) {
    int slot;

    // Convert board ID to array index
    switch (nodeId) {
    case dev::PowertrainCAN::BMS_MESSAGE_ID:
        slot = 2;
        break;
    case dev::PowertrainCAN::GFDB_INCOMING_ID:
        slot = 3;
        break;
    case dev::PowertrainCAN::HIB_MESSAGE_ID:
        slot = 4;
        break;
    default:
        return;
    }

    hbMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);    // todo: idk if a mutex is even needed here
    heartbeatMessages[slot]++;
    hbMutex.put();
}

void MCuC::setGroundFaultFlag() {
    groundFaultRequestFlag = true;
}

void MCuC::setHealthFlags(bool modelSpeedErr, bool modelRanErr, bool ptcanRanErr, bool ptcanISRErr, bool canopenNotRun, bool gfdbReqNotRun) {
    // set the variable that canOpen sends for the health flags
    healthFlags.modelSpeedErr = modelSpeedErr;
    healthFlags.modelRanErr = modelRanErr;
    healthFlags.ptcanRanErr = ptcanRanErr;
    healthFlags.ptcanISRErr = ptcanISRErr;
    healthFlags.canopenNotRun = canopenNotRun;
    healthFlags.gfdbReqNotRun = gfdbReqNotRun;
}

// todo: for testing purposes; remove when done
inline const char* stateToString(UC_State state) {
    switch (state) {
    case UC_State::Preset:
        return "Preset";
    case UC_State::Key_Cycle:
        return "Key_Cycle";
    case UC_State::MC_Off:
        return "MC_Off";
    case UC_State::LVSS_MC_Startup:
        return "LVSS_MC_Startup";
    case UC_State::MC_Init:
        return "MC_Init";
    case UC_State::Contactor_Closed:
        return "Contactor_Closed";
    case UC_State::MC_Ready:
        return "MC_Ready";
    case UC_State::Contactor_Open:
        return "Contactor_Open";
    case UC_State::MC_Discharging:
        return "MC_Discharging";
    case UC_State::LVSS_MC_Shutdown:
        return "LVSS_MC_Shutdown";
    case UC_State::MC_Active:
        return "MC_Active";
    case UC_State::Estop:
        return "Estop";
    case UC_State::Fault:
        return "Fault";
    case UC_State::Super_Fault:
        return "Super_Fault";
    default:
        return "Unknown_State";
    }
}

bool MCuC::process() {
    // The mcEnable needs to do a pulse every time it switches, so keep track of last mcEnable value
    static bool mcEnableLast = false;

    // Need to send canOpen message on state change, so keep track of lastState
    static UC_State lastState = UC_State::Preset;

    // todo: debugging static vars for manually tricking simulink model into going through full state machine
    static bool firstStep         = true;
    static bool seenMCInit        = false;
    static bool forwardStatic     = false;
    static bool interlock         = true;
    static int16_t throttleStatic = 0;

    // todo: These 3 below are only for checking process trigger timing using saleae
//    static io::GPIO::State state = io::GPIO::State::LOW;
//    gpios.ledTwoGPIO.writePin(state);
//    state = (state == io::GPIO::State::LOW ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

#ifdef EVT_CORE_LOG_ENABLE
    uint32_t halstart, halstep, halstepEnd, halpowerTrainCAN = 0, halmotorControllerCan, halend;

    halstart = core::time::millis();
#endif

    bufferMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);

    // update Accessory Can Safe buffer
    sendInputDataToSafeBuffer();
    bufferMutex.put();

    // Set pin inputs
    modelInputs.LS_Self_Test_In_A = gpios.lsSelfTestInAGPIO.readPin() == io::GPIO::State::HIGH;
    modelInputs.LS_Self_Test_In_B = gpios.lsSelfTestInBGPIO.readPin() == io::GPIO::State::HIGH;
    modelInputs.MC_ON             = gpios.mcStatusGPIO.readPin() == io::GPIO::State::HIGH;
    modelInputs.Interlock         = gpios.interlockGPIO.readPin() == io::GPIO::State::HIGH;
    modelInputs.Ignition_LS_A     = gpios.ignitionAGPIO.readPin() == io::GPIO::State::LOW; // active low
    modelInputs.Ignition_LS_B     = gpios.ignitionBGPIO.readPin() == io::GPIO::State::LOW; // active low
    modelInputs.ESTOP_LS_A        = gpios.eStopAGPIO.readPin() == io::GPIO::State::LOW;    // active low
    modelInputs.ESTOP_LS_B        = gpios.eStopBGPIO.readPin() == io::GPIO::State::LOW;    // active low

    // Set CAN inputs (values updated over CAN)

        //unused inputs
    modelInputs.Torque_Limit_Command1;
    modelInputs.Speed_Mode_Enable1;
    modelInputs.Speed_Command1;
    modelInputs.Direction_Command1;
    modelInputs.Rolling_Counter1;

        // From Motor Controller
    modelInputs.MC_DC_State_CAN   = mcDischarge;
    modelInputs.MC_VSM_State_CAN  = mcState;

        // From HIB over raw CAN
    modelInputs.Forward_EN_CAN    = forwardEnable;
    modelInputs.Start_CAN         = startPressed;
    modelInputs.Brake_CAN         = brakeOn;
    modelInputs.HIB_Comparison_Fault_CAN = hibComparisonFault;
    modelInputs.Throttle_CAN      = throttle;
        // From BMS over raw CAN
    memcpy(modelInputs.BMS_Cell_Temps_CAN, bmsCellTemps, sizeof(bmsCellTemps));
    memcpy(modelInputs.BMS_Cell_Voltages_CAN, bmsCellVoltages, sizeof(bmsCellVoltages));
    modelInputs.BMS_Contactor_Closed_CAN = bmsContactorClosed;
        // From GFDB over raw CAN
    modelInputs.GFDB_Isolation_State_CAN = gfdbIsolationState;
        // From TMS
    modelInputs.Batt_PS_Present_CAN      = battPSPresent;
    memcpy(modelInputs.Cooling_Loop_Temps_CAN, coolingLoopTemps, sizeof(coolingLoopTemps));
    modelInputs.MC_Cooling_FR_CAN        = mcCoolingFR;
    modelInputs.Batt_Cooling_FR_CAN      = battCoolingFR;
    modelInputs.MC_PS_Present_CAN        = mcPSPresent;
        // From LVSS
    modelInputs.LVSS_ON_CAN              = lvssOn;
    modelInputs.Acc_ON_CAN               = accessoryCanDataSafeBuffer.LVSS_in_EnableBoardSignal.acc;
    modelInputs.HIB_ON_CAN               = accessoryCanDataSafeBuffer.LVSS_in_EnableBoardSignal.hib;
    modelInputs.HUDL_ON_CAN              = accessoryCanDataSafeBuffer.LVSS_in_EnableBoardSignal.hudl;
    modelInputs.TMS_ON_CAN               = accessoryCanDataSafeBuffer.LVSS_in_EnableBoardSignal.tms;
    modelInputs.GUB_ON_CAN               = accessoryCanDataSafeBuffer.LVSS_in_EnableBoardSignal.gub;
    modelInputs.Batt_12V_ON_CAN          = accessoryCanDataSafeBuffer.LVSS_in_EnableBoardSignal.batt;
    modelInputs.Vicor_Input_Current_CAN  = accessoryCanDataSafeBuffer.LVSS_in_VicorCurrent;

    memcpy(modelInputs.LVSS_Temps_CAN, accessoryCanDataSafeBuffer.LVSS_in_PowerSwitchTemperatures, sizeof(accessoryCanDataSafeBuffer.LVSS_in_PowerSwitchTemperatures));
    memcpy(modelInputs.LVSS_Currents_CAN, accessoryCanDataSafeBuffer.LVSS_in_PowerSwitchCurrents, sizeof(accessoryCanDataSafeBuffer.LVSS_in_PowerSwitchCurrents));


    // todo: test hardcoding
    modelInputs.Interlock                = interlock;
    modelInputs.LVSS_ON_CAN              = false;
    modelInputs.LVSS_ON_CAN              = false;
    modelInputs.MC_ON                    = false;
    modelInputs.BMS_Contactor_Closed_CAN = false;
    modelInputs.GFDB_Isolation_State_CAN = 0;
    modelInputs.MC_VSM_State_CAN         = MC_VSM_State::Start;
    modelInputs.HIB_Comparison_Fault_CAN = false;

    if (firstStep) {
        firstStep = false;
    } else {
        modelInputs.BMS_Contactor_Closed_CAN = static_cast<int>(modelOutputs.BMS_Contactor_Command_uC_CAN) != 0;
        modelInputs.LVSS_ON_CAN              = modelOutputs.LVSS_EN_uC;
        modelInputs.MC_ON                    = modelOutputs.MC_EN_uC;
    }

    // Big ass code block to fake inputs to test simulink model
    if (!firstStep) {
        if (modelOutputs.uC_State == UC_State::MC_Init || seenMCInit) {
            modelInputs.MC_VSM_State_CAN = MC_VSM_State::Ready;
            seenMCInit                   = true;
        }

        if (modelOutputs.uC_State == UC_State::Contactor_Closed) {
            modelInputs.Start_CAN = true;
        }

        if (modelOutputs.uC_State == UC_State::MC_Ready) {
            modelInputs.Brake_CAN    = true;
            modelInputs.Throttle_CAN = 0;
            forwardStatic            = true;
        }

        modelInputs.Forward_EN_CAN = forwardStatic;

        if (modelOutputs.uC_State == UC_State::MC_Active) {
            modelInputs.Throttle_CAN = throttleStatic++;
        }

        if (modelOutputs.uC_State == UC_State::MC_Discharging) {
            modelInputs.MC_DC_State_CAN = MC_DC_State::Complete;
        }

        if (modelOutputs.uC_State == UC_State::Contactor_Open) {
            forwardStatic                = false;
            modelInputs.Forward_EN_CAN   = false;
            modelInputs.MC_VSM_State_CAN = MC_VSM_State::Start;
            seenMCInit                   = false;
        }
    }

    hbMutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    for (int i = 0; i < HB_SIZE; i++) {
        modelInputs.Heartbeats_CAN[i] = heartbeatMessages[i];
    }
    hbMutex.put();

#ifdef EVT_CORE_LOG_ENABLE
//    halstep = core::time::millis();
#endif

    // Set inputs, run model, and receive outputs
    model.setExternalInputs(&modelInputs);
    model.step();
    modelOutputs = model.getExternalOutputs();

#ifdef EVT_CORE_LOG_ENABLE
//    halstepEnd = core::time::millis();
#endif

    bufferMutex.get(rtos::TXW_WAIT_FOREVER);

    ucState.stateEnum = modelOutputs.uC_State; // keep for the union
    mcEnableUC        = modelOutputs.MC_EN_uC;

    // use outputs

    // Unused model signals so just ignore them
    modelOutputs.Batt_PS_Request_uC_CAN;
    modelOutputs.MC_PS_Request_uC_CAN;
    modelOutputs.Fault_to_MC_CAN;


    gpios.ucStateZeroGPIO.writePin(ucState.stateBit0 ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateOneGPIO.writePin(ucState.stateBit1 ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateTwoGPIO.writePin(ucState.stateBit2 ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateThreeGPIO.writePin(ucState.stateBit3 ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

    gpios.lvssEnableGPIO.writePin(modelOutputs.LVSS_EN_uC ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.watchdogGPIO.writePin(modelOutputs.Watchdog ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

    gpios.canSelfTestGPIO.writePin(modelOutputs.CAN_Self_Test ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.lsSelfTestOutGPIO.writePin(modelOutputs.LS_Self_Test_Out ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.mcSelfTestGPIO.writePin(modelOutputs.MC_Self_Test ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

    // Fault LEDs
    gpios.faultLEDGPIO.writePin(modelOutputs.Fault ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.superFaultLEDGPIO.writePin(modelOutputs.Super_Fault ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

    // HUDL LEDs
    gpios.ledOneGPIO.writePin(modelOutputs.LED[0] ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
//    gpios.ledTwoGPIO.writePin(modelOutputs.LED[1] ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ledThreeGPIO.writePin(modelOutputs.LED[2] ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

    // set Motor Controller via the two gpios
    if (mcEnableLast != mcEnableUC) {
        // If mcEnableUC switches to true mcTogglePositive should pulse high
        if (mcEnableUC) {
            gpios.mcTogglePositiveGPIO.writePin(io::GPIO::State::HIGH);
            rtos::sleep(MS_TO_TICKS(10));
            gpios.mcTogglePositiveGPIO.writePin(io::GPIO::State::LOW);
        }
        // if mcEnableUC switches to false mcToggleNegative should pulse high
        else {
            gpios.mcToggleNegativeGPIO.writePin(io::GPIO::State::HIGH);
            rtos::sleep(MS_TO_TICKS(10));
            gpios.mcToggleNegativeGPIO.writePin(io::GPIO::State::LOW);
        }
        mcEnableLast = mcEnableUC;
    }


    // Power is going be gone soon
    if (modelOutputs.Shutdown) {
        powertrainCAN.sendShutdownWarningMessage();
    }

    // Set CanOpen output data
    accessoryCanDataSafeBuffer.LVSS_out_EnableBoardSignal.acc = modelOutputs.Acc_EN_uC_CAN;
    accessoryCanDataSafeBuffer.LVSS_out_EnableBoardSignal.batt = modelOutputs.Batt_12V_EN_uC_CAN;
    accessoryCanDataSafeBuffer.LVSS_out_EnableBoardSignal.gub = modelOutputs.GUB_EN_uC_CAN;
    accessoryCanDataSafeBuffer.LVSS_out_EnableBoardSignal.hib = modelOutputs.HIB_EN_uC_CAN;
    accessoryCanDataSafeBuffer.LVSS_out_EnableBoardSignal.tms = modelOutputs.TMS_EN_uC_CAN;
    accessoryCanDataSafeBuffer.LVSS_out_EnableBoardSignal.hudl = modelOutputs.HUDL_EN_uC_CAN;

    // todo: If needed, look into speeding up cycle time by only sending powertrain CAN messages if something changes
    // Send the Motor Controller CAN message (set values first)
    powertrainCAN.setMCAll(modelOutputs.Torque_Request_CAN,
                           modelOutputs.Speed_Command_uC_CAN,
                           modelOutputs.Direction_Command_uC_CAN,
                           modelOutputs.Inverter_EN_uC_CAN,
                           modelOutputs.Inverter_DC_uC_CAN,
                           modelOutputs.Speed_Mode_Enable_uC_CAN,
                           modelOutputs.Rolling_Counter_uC_CAN,
                           modelOutputs.Torque_Limit_Command_uC_CAN);

    io::CAN::CANStatus mcMessageStatus = powertrainCAN.sendMCMessage();

#ifdef EVT_CORE_LOG_ENABLE
    if (mcMessageStatus != io::CAN::CANStatus::OK) {
        log::LOGGER.log(core::log::Logger::LogLevel::WARNING, "MC Message Failed with error %d", mcMessageStatus);
    }
#endif

    powertrainCAN.setBMSContactor(static_cast<int16_t>(modelOutputs.BMS_Contactor_Command_uC_CAN));
    io::CAN::CANStatus bmsMessageStatus = powertrainCAN.sendBMSMessage();

#ifdef EVT_CORE_LOG_ENABLE
    if (bmsMessageStatus != io::CAN::CANStatus::OK) {
        log::LOGGER.log(core::log::Logger::LogLevel::WARNING, "BMS Message Failed with error %d", bmsMessageStatus);
    }
#endif

    io::CAN::CANStatus gfdbMessageStatus = io::CAN::CANStatus::OK;

    // Flag is set by RTOS gfdbTimer
    if (groundFaultRequestFlag) {
        gfdbMessageStatus = powertrainCAN.sendGFDBStateRequest();
        groundFaultRequestFlag = false;
    }

#ifdef EVT_CORE_LOG_ENABLE
    if (gfdbMessageStatus != io::CAN::CANStatus::OK) {
        log::LOGGER.log(core::log::Logger::LogLevel::WARNING, "GFDB Isolation State Message Failed with error %d", gfdbMessageStatus);
    }
#endif

    sendOutputDataToUnsafeBuffer();
    bufferMutex.put();

#ifdef EVT_CORE_LOG_ENABLE
    halend = core::time::millis();

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

    if (ucState.stateEnum != lastState) {
        lastState = ucState.stateEnum;
        return true;
    } else {
        return false;
    }
}

} // namespace vcu
