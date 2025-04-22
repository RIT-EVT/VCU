#ifndef VCU_HPP
#define VCU_HPP

#include <PowertrainCAN.hpp>
#include <core/io/CANDevice.hpp>
#include <core/io/CANOpenMacros.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/utils/types/FixedQueue.hpp>
#include <models/MCuC_Model.hpp>

#include <core/rtos/Initializable.hpp>
#include <core/rtos/Mutex.hpp>

namespace io = core::io;
namespace rtos = core::rtos;

namespace vcu {

/**
 * Driver for the VCU device
 */
class MCuC : public CANDevice, public rtos::Initializable {
public:
    //////////////////////////////////////////////
    ///              MCUC Pinout               ///
    //////////////////////////////////////////////

    /** LED Pins */

    static constexpr io::Pin LED_THREE_PIN = io::Pin::PA_0;
    static constexpr io::Pin LED_TWO_PIN = io::Pin::PA_1;
    static constexpr io::Pin LED_ONE_PIN = io::Pin::PA_2;
    /** Self Test Led Pin */
    static constexpr io::Pin SELF_TEST_LED_PIN = io::Pin::PC_5;

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
    static constexpr io::Pin UC_STATE_ZERO_PIN = io::Pin::PB_5;
    static constexpr io::Pin UC_STATE_ONE_PIN = io::Pin::PB_4;
    static constexpr io::Pin UC_STATE_TWO_PIN = io::Pin::PB_15;
    static constexpr io::Pin UC_STATE_THREE_PIN = io::Pin::PD_2;
    /** Hardware Monitor Fault Pin */
    static constexpr io::Pin HM_FAULT_PIN = io::Pin::PC_8;
    /** Microcontroller Fault Pin */
    static constexpr io::Pin UC_FAULT_PIN = io::Pin::PC_9;

    /** Self Test Pins */

    /** CAN Self Test Pin */
    static constexpr io::Pin CAN_SELF_TEST_PIN = io::Pin::PB_8;
    /** Ignition Self Test Pin */
    static constexpr io::Pin IGNITION_SELF_TEST_PIN = io::Pin::PC_2;
    /** ESTOP Self Test Pin */
    static constexpr io::Pin ESTOP_SELF_TEST_PIN = io::Pin::PC_3;
    /** Motor Controller Self Test Pin */
    static constexpr io::Pin MC_SELF_TEST_PIN = io::Pin::PC_4;

    /** Motor Controller Pins */

    /** Motor Controller Toggle Pins */
    static constexpr io::Pin MC_TOGGLE_POSITIVE_PIN = io::Pin::PC_1;
    static constexpr io::Pin MC_TOGGLE_NEGATIVE_PIN = io::Pin::PC_0;
    /** Motor Controller Enable Pins */
    static constexpr io::Pin MC_ENABLE_POSITIVE_PIN = io::Pin::PB_6;
    static constexpr io::Pin MC_ENABLE_NEGATIVE_PIN = io::Pin::PB_7;
    /** Motor Controller Status Pin */
    static constexpr io::Pin MC_STATUS_PIN = io::Pin::PB_9;

    /** UART Pins */

    /** UART TX Pin */
    static constexpr io::Pin UART_TX = io::Pin::PC_6;
    /** UART RX Pin */
    static constexpr io::Pin UART_RX = io::Pin::PC_7;

    /** Component Communication Pins */

    /** ESTOP Pin */
    static constexpr io::Pin ESTOP_PIN = io::Pin::PC_10;
    /** Ignition Pin */
    static constexpr io::Pin IGNITION_PIN = io::Pin::PC_11;
    /** LVSS Status Pin */
    static constexpr io::Pin LVSS_STATUS_PIN = io::Pin::PC_12;
    /** LVSS Enable Pin */
    static constexpr io::Pin LVSS_ENABLE_PIN = io::Pin::PC_13;

    /**
     * Accessory CAN Node IDs
     */

    static constexpr uint8_t IMU_NODE_ID = 9;

    /**
     * Struct that contains all the GPIOs that an instance of this class requires.
     */
    union MCuC_GPIO {
        struct {
            //model input GPIOs
            io::GPIO& eStopGPIO;
            io::GPIO& ignitionGPIO;
            io::GPIO& hmFaultGPIO;
            io::GPIO& lvssStatusGPIO;
            io::GPIO& mcStatusGPIO;

            //model output GPIOs
            io::GPIO& ucFaultGPIO;
            io::GPIO& lvssEnableGPIO;
            io::GPIO& watchdogGPIO;

            io::GPIO& ucStateZeroGPIO;
            io::GPIO& ucStateOneGPIO;
            io::GPIO& ucStateTwoGPIO;
            io::GPIO& ucStateThreeGPIO;

            io::GPIO& mcToggleNegativeGPIO;
            io::GPIO& mcTogglePositiveGPIO;
            io::GPIO& mcSelfTestGPIO;
            io::GPIO& estopSelfTestGPIO;
            io::GPIO& ignitionSelfTestGPIO;

            //Set based off of ucState.
            io::GPIO& canSelfTestGPIO;
        };
        struct {
            io::GPIO* inputArr[5];
            io::GPIO* outputArr[12];
        };
    };

