#ifndef VCU_HPP
#define VCU_HPP

#include <core/io/CANDevice.hpp>
#include <core/io/CANOpenMacros.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/rtos/Initializable.hpp>
#include <core/rtos/Mutex.hpp>
#include <core/rtos/EventFlags.hpp>

#include <PowertrainCAN.hpp>
#include <models/MCuC_Model.hpp>

namespace io   = core::io;
namespace rtos = core::rtos;

namespace vcu {

/**
 * The MCuC (Motor Controller microController) is one of two microcontrollers on the VCU board.
 * It receives a variety of input over CAN and CANOpen and processes those signals to control the state of the bike as a
 * whole. The raw CAN line, called the PowertrainCAN, includes the motor controller, HIB, and other critical
 * peripherals. The CANOpen line, called the AccessoryCAN, includes accessory devices that are generally not as
 * necessary for the bike functioning (with exception of the LVSS, which enables every other board).
 *
 * Given all of this input, the MCuC then runs the MCuC Model, which is a Simulink Model compiled to C++ code.
 * The MCuC Model determines what outputs must be sent out to other devices on the bike in order to control their
 * functioning. It is designed to be run as one thread of a process, and thus has built in mutual exclusion properties.
 *
 * The Hardmon (the other microcontroller on the VCU board), monitors the MCuC to ensure that it is functional and
 * operating safely. It has the ability to override the MCuC and even nReset if it determines that the MCuC is operating
 * unsafely.
 */
class MCuC : public CANDevice, public rtos::Initializable {
public:
    //////////////////////////////////////////////
    ///              MCUC Pinout               ///
    //////////////////////////////////////////////

    /** ESTOP Level Shifter (LS) Pins */
    static constexpr io::Pin ESTOP_A_PIN = io::Pin::PA_0;
    static constexpr io::Pin ESTOP_B_PIN = io::Pin::PA_1;

    /** Ignition Level Shifter (LS) Pins */
    static constexpr io::Pin IGNITION_A_PIN = io::Pin::PA_2;
    static constexpr io::Pin IGNITION_B_PIN = io::Pin::PA_3;

    /** Motor Controller Self Test Pin */
    static constexpr io::Pin LS_SELF_TEST_OUT_PIN  = io::Pin::PA_4;
    static constexpr io::Pin LS_SELF_TEST_IN_A_PIN = io::Pin::PA_5;
    static constexpr io::Pin LS_SELF_TEST_IN_B_PIN = io::Pin::PA_6;

    /** Interlock Pin */
    static constexpr io::Pin INTERLOCK_PIN = io::Pin::PA_7;

    /** UART TX Pin */
    static constexpr io::Pin UART_TX = io::Pin::PB_6;
    /** UART RX Pin */
    static constexpr io::Pin UART_RX = io::Pin::PB_7;

    /** Accessory CAN RX pin */
    static constexpr io::Pin ACCESSORY_CAN_RX_PIN = io::Pin::PA_11;
    /** Accessory CAN TX pin */
    static constexpr io::Pin ACCESSORY_CAN_TX_PIN = io::Pin::PA_12;

    /** Watchdog pin */
    static constexpr io::Pin WATCHDOG_PIN = io::Pin::PA_15;

    /** LED Pins */
    static constexpr io::Pin FAULT_LED_PIN       = io::Pin::PB_4;
    static constexpr io::Pin SUPER_FAULT_LED_PIN = io::Pin::PB_5;

    /** Powertrain CAN RX Pin*/
    static constexpr io::Pin POWERTRAIN_CAN_RX_PIN = io::Pin::PB_12;
    /** Powertrain CAN TX Pin*/
    static constexpr io::Pin POWERTRAIN_CAN_TX_PIN = io::Pin::PB_13;

    /** CAN Self Test Pin */
    static constexpr io::Pin CAN_SELF_TEST_PIN = io::Pin::PB_14;

    /** Motor Controller Toggle Pins */
    static constexpr io::Pin MC_TOGGLE_NEGATIVE_PIN = io::Pin::PC_0;
    static constexpr io::Pin MC_TOGGLE_POSITIVE_PIN = io::Pin::PC_1;

