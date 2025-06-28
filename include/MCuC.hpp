#ifndef VCU_HPP
#define VCU_HPP

#include <PowertrainCAN.hpp>
#include <core/io/CANDevice.hpp>
#include <core/io/CANOpenMacros.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>
#include <core/io/types/CANMessage.hpp>
#include <models/MCuC_Model.hpp>

#include <core/rtos/Initializable.hpp>
#include <core/rtos/Mutex.hpp>

namespace io = core::io;
namespace rtos = core::rtos;

namespace vcu {

/**
 * The MCuC (Motor Controller microController) is one of two microcontrollers on the VCU board.
 * It receives a variety of input over CAN and CANOpen and processes those signals to control the state of the bike as a whole.
 * The raw CAN line, called the PowertrainCAN, includes the motor controller, HIB, and other critical peripherals.
 * The CANOpen line, called the AccessoryCAN, includes accessory devices that are generally not as necessary for
 * the bike functioning (with exception of the LVSS, which enables every other board).
 *
 * Given all of this input, the MCuC then runs the MCuC Model, which is a Simulink Model compiled to C++ code.
 * The MCuC Model determines what outputs must be sent out to other devices on the bike in order to control their functioning.
* It is designed to be run as one thread of a process, and thus has built in mutual exclusion properties.
 *
 * The Hardmon (the other microcontroller on the VCU board), monitors the MCuC to ensure that it is functional and operating safely.
 * It has the ability to override the MCuC and even nReset if it determines that the MCuC is operating unsafely.
 */
class MCuC : public CANDevice, public rtos::Initializable {
public:
    //////////////////////////////////////////////
    ///              MCUC Pinout               ///
    //////////////////////////////////////////////

    /** LED Pins */

    static constexpr io::Pin LED_THREE_PIN = io::Pin::PC_12;
    static constexpr io::Pin LED_TWO_PIN = io::Pin::PC_11;
    static constexpr io::Pin LED_ONE_PIN = io::Pin::PC_10;
    /** Self Test Led Pin */
    //static constexpr io::Pin SELF_TEST_LED_PIN = io::Pin::PC_5; (Gone)
    static constexpr io::Pin FAULT_LED_PIN = io::Pin::PB_4;
    static constexpr io::Pin SUPER_FAULT_LED_PIN = io::Pin::PB_5;

    /** CAN Pins */

    /** Accessory CAN RX pin */
    static constexpr io::Pin ACCESSORY_CAN_RX_PIN = io::Pin::PA_11;
    /** Accessory CAN TX pin */
    static constexpr io::Pin ACCESSORY_CAN_TX_PIN = io::Pin::PA_12;
    /** Powertrain CAN RX Pin*/
    static constexpr io::Pin POWERTRAIN_CAN_RX_PIN = io::Pin::PB_12;
    /** Powertrain CAN TX Pin*/
    static constexpr io::Pin POWERTRAIN_CAN_TX_PIN = io::Pin::PB_13;

    /** Hardware Monitor Pins */

    /** Watchdog pin */
    static constexpr io::Pin WATCHDOG_PIN = io::Pin::PA_15;
    /** Microcontroller State Pins */
    static constexpr io::Pin UC_STATE_ZERO_PIN = io::Pin::PC_6;
    static constexpr io::Pin UC_STATE_ONE_PIN = io::Pin::PC_7;
    static constexpr io::Pin UC_STATE_TWO_PIN = io::Pin::PC_8;
    static constexpr io::Pin UC_STATE_THREE_PIN = io::Pin::PC_9;
    /** Hardware Monitor Fault Pin */
    //static constexpr io::Pin HM_FAULT_PIN = io::Pin::PC_8; (Gone)
    /** Microcontroller Fault Pin */
    //static constexpr io::Pin UC_FAULT_PIN = io::Pin::PC_9; (Gone)

    /** Self Test Pins */

    /** CAN Self Test Pin */
    static constexpr io::Pin CAN_SELF_TEST_PIN = io::Pin::PB_14;
    /** Ignition Self Test Pin */
    //static constexpr io::Pin IGNITION_SELF_TEST_PIN = io::Pin::PC_2; (Gone)
    /** ESTOP Self Test Pin */
    //static constexpr io::Pin ESTOP_SELF_TEST_PIN = io::Pin::PC_3; (Gone)
    /** Motor Controller Self Test Pin */
    static constexpr io::Pin MC_SELF_TEST_PIN = io::Pin::PC_3;
    static constexpr io::Pin LS_SELF_TEST_OUT_PIN = io::Pin::PA_4;
    static constexpr io::Pin LS_SELF_TEST_IN_A_PIN = io::Pin::PA_5;
    static constexpr io::Pin LS_SELF_TEST_IN_B_PIN = io::Pin::PA_6;