    /**
     * Struct that contains all the data that AccessoryCan should read in.
     * Used for double buffering for threadsafety with CANOpen
     */
     struct AccessoryCanData_s {
        uint16_t LVSS_out_EnableBoardSignal;     ///< Signal sent to LVSS that determines which boards it will send power to
        uint16_t LVSS_in_HVCurrent;              ///< Signal received from LVSS
        uint16_t LVSS_in_PowerSwitchErrorStatus; ///< Signal received from LVSS
        uint16_t LVSS_in_PowerSwitchCurrents;    ///< Signal received from LVSS
        uint16_t LVSS_in_Temperatures;           ///< Signal received from LVSS
     } AccessoryCanData_t;

     /**
     * Union that represents the state of the microcontroller
     */
     union UCState {
        struct {
            int16_t padding:12;
            int16_t stateBit0:1;
            int16_t stateBit1:1;
            int16_t stateBit2:1;
            int16_t stateBit3:1;
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

    //override methods from Initializable
    rtos::TXError init(rtos::BytePoolBase& pool) override;

    //override methods from CANDevice

    CO_OBJ_T* getObjectDictionary() override;

    uint8_t getNumElements() override;

    uint8_t getNodeID() override;

    /**
     * Unsafe (non-mutexed) Buffer Data that comes in or is sent out over Accessory CAN.
     */
    AccessoryCanData_s accessoryCanDataUnsafeBuffer;

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
    AccessoryCanData_s accessoryCanDataSafeBuffer;


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

    //Model output data
    bool lvssEnable;              ///< GPIO: Whether or not the lvss should be on.
    bool inverterEnable;          ///< CAN (MC): Whether or not the inverter on the motor controller should be enabled.
    bool ucFault;                 ///< GPIO: Whether or not the MCUC is in a fault state
    bool watchdog;                ///< GPIO: watchdog signal between the Hardmon and MCUC.
    UCState ucState;              ///< GPIO: Current State of the MCUC;
    bool inverterDischarge;       ///< CAN (MC): Whether or not the inverter on the motor controller should be discharging.
    bool mcEnableUC;              ///< GPIO: controls mcTogglePositive and mcToggleNegative to enable or disable the motor controller
    int16_t torqueRequest;        ///< CAN (MC): How much torque the MCUC is requesting the motor controller to output
    bool mcSelfTestOut;           ///< GPIO: Whether or not the motor controller should be self-testing.
    bool estopSelfTestOut;        ///< GPIO: Whether or not the estop should be self-testing.
    bool ignitionSelfTestOut;     ///< GPIO: Whether or not ignition should be self-testing.
    bool accessoryCanSelfTestOut; ///< CAN (Hardmon): Whether a self-test message should be sent to the Hardmon over accessoryCAN
    bool powertrainCanSelfTestOut;///< CAN (Hardmon): Whether a self-test message should be sent to the Hardmon over powertrainCAN

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 0;
    static constexpr uint8_t LVSS_NODE_ID = 1;

    /**
     * The size of the Object Dictionary
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 49; //TODO: CANopen set size of object dictionary

    /**
     * The object dictionary itself. Will be populated by this object during
     * construction.
     *
     * The plus one is for the special "end of dictionary" marker.
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        //RPDOS and data links
        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x00, 0x00, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x00, 0x01),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x00, 0x01, PDO_MAPPING_UNSIGNED16),

        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x01, 0x01, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x01, 0x01),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x01, 0x01, PDO_MAPPING_UNSIGNED16),

        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x02, 0x02, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x02, 0x01),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x02, 0x01, PDO_MAPPING_UNSIGNED16),

        RECEIVE_PDO_SETTINGS_OBJECT_140X(0x03, 0x03, LVSS_NODE_ID, RECEIVE_PDO_TRIGGER_ASYNC),
        RECEIVE_PDO_MAPPING_START_KEY_16XX(0x03, 0x01),
        RECEIVE_PDO_MAPPING_ENTRY_16XX(0x03, 0x01, PDO_MAPPING_UNSIGNED16),


        // Actual TPDO
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x04, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 2000),
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x04, 0x01),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x04, 0x01, PDO_MAPPING_UNSIGNED16),

        // data links
        // LVSS!!!!
        // HV Current Data
        DATA_LINK_START_KEY_21XX(0x00, 0x01),
        DATA_LINK_21XX(0x00, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_HVCurrent),

        // Power Switch Error Status Data
        DATA_LINK_START_KEY_21XX(0x01, 0x01),
        DATA_LINK_21XX(0x01, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchCurrents),

        // Power Switch Currents Data
        DATA_LINK_START_KEY_21XX(0x02, 0x01),
        DATA_LINK_21XX(0x02, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_Temperatures),

        // LVSS Temperature Data
        DATA_LINK_START_KEY_21XX(0x03, 0x01),
        DATA_LINK_21XX(0x03, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_in_PowerSwitchErrorStatus),

        //Transmit DATA
        DATA_LINK_START_KEY_21XX(0x04, 0x01),
        DATA_LINK_21XX(0x04, 0x01, CO_TUNSIGNED16, &accessoryCanDataUnsafeBuffer.LVSS_out_EnableBoardSignal),

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace vcu

#endif//VCU_HPP