    /** Motor Controller Status Pin */
    static constexpr io::Pin MC_STATUS_PIN = io::Pin::PC_2;

    /** Motor Controller Relay Self Test */
    static constexpr io::Pin MC_RELAY_SELF_TEST_PIN = io::Pin::PC_3;

    /** LVSS Enable Pin */
    static constexpr io::Pin LVSS_ENABLE_PIN = io::Pin::PC_4;

    /** Microcontroller State Pins */
    static constexpr io::Pin UC_STATE_ZERO_PIN  = io::Pin::PC_6;
    static constexpr io::Pin UC_STATE_ONE_PIN   = io::Pin::PC_7;
    static constexpr io::Pin UC_STATE_TWO_PIN   = io::Pin::PC_8;
    static constexpr io::Pin UC_STATE_THREE_PIN = io::Pin::PC_9;

    /** LED Pins */
    static constexpr io::Pin LED_ONE_PIN   = io::Pin::PC_10;
    static constexpr io::Pin LED_TWO_PIN   = io::Pin::PC_11;
    static constexpr io::Pin LED_THREE_PIN = io::Pin::PC_12;

    /**
     * Length of Heartbeat array (number of boards to listen for over CAN)
     * Must match length of Heartbeats_CAN array in the MCuC model
     */
    static constexpr uint8_t HB_SIZE = 5;

    static constexpr uint8_t BMS_CELL_TEMP_LEN = 45;
    static constexpr uint8_t BMS_CELL_VOLT_LEN = 100;

    static constexpr uint32_t LVSS_POWER_CMD_TPDO_NUM = 0x00;
    static constexpr uint32_t SIM_STATE_TPDO_NUM = 0x01;
    static constexpr uint32_t HEALTH_FLAG_TPDO_NUM = 0x02;

    static constexpr uint32_t MC_FR_IDX = 0; // flowrate index of MC FR from TMS on Accessory CAN
    static constexpr uint32_t BATT_FR_IDX = 1; // flowrate index of Battery FR from TMS on Accessory CAN

    static constexpr uint32_t LVSS_OUT_CHANGED_MASK = 1 << 14;
    static constexpr uint32_t VCU_STATE_CHANGE_MASK = 1 << 15;

    /**
     * Struct that contains all the GPIOs that an instance of this class requires.
     */
    union MCuC_GPIO {
        struct {
            // model input GPIOs
            io::GPIO& eStopAGPIO;
            io::GPIO& eStopBGPIO;

            io::GPIO& ignitionAGPIO;
            io::GPIO& ignitionBGPIO;

            io::GPIO& interlockGPIO;
            io::GPIO& mcStatusGPIO;

            io::GPIO& lsSelfTestInAGPIO;
            io::GPIO& lsSelfTestInBGPIO;

            // model output GPIOs
            io::GPIO& lsSelfTestOutGPIO;
            io::GPIO& watchdogGPIO;

            io::GPIO& faultLEDGPIO;
            io::GPIO& superFaultLEDGPIO;

            io::GPIO& canSelfTestGPIO;

            io::GPIO& mcToggleNegativeGPIO;
            io::GPIO& mcTogglePositiveGPIO;
            io::GPIO& mcSelfTestGPIO;

            io::GPIO& lvssEnableGPIO;

            io::GPIO& ucStateZeroGPIO;
            io::GPIO& ucStateOneGPIO;
            io::GPIO& ucStateTwoGPIO;
            io::GPIO& ucStateThreeGPIO;

            io::GPIO& ledOneGPIO;
            io::GPIO& ledTwoGPIO;
            io::GPIO& ledThreeGPIO;
        };
        struct {
            io::GPIO* inputArr[8];
            io::GPIO* outputArr[16];
        };
    };