    /** Motor Controller Pins */

    /** Motor Controller Toggle Pins */
    static constexpr io::Pin MC_TOGGLE_POSITIVE_PIN = io::Pin::PC_1;
    static constexpr io::Pin MC_TOGGLE_NEGATIVE_PIN = io::Pin::PC_0;
    /** Motor Controller Enable Pins */
    //static constexpr io::Pin MC_ENABLE_POSITIVE_PIN = io::Pin::PB_6; (Gone)
    //static constexpr io::Pin MC_ENABLE_NEGATIVE_PIN = io::Pin::PB_7; (Gone)
    /** Motor Controller Status Pin */
    static constexpr io::Pin MC_STATUS_PIN = io::Pin::PC_2;

    /** UART Pins */

    /** UART TX Pin */
    static constexpr io::Pin UART_TX = io::Pin::PA_8;
    /** UART RX Pin */
    static constexpr io::Pin UART_RX = io::Pin::PA_9;

    /** Component Communication Pins */

    /** ESTOP Pin */
    static constexpr io::Pin ESTOP_A_PIN = io::Pin::PA_0;
    static constexpr io::Pin ESTOP_B_PIN = io::Pin::PA_1;
    /** Ignition Pin */
    static constexpr io::Pin IGNITION_A_PIN = io::Pin::PA_2;
    static constexpr io::Pin IGNITION_B_PIN = io::Pin::PA_3;
    /** LVSS Status Pin */
    //static constexpr io::Pin LVSS_STATUS_PIN = io::Pin::PC_12; (Gone)
    static constexpr io::Pin INTERLOCK_PIN = io::Pin::PA_7;
    /** LVSS Enable Pin */
    static constexpr io::Pin LVSS_ENABLE_PIN = io::Pin::PC_4;

    /**
     * Struct that contains all the GPIOs that an instance of this class requires.
     */
    union MCuC_GPIO {
        struct {
            //model input GPIOs
            io::GPIO& eStopGPIO;
            io::GPIO& ignitionGPIO;
            io::GPIO& mcStatusGPIO;

            //model output GPIOs
            io::GPIO& lvssEnableGPIO;
            io::GPIO& watchdogGPIO;

            io::GPIO& ucStateZeroGPIO;
            io::GPIO& ucStateOneGPIO;
            io::GPIO& ucStateTwoGPIO;
            io::GPIO& ucStateThreeGPIO;

            io::GPIO& mcToggleNegativeGPIO;
            io::GPIO& mcTogglePositiveGPIO;
            io::GPIO& mcSelfTestGPIO;

            //Set based off of ucState.
            io::GPIO& canSelfTestGPIO;

            //TODO New Pins fix this (integrate into current structure)
            io::GPIO& faultLEDGPIO;
            io::GPIO& superFaultLEDGPIO;
            io::GPIO& lsSelfTestOutGPIO;
            io::GPIO& lsSelfTestInAGPIO;
            io::GPIO& lsSelfTestInBGPIO;
            io::GPIO& estopBGPIO; //Also an "A" estop pin, but didn't want to change existing setup
            io::GPIO& ignitionBGPIO; //Same thing with ignition as with estop
            io::GPIO& interlockGPIO;
        };
        struct {
            io::GPIO* inputArr[3];
            io::GPIO* outputArr[9];
            //TODO fix this
            io::GPIO* newPinArr[8];
        };
    };

    /**
     * Struct that contains all the data that AccessoryCan should read in.
     * Used for double buffering for threadsafety with CANOpen
     */
    typedef struct AccessoryCanData_s {
        uint16_t LVSS_out_EnableBoardSignal;       ///< LVSS (out) Determines which boards it will send power to
        uint16_t LVSS_in_HVCurrent[2];             ///< LVSS (in)
        uint16_t LVSS_in_PowerSwitchCurrents[4];   ///< LVSS (in)
        uint16_t LVSS_in_Temperatures[2];          ///< LVSS (in)
        uint16_t LVSS_in_PowerSwitchErrorStatus[3];///< LVSS (in)
    } AccessoryCanData_t;

    /**
     * Union that represents the state of the microcontroller
     */
    union UCState {
        struct {
            int16_t padding : 12;
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
     * @param message message to handle
     */
    void handlePowertrainCanMessage(io::CANMessage& message);

    /**
     * Returns a pointer to the queue for Powertrain CANopen messages
     *
     * @return pointer to the fixed queue.
     */
    rtos::Queue* getPowertrainQueue();

    /**
     * Runs one step of the Hardmon model, including processing and handling inputs and outputs of the model.
     */
    void process();

    /**
     * Process method for imagine- literally just
     * reads estop and ignition and then sets the
     * LVSS signal to 63 (all on) or 0 (all off)
     */
    void imagineNeuteredProcess();

