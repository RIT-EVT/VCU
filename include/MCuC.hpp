#ifndef VCU_HPP
#define VCU_HPP

#include <core/io/CANDevice.hpp>
#include <core/io/CANOpenMacros.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/rtos/Initializable.hpp>
#include <core/rtos/Mutex.hpp>

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

    // todo: Havent even looked at CanOpen yet
    /**
     * Struct that contains all the data that AccessoryCan should read in.
     * Used for double buffering for threadsafety with CANOpen
     */
    typedef union {
        struct {
            uint16_t LVSS_out_EnableBoardSignal;        ///< LVSS (out) Determines which boards it will send power to
            uint16_t LVSS_in_HVCurrent[2];              ///< LVSS (in)
            uint16_t LVSS_in_PowerSwitchCurrents[4];    ///< LVSS (in)
            uint16_t LVSS_in_Temperatures[2];           ///< LVSS (in)
            uint16_t LVSS_in_PowerSwitchErrorStatus[3]; ///< LVSS (in)
        };
        struct {
            uint16_t outputs[1];
            uint16_t inputs[11];
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
     * Runs one step of the Hardmon model, including processing and handling inputs and outputs of the model.
     */
    void process();

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
     * Updates the heartbeat array as a notice of receiving a message from a CAN node.
     * @param nodeId the CAN ID of the node to accept heartbeat from
     */
    void updateNodeHeartbeat(uint32_t nodeId);

    /**
     * Sets private groundFaultRequestFlag variable to true, which is then used
     * in process() to send CAN request.
     */
    void setGroundFaultFlag();

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

    /// the gpios
    MCuC_GPIO gpios;

    // Model input data
// TODO: Should i delete? no point to have GPIO inputs put in middle-man var
//    bool eStopA      = false; ///< GPIO: LS A; Whether or not the emergency stop is enabled.
//    bool eStopB      = false; ///< GPIO: LS B; Whether or not the emergency stop is enabled.
//    bool ignitionOnA = false; ///< GPIO: LS A; Whether or not the ignition is on.
//    bool ignitionOnB = false; ///< GPIO: LS B; Whether or not the ignition is on.
//    bool interlock   = false; ///< GPIO: if BFC is plugged in.
//    bool mcOn        = false; ///< GPIO: Whether or not the motor controller is on.

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
    uint8_t battPSPresent        = 0;               ///< CAN (TMS): The battery's pump speed.
    int32_t coolingLoopTemps[5]  = {0};             ///< CAN (TMS): The cooling loop temperatures.
    int16_t mcCoolingFR          = 0;               ///< CAN (TMS): The MC's cooling flow rate.
    int16_t battCoolingFR        = 0;               ///< CAN (TMS): The battery's cooling flow rate.
    uint8_t mcPSPresent          = 0;               ///< CAN (TMS): The MC pump speed.
    bool lvssOn                  = false;           ///< CAN (LVSS): Whether or not the LVSS is on.
    bool hibOn                   = false;           ///< CAN (LVSS): Whether or not the HIB is on.
    bool hudlOn                  = false;           ///< CAN (LVSS): Whether or not the HUDL is on.
    bool tmsOn                   = false;           ///< CAN (LVSS): Whether or not the TMS is on.
    bool gubOn                   = false;           ///< CAN (LVSS): Whether or not the GUB is on.
    bool batt12vOn               = false;           ///< CAN (LVSS): Whether or not the 12V battery is on.
    int16_t vicorInputCurrent    = 0;               ///< CAN (LVSS): The Vicor's input current.
    int32_t lvssTemps[6]         = {0};             ///< CAN (LVSS): The LVSS temperatures.
    int16_t lvssCurrents[6]      = {0};             ///< CAN (LVSS): The LVSS currents.

    /**
     * Array holding number of messages received from each of the other boards.
     * Used to hold data before sending it to Simulink model for heartbeat checking.
     */
    uint32_t heartbeatMessages[HB_SIZE] = {0};

    // Model input data (struct)
    vcu::MCuC_Model::ExtU_MCuC_T modelInputs;

    // Model output data (only the ones that need a var) some outputs are used directly; like writing to pin
    UCState ucState; ///< GPIO: Current State of the MCUC;
    bool mcEnableUC =
        false; ///< GPIO: controls mcTogglePositive and mcToggleNegative to enable or disable the motor controller
    bool powertrainCanSelfTestOut =
        false; ///< CAN (Hardmon): Whether a self-test message should be sent to the Hardmon over powertrainCAN

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
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 64; // TODO: CANopen set size of object dictionary

    /**
     * The object dictionary itself. Will be populated by this object during
     * construction.
     *
     * The plus one is for the special "end of dictionary" marker.
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000), //2000ms
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        //MY STUFF --------------------------------------------------
        /* --- Receive PDOs --- */
        //RPDO 0 HV Current (100ms)
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x00, 0x00, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x00, 0x02), //RPDO 0 quantity 2
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x02, PDO_MAPPING_UNSIGNED16),

        //RPDO 1 Power Switch Error Status (100ms)
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x01, 0x01, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x01, 0x03), //RPDO 1 quantity 3
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x03, PDO_MAPPING_UNSIGNED16),

        //RPDO 2 Power Switch currents (1000ms)
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x02, 0x02, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x02, 0x04), //RPDO 2 quantity 4
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x03, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x04, PDO_MAPPING_UNSIGNED16),

        //RPDO 3 Power Switch Temperature (1000ms)
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x03, 0x03, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x03, 0x02), //RPDO 4 quantity 2
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03, 0x02, PDO_MAPPING_UNSIGNED16),

        /* -- Transmit PDOs -- */

        //TPDO 0 VCU State (On Transition) TODO This could be wrong
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_ASYNC, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 0),
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x01),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),

        /* -- Data Links -- */
        //HV current
        DATA_LINK_START_KEY_21XX(0x00, 0x02), //RPDO 0 quantity 2
        DATA_LINK_21XX(0x00, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_HVCurrent[0]),
        DATA_LINK_21XX(0x00, 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_HVCurrent[1]),

        //Power Switch Error Status
        DATA_LINK_START_KEY_21XX(0x01, 0x03), //RPDO 1 quantity 3
        DATA_LINK_21XX(0x01, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[0]),
        DATA_LINK_21XX(0x01, 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[1]),
        DATA_LINK_21XX(0x01, 0x03, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[2]),

        //Power Switch Currents
        DATA_LINK_START_KEY_21XX(0x02, 0x04), //RPDO 2, quantity 4
        DATA_LINK_21XX(0x02, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_Temperatures[0]),
        DATA_LINK_21XX(0x02, 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_Temperatures[1]),
        DATA_LINK_21XX(0x02, 0x03, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_Temperatures[2]),
        DATA_LINK_21XX(0x02, 0x04, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_Temperatures[3]),

        //Power Switch Temperature
        DATA_LINK_START_KEY_21XX(0x03, 0x02),
        DATA_LINK_21XX(0x03, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchErrorStatus),
        DATA_LINK_21XX(0x03, 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchErrorStatus),

        //VCU Outputs (State and Enable Board)


        // End marker

        //BEFORE STUFF--------------------------------------------------
        // RPDOS and data links
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x00, 0x00, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x01, 0x01, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x02, 0x02, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x03, 0x03, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),

        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x00, 0x02),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x02, PDO_MAPPING_UNSIGNED16),

        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x01, 0x04),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x03, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x04, PDO_MAPPING_UNSIGNED16),

        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x02, 0x02),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x02, PDO_MAPPING_UNSIGNED16),

        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x03, 0x03),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03, 0x01, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03, 0x02, PDO_MAPPING_UNSIGNED16),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03, 0x03, PDO_MAPPING_UNSIGNED16),

        // Actual TPDO
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 500),
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x01),
        {
            .Key  = CO_KEY(0x1A00 + 0x00, 0x01, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED32,
            .Data = (CO_DATA) CO_LINK(0x2200 + 0x00, 0x00 + 0x01, PDO_MAPPING_UNSIGNED16),
        },
        // data links
        // LVSS!!!!
        // HV Current Data
        DATA_LINK_START_KEY_21XX(0x00, 0x02),
        DATA_LINK_21XX(0x00, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_HVCurrent[0]),
        DATA_LINK_21XX(0x00, 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_HVCurrent[1]),

        // Power Switch Error Status Data
        DATA_LINK_START_KEY_21XX(0x01, 0x04),
        DATA_LINK_21XX(0x01, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[0]),
        DATA_LINK_21XX(0x01, 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[1]),
        DATA_LINK_21XX(0x01, 0x03, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[2]),
        DATA_LINK_21XX(0x01, 0x04, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents[3]),

        // Power Switch Currents Data
        DATA_LINK_START_KEY_21XX(0x02, 0x02),
        DATA_LINK_21XX(0x02, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_Temperatures[0]),
        DATA_LINK_21XX(0x02, 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_Temperatures[1]),

        // LVSS Temperature Data
        DATA_LINK_START_KEY_21XX(0x03, 0x03),
        DATA_LINK_21XX(0x03, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchErrorStatus),
        DATA_LINK_21XX(0x03, 0x02, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchErrorStatus),
        DATA_LINK_21XX(0x03, 0x03, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchErrorStatus),

        // todo: need to add CANOPEN transmits


        // Transmit DATA
        {
            .Key  = CO_KEY(0x2200 + 0x00, 0x00, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED8,
            .Data = (CO_DATA) 0x01,
        },
        {
            .Key  = CO_KEY(0x2200 + 0x00, 0x01, CO_OBJ____PRW),
            .Type = CO_TUNSIGNED16,
            .Data = (CO_DATA) &accessoryCanDataUnsafeBuffer.LVSS_out_EnableBoardSignal,
        },
        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};

} // namespace vcu

#endif // VCU_HPP