    /**
     * Union to hold which switches are being powered from LVSS. Used to receive currently enabled boards from LVSS,
     * and also used to send to LVSS to turn on / off switches.
     */
    typedef union {
        uint16_t val;
        struct {
            // Power Switch 0
            uint8_t batt : 1;
            uint8_t hib  : 1;

            // Power Switch 1
            uint8_t tms  : 1;
            uint8_t hudl : 1;

            // Power Switch 2
            uint8_t gub : 1;
            uint8_t acc : 1;
        };
    } LVSSPowerState_t;

    /**
     * Union to hold the fault status' from the LVSS for the switches currents and temperatures
     */
    typedef union {
        uint16_t val;
        struct {
            // Power Switch 0
            uint16_t battCurrentFault : 1;
            uint16_t hibCurrentFault  : 1;

            // Power Switch 1
            uint16_t tmsCurrentFault  : 1;
            uint16_t hudlCurrentFault : 1;

            // Power Switch 2
            uint16_t accCurrentFault  : 1;
            uint16_t gubCurrentFault  : 1;

            uint16_t switch0TempFault : 1;
            uint16_t switch1TempFault : 1;
            uint16_t switch2TempFault : 1;
        };
    } LVSSSwitchFaults_t;

    /**
     * Struct that contains all the data that AccessoryCan should read in.
     * Used for double buffering for threadsafety with CANOpen
     */
    typedef union {
        struct {
            LVSSPowerState_t LVSS_out_EnableBoardSignal;    ///< LVSS (out): Determines which boards it will send power to
            uint16_t LVSS_in_PowerSwitchCurrents[6];        ///< LVSS (in): switch currents
            uint16_t LVSS_in_PowerSwitchTemperatures[3];    ///< LVSS (in): switch temps
            int16_t LVSS_in_VicorCurrent;                  ///< LVSS (in): hv vicor current
            LVSSSwitchFaults_t LVSS_in_SwitchFaults;        ///< LVSS (in): switch faults
            LVSSPowerState_t LVSS_in_EnableBoardSignal;     ///< LVSS (in): what LVSS is powering
            int16_t TMS_in_FlowRates[2]; ///< TMS (in): The cooling loop flow rates.
            int16_t TMS_in_Temps[5]; ///< TMS (in): The cooling loop temperatures.
        };
        struct {
            uint16_t outputs[1];
            uint16_t inputs[19];
        };
    } AccessoryCanData_t;

    /**
     * Union that represents the state of the microcontroller
     */
    union UCState {
        struct {
            int16_t padding   : 12;
            int16_t stateBit0 : 1;
            int16_t stateBit1 : 1;
            int16_t stateBit2 : 1;
            int16_t stateBit3 : 1;
        };
        UC_State stateEnum;
    };

    /**
     * Union to hold the VCU's internal status, in the form of error flags. This is the payload that is sent over CanOpen.
     * *NotRun -> * was not run in the last health check period.
     * *RanErr -> * had an error in the running of the code in the last health check period.
     * *SpeedErr -> * did not run fast enough in the last health check period.
     */
    typedef union {
        uint16_t flags;
        struct {
            int16_t padding   : 10;
            int16_t gfdbReqNotRun : 1;
            int16_t canopenNotRun : 1;
            int16_t ptcanISRErr : 1;
            int16_t ptcanRanErr : 1;
            int16_t modelRanErr : 1;
            int16_t modelSpeedErr : 1;
        };
    } HealthFlags_t;

    /**
     * Constructor for MCuC object
     */
    MCuC(MCuC_GPIO gpios, io::CAN& ptCAN);

    /**
     * Handles the passed in Powertrain CAN message.
     *
     * @param[in] message message to handle
     */
    void handlePowertrainCanMessage(io::CANMessage& message);