    //override methods from Initializable
    rtos::TXError init(rtos::BytePoolBase& pool) override;

    //override methods from CANDevice

    CO_OBJ_T* getObjectDictionary() override;

    uint8_t getNumElements() override;

    uint8_t getNodeID() override;

    /**
     * Unsafe (non-mutexed) Buffer Data that comes in or is sent out over Accessory CAN.
     */
    AccessoryCanData_t accessoryCanDataUnsafeBuffer;

    void sendOutputDataToUnsafeBuffer();

    void sendInputDataToSafeBuffer();

private:
    /**
     * Mutex that protects internal access to the MCuC
     */
    rtos::Mutex mutex;

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

    ///the gpios
    MCuC_GPIO gpios;

    //TODO: ask EEs about initial values (i.e. if they should be 0 or whatever)

    //Model input data
    bool brakeOn;                ///< CAN (HIB): Whether or not the brake is on.
    bool eStop;                  ///< GPIO: Whether or not the emergency stop is enabled.
    bool forwardEnable;          ///< CAN (HIB): Whether or not the bike is commanded to go forward.
    bool startPressed;           ///< CAN (HIB): Whether or not the bike is starting.
    MC_VSM_State mcState;        ///< CAN (MC): What state the motor controller state machine is in. [0,14] range
    MC_DC_State mcDischarge;     ///< CAN (MC): What state the motor controller discharger is in. [0,4] range
    bool ignitionOn;             ///< GPIO: Whether or not the ignition is on.
    bool hmFault;                ///< GPIO: Whether or not the hardware monitor is telling the MCUC to go into a fault state.
    int16_t throttle;            ///< CAN (HIB): Signal state of the throttle.
    bool lvssOn;                 ///< GPIO: Whether or not the LVSS is on.
    bool mcOn;                   ///< GPIO: Whether or not the motor controller is on.
    bool powertrainCANSelfTestIn;///< CAN (Hardmon): If the powertrain CAN network is working.
    bool accessoryCANSelfTestIn; ///< CAN (Hardmon): If the accessory CAN network is working.

    //Model input data (struct)
    vcu::MCuC_Model::ExtU_MCuC_T modelInputs;

    //Model output data
    bool lvssEnable = false;              ///< GPIO: Whether or not the lvss should be on.
    bool inverterEnable = false;          ///< CAN (MC): Whether or not the inverter on the motor controller should be enabled.
    bool ucFault = false;                 ///< GPIO: Whether or not the MCUC is in a fault state
    bool watchdog = false;                ///< GPIO: watchdog signal between the Hardmon and MCUC.
    UCState ucState;                      ///< GPIO: Current State of the MCUC;
    bool inverterDischarge = false;       ///< CAN (MC): Whether or not the inverter on the motor controller should be discharging.
    bool mcEnableUC = false;              ///< GPIO: controls mcTogglePositive and mcToggleNegative to enable or disable the motor controller
    int16_t torqueRequest;                ///< CAN (MC): How much torque the MCUC is requesting the motor controller to output
    bool mcSelfTestOut = false;           ///< GPIO: Whether or not the motor controller should be self-testing.
    bool estopSelfTestOut = false;        ///< GPIO: Whether or not the estop should be self-testing.
    bool ignitionSelfTestOut = false;     ///< GPIO: Whether or not ignition should be self-testing.
    bool accessoryCanSelfTestOut = false; ///< CAN (Hardmon): Whether a self-test message should be sent to the Hardmon over accessoryCAN
    bool powertrainCanSelfTestOut = false;///< CAN (Hardmon): Whether a self-test message should be sent to the Hardmon over powertrainCAN

    //Model output data (struct)
    vcu::MCuC_Model::ExtY_MCuC_T modelOutputs;

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 0;
    static constexpr uint8_t LVSS_NODE_ID = 1;
    static constexpr uint8_t IMU_NODE_ID = 9;

    /**
     * The size of the Object Dictionary
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 64;//TODO: CANopen set size of object dictionary

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

        //RPDOS and data links
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
            .Key = CO_KEY(0x1A00 + 0x00, 0x01, CO_OBJ_D___R_),
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

        //Transmit DATA
        {
            .Key = CO_KEY(0x2200 + 0x00, 0x00, CO_OBJ_D___R_),
            .Type = CO_TUNSIGNED8,
            .Data = (CO_DATA) 0x01,
        },
        {
            .Key = CO_KEY(0x2200 + 0x00, 0x01, CO_OBJ____PRW),
            .Type = CO_TUNSIGNED16,
            .Data = (CO_DATA) &accessoryCanDataUnsafeBuffer.LVSS_out_EnableBoardSignal,
        },
        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace vcu

#endif//VCU_HPP