    /**
     * Sends the provided message to the Powertrain CAN's Queue. If the queue is full, the calling thread
     * will suspend for waitOption ticks.
     *
     * @param[in] messagePointer the pointer to the passed in message
     * @param[in] waitOption How long to wait (in ticks). use rtos::TXWait::TXWaitForever to wait forever
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    rtos::TXError sendToPowertrainQueue(io::CANMessage* messagePointer, uint32_t waitOption);

    /**
     * Pops and copies the first message in the Powertrain CAN's Queue to the destination. If the queue is empty,
     * the calling thread will suspend for waitOption ticks.
     *
     * @param[in] destination the pointer to the destination
     * @param[in] waitOption How long to wait (in ticks). use rtos::TXWait::TXWaitForever to wait forever
     * @return The first error found by the function or TXE_SUCCESS if there was no error
     */
    rtos::TXError receiveFromPowertrainQueue(io::CANMessage* destination, uint32_t waitOption);

    /**
     * Runs one step of the Simulink model, including processing and handling inputs and outputs of the model.
     * @param[in] flags the rtos EventFlags so we can set flags for the thread to alert canOpen to send messages.
     */
    void process(core::rtos::EventFlags* flags);

    // override methods from Initializable
    rtos::TXError init(rtos::BytePoolBase& pool) override;

    // override methods from CANDevice

    CO_OBJ_T* getObjectDictionary() override;

    uint8_t getNumElements() override;

    uint8_t getNodeID() override;

    /**
     * Unsafe (non-mutexed) Buffer Data that comes in or is sent out over Accessory CAN.
     */
    AccessoryCanData_t accessoryCanDataUnsafeBuffer;

    /**
     * Copies the output data in the accessoryCANData Safe buffer to the Unsafe buffer.
     */
    void sendOutputDataToUnsafeBuffer();

    /**
     * Copies the input data in the accessoryCANData Unsafe buffer to the Safe buffer.
     */
    void sendInputDataToSafeBuffer();

    /**
     * Updates the heartbeat array for CanOpen nodes as a notice of receiving a message from said CanOpen node.
     *
     * @param nodeId the CanOpen ID of the node to accept heartbeat from
     */
    void updateCanOpenNodeHeartbeat(uint32_t nodeId);

    /**
     * Updates the heartbeat array for raw Can nodes as a notice of receiving a message from said CAN node.
     * @param nodeId the Can ID of the node to accept heartbeat from
     */
    void updateCanNodeHeartbeat(uint32_t nodeId);

    /**
     * Sets private groundFaultRequestFlag variable to true, which is then used
     * in process() to send CAN request.
     */
    void setGroundFaultFlag();

    /**
     * Takes the boolean flags from the health thread and sets variable for CanOpen to send.
     * @param modelSpeedErr true if model was attempted to be triggered but last cycle hasn't finished yet, else false
     * @param modelRanErr true if model thread has NOT run since last health thread cycle, else false
     * @param ptcanRanErr true if ptcan thread has NOT run since last health thread cycle, else false
     * @param ptcanISRErr true if ptcan ISR had an error adding message to queue, else false
     * @param canopenNotRun true if canopen thread has NOT run since last health thread cycle, else false
     * @param gfdbReqNotRun true if GFDB Request thread has NOT run since last health thread cycle, else false
     */
    void setHealthFlags(bool modelSpeedErr, bool modelRanErr, bool ptcanRanErr, bool ptcanISRErr, bool canopenNotRun, bool gfdbReqNotRun);

private:
    /**
     * Mutex that protects internal buffer access to the MCuC
     */
    rtos::Mutex bufferMutex;

    /**
     * Mutex that protects internal CAN heartbeat data access to the MCuC
     */
    rtos::Mutex hbMutex;

    /**
     * Local instance of PowertrainCan
     */
    dev::PowertrainCAN powertrainCAN;

    /**
     * The model that is determining our control flow
     * Automatically constructed here (not to be passed in)
     */
    MCuC_Model model;

    /**
     * Safe (mutexed) Buffer Data that comes in or is sent out over Accessory CAN.
     */
    AccessoryCanData_t accessoryCanDataSafeBuffer;

    /**
     * Flag for when to send the GFDB Isolation state CAN request
     */
    volatile bool groundFaultRequestFlag = false;

    /**
     * Flags from Health Thread to be sent over CANOpen (Accessory CAN)
     */
    HealthFlags_t healthFlags = {0};

    /// Instance of the struct that contains all the GPIOs that an instance of this class requires.
    MCuC_GPIO gpios;

    // Model input data
    bool powertrainCANSelfTestIn = false;             ///< CAN (Hardmon): If the powertrain CAN network is working.
    MC_DC_State mcDischarge  = MC_DC_State::Disabled; ///< CAN (MC): What state the MC discharger is in. [0,4] range.
    MC_VSM_State mcState     = MC_VSM_State::Start; ///< CAN (MC): What state the MC state machine is in. [0,14] range.
    bool forwardEnable       = false;               ///< CAN (HIB): Whether or not the bike is commanded to go forward.
    bool startPressed        = false;               ///< CAN (HIB): Whether or not the bike is starting.
    bool brakeOn             = false;               ///< CAN (HIB): Whether or not the brake is on.
    bool hibComparisonFault  = false;               ///< CAN (HIB): Whether or not there is a HIB comparison fault.
    int16_t throttle         = 0;                   ///< CAN (HIB): Signal state of the throttle.
    int32_t bmsCellTemps[BMS_CELL_TEMP_LEN] = {0};       ///< CAN (BMS): The cell temperatures.
    int16_t bmsCellVoltages[BMS_CELL_VOLT_LEN] = {0};    ///< CAN (BMS): The cell voltages.
    bool bmsContactorClosed      = false;           ///< CAN (BMS): Whether or not the contactor is closed.
    uint8_t gfdbIsolationState   = 0;               ///< CAN (GFDB): The isolation state int value.

    /**
     * Array holding number of messages received from each of the other boards.
     * Used to hold data before sending it to Simulink model for heartbeat checking.
     *
     * Volatile because it is updated from the canOpen interrupt, as that is the only place we have access to CanOpen Node ID's.
     */
    volatile uint32_t heartbeatMessages[HB_SIZE] = {0};

    // Model input data (struct)
    vcu::MCuC_Model::ExtU_MCuC_T modelInputs;

    // Model output data (only the ones that need a var) some outputs are used directly; like writing to pin
    UCState ucState; ///< GPIO: Current State of the MCUC;
    bool mcEnableUC =
        false; ///< GPIO: controls mcTogglePositive and mcToggleNegative to enable or disable the motor controller
    bool powertrainCanSelfTestOut =
        false; ///< CAN (Hardmon): Whether a self-test message should be sent to the Hardmon over powertrainCAN

    // Tick on which we received most recent message from LVSS on CanOpen
    uint32_t lvssLastMessageTick = 0;

    // todo: this will need to be figured out what value works
    static constexpr uint32_t LVSS_MESSAGE_LIFESPAN = 750; // how long after receiving a CanOpen msg from LVSS to consider it "enabled"

    // Model output data (struct)
    vcu::MCuC_Model::ExtY_MCuC_T modelOutputs;

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID      = 0;
    static constexpr uint8_t LVSS_NODE_ID = 1;
    static constexpr uint8_t TMS_NODE_ID  = 2;
    static constexpr uint8_t IMU_NODE_ID  = 5;
    /**
     * The size of the Object Dictionary
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 114;

    /**
     * The object dictionary itself. Will be populated by this object during
     * construction.
     *
     * The plus one is for the special "end of dictionary" marker.
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        // RPDOS settings
        //------LVSS Settings--------//
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x00, 0x00, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x01, 0x01, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x02, 0x02, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),

        //------TMS Settings--------//
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x03, 0x00, TMS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC), // todo: make sure these tpdo nums are correct
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x04, 0x01, TMS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC), // todo: make sure these tpdo nums are correct
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x05, 0x02, TMS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC), // todo: make sure these tpdo nums are correct

        //------LVSS Mapping------//
        // 4 currents
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x00, 0x04),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x03, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x04, PDO_MAPPING_UNSIGNED16),

        // other 2 currents + vicor current + power switch faults
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x01, 0x04),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x03, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x04, PDO_MAPPING_UNSIGNED16),

        // temperatures 0-3 & board-enabled bit-packed value
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x02, 0x04),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x03, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x04, PDO_MAPPING_UNSIGNED16),

        //------TMS Mapping------//
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x03,0x04),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03,0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03,0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03,0x03, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03,0x04, PDO_MAPPING_UNSIGNED16),

        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x04,0x04),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x04,0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x04,0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x04,0x03, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x04,0x04, PDO_MAPPING_UNSIGNED16),

        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x05,0x02),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x05,0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x05,0x02, PDO_MAPPING_UNSIGNED16),


        // TPDO Setting
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(LVSS_POWER_CMD_TPDO_NUM, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 1000),
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(SIM_STATE_TPDO_NUM, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 0),
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(HEALTH_FLAG_TPDO_NUM, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 0),

        // Send EnableBoardSignal to LVSS
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(LVSS_POWER_CMD_TPDO_NUM, 0x01),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(LVSS_POWER_CMD_TPDO_NUM, 0x01, PDO_MAPPING_UNSIGNED16),

        // Send simulink state out when triggered by code
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(SIM_STATE_TPDO_NUM, 0x01),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(SIM_STATE_TPDO_NUM, 0x01, PDO_MAPPING_UNSIGNED16),

        // Send health flags out when triggered by code
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(HEALTH_FLAG_TPDO_NUM, 0x01),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(HEALTH_FLAG_TPDO_NUM, 0x01, PDO_MAPPING_UNSIGNED16),

        // data links
        // TPDO Datalinks
        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(LVSS_POWER_CMD_TPDO_NUM), 0x01),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(LVSS_POWER_CMD_TPDO_NUM), 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_out_EnableBoardSignal.val),

        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(SIM_STATE_TPDO_NUM), 0x01),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(SIM_STATE_TPDO_NUM), 0x01, CO_TUNSIGNED16, &ucState),

        DATA_LINK_START_KEY_21XX(LINK_TPDO_NUMBER(HEALTH_FLAG_TPDO_NUM), 0x01),
        DATA_LINK_21XX(LINK_TPDO_NUMBER(HEALTH_FLAG_TPDO_NUM), 0x01, CO_TUNSIGNED16, &healthFlags.flags),

        // RPDO Datalinks
        //------LVSS--------//
        DATA_LINK_START_KEY_21XX(LINK_RPDO_NUMBER(0x00), 0x04),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x00), 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[0]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x00), 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[1]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x00), 0x03, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[2]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x00), 0x04, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[3]),

        DATA_LINK_START_KEY_21XX(LINK_RPDO_NUMBER(0x01), 0x04),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x01), 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[4]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x01), 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[5]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x01), 0x03, CO_TSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_VicorCurrent),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x01), 0x04, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_SwitchFaults.val),

        DATA_LINK_START_KEY_21XX(LINK_RPDO_NUMBER(0x02), 0x04),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x02), 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchTemperatures[0]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x02), 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchTemperatures[1]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x02), 0x03, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchTemperatures[2]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x02), 0x04, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_EnableBoardSignal.val),

        //------TMS--------//
        DATA_LINK_START_KEY_21XX(LINK_RPDO_NUMBER(0x03), 0x04),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x03), 0x01, CO_TSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_Temps[0]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x03), 0x02, CO_TSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_Temps[1]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x03), 0x03, CO_TSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_Temps[2]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x03), 0x04, CO_TSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_Temps[3]),

        DATA_LINK_START_KEY_21XX(LINK_RPDO_NUMBER(0x04), 0x04),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x04), 0x01, CO_TSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_Temps[4]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x04), 0x02, CO_TSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_Temps[0]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x04), 0x03, CO_TSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_Temps[0]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x04), 0x04, CO_TSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_Temps[0]),

        DATA_LINK_START_KEY_21XX(LINK_RPDO_NUMBER(0x05), 0x02),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x05), 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_FlowRates[0]),
        DATA_LINK_21XX(LINK_RPDO_NUMBER(0x05), 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.TMS_in_FlowRates[1]),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};

} // namespace vcu

#endif // VCU_HPP
