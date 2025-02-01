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

namespace io = core::io;

namespace vcu {

/**
 * Driver for the VCU device
 */
class MCUC : public CANDevice {
public:
    //////////////////////////////////////////////
    ///              MCUC Pinout               ///
    //////////////////////////////////////////////

    /** LED Pins */

    static constexpr IO::Pin LED_THREE_PIN = IO::Pin::PA_0;
    static constexpr IO::Pin LED_TWO_PIN = IO::Pin::PA_1;
    static constexpr IO::Pin LED_ONE_PIN = IO::Pin::PA_2;
    /** Self Test Led Pin */
    static constexpr IO::Pin SELF_TEST_LED_PIN = IO::Pin::PC_5;

    /** CAN Pins */

    /** Accessory CAN RX pin */
    static constexpr IO::Pin ACCESSORY_CAN_RX_PIN = IO::Pin::PA_11;
    /** Accessory CAN TX pin */
    static constexpr IO::Pin ACCESSORY_CAN_TX_PIN = IO::Pin::PA_12;
    /** Powertrain CAN RX Pin*/
    static constexpr IO::Pin POWERTRAIN_CAN_RX_PIN = IO::Pin::PB_12;
    /** Powertrain CAN TX Pin*/
    static constexpr IO::Pin POWERTRAIN_CAN_TX_PIN = IO::Pin::PB_13;

    /** Hardware Monitor Pins */

    /** Watchdog pin */
    static constexpr IO::Pin WATCHDOG_PIN = IO::Pin::PA_15;
    /** Microcontroller State Pins */
    static constexpr IO::Pin UC_STATE_ZERO_PIN = IO::Pin::PB_5;
    static constexpr IO::Pin UC_STATE_ONE_PIN = IO::Pin::PB_4;
    static constexpr IO::Pin UC_STATE_TWO_PIN = IO::Pin::PB_15;
    static constexpr IO::Pin UC_STATE_THREE_PIN = IO::Pin::PD_2;
    /** Hardware Monitor Fault Pin */
    static constexpr IO::Pin HM_FAULT_PIN = IO::Pin::PC_8;
    /** Microcontroller Fault Pin */
    static constexpr IO::Pin UC_FAULT_PIN = IO::Pin::PC_9;

    /** Self Test Pins */

    /** CAN Self Test Pin */
    static constexpr IO::Pin CAN_SELF_TEST_PIN = IO::Pin::PB_8;
    /** Ignition Self Test Pin */
    static constexpr IO::Pin IGNITION_SELF_TEST_PIN = IO::Pin::PC_2;
    /** ESTOP Self Test Pin */
    static constexpr IO::Pin ESTOP_SELF_TEST_PIN = IO::Pin::PC_3;
    /** Motor Controller Self Test Pin */
    static constexpr IO::Pin MC_SELF_TEST_PIN = IO::Pin::PC_4;

    /** Motor Controller Pins */

    /** Motor Controller Toggle Pins */
    static constexpr IO::Pin MC_TOGGLE_POSITIVE_PIN = IO::Pin::PC_1;
    static constexpr IO::Pin MC_TOGGLE_NEGATIVE_PIN = IO::Pin::PC_0;
    /** Motor Controller Enable Pins */
    static constexpr IO::Pin MC_ENABLE_POSITIVE_PIN = IO::Pin::PB_6;
    static constexpr IO::Pin MC_ENABLE_NEGATIVE_PIN = IO::Pin::PB_7;
    /** Motor Controller Status Pin */
    static constexpr IO::Pin MC_STATUS_PIN = IO::Pin::PB_9;

    /** UART Pins */

    /** UART TX Pin */
    static constexpr IO::Pin UART_TX = IO::Pin::PC_6;
    /** UART RX Pin */
    static constexpr IO::Pin UART_RX = IO::Pin::PC_7;

    /** Component Communication Pins */

    /** ESTOP Pin */
    static constexpr IO::Pin ESTOP_PIN = IO::Pin::PC_10;
    /** Ignition Pin */
    static constexpr IO::Pin IGNITION_PIN = IO::Pin::PC_11;
    /** LVSS Status Pin */
    static constexpr IO::Pin LVSS_STATUS_PIN = IO::Pin::PC_12;
    /** LVSS Enable Pin */
    static constexpr IO::Pin LVSS_ENABLE_PIN = IO::Pin::PC_13;

    /**
     * Accessory CAN Node IDs
     */

    static constexpr uint8_t IMU_NODE_ID = 9;

    /**
     * Struct that contains all the GPIOs that an instance of this class requires.
     */
    struct ReqGPIO {
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

    /**
     * Constructor for VCU object
     */
    MCUC(ReqGPIO gpios, IO::CAN& ptCAN);

    /**
     * Handles the passed in Powertrain CAN message.
     *
     * @param message message to handle
     */
    void handlePowertrainCanMessage(IO::CANMessage& message);

    /**
     * Returns a pointer to the queue for Powertrain CANopen messages
     *
     * @return pointer to the fixed queue.
     */
    core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage>* getPowertrainQueue();

    /**
     * Runs one step of the Hardmon model, including processing and handling inputs and outputs of the model.
     */
    void process();

    //override methods from CANDevice

    CO_OBJ_T* getObjectDictionary() override;

    uint8_t getNumElements() override;

    uint8_t getNodeID() override;

private:
    /**
     * Local instance of PowertrainCan
     */
    dev::PowertrainCAN powertrainCAN;

    /**
     * The model that is determining our control flow
     * Automatically constructed here (not to be passed in)
     */
    MCuC_Model model;

    ///the gpios
    ReqGPIO gpios;

    //TODO: ask EEs about initial values (i.e. if they should be 0 or whatever)

    //Model input data
    bool brakeOn;                ///< CAN (HIB): Whether or not the brake is on.
    bool eStop;                  ///< GPIO: Whether or not the emergency stop is enabled.
    bool forwardEnable;          ///< CAN (HIB): Whether or not the bike is commanded to go forward.
    bool startPressed;           ///< CAN (HIB): Whether or not the bike is starting.
    uint8_t mcState;             ///< CAN (MC): What state the motor controller state machine is in. [0,14] range
    uint8_t mcDischarge;         ///< CAN (MC): What state the motor controller discharger is in. [0,4] range
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
    bool ucState[4];              ///< GPIO: Current State of the MCUC;
    bool inverterDischarge;       ///< CAN (MC): Whether or not the inverter on the motor controller should be discharging.
    bool mcTogglePositive;        ///< GPIO: Together with mcToggleNegative controls whether the motor controller should be on or not.
    bool mcToggleNegative;        ///< GPIO: Together with mcTogglePositive controls whether the motor controller should be on or not.
    int16_t torqueRequest;        ///< CAN (MC): How much torque the MCUC is requesting the motor controller to output
    bool mcSelfTestOut;           ///< GPIO: Whether or not the motor controller should be self-testing.
    bool estopSelfTestOut;        ///< GPIO: Whether or not the estop should be self-testing.
    bool ignitionSelfTestOut;     ///< GPIO: Whether or not ignition should be self-testing.
    bool accessoryCanSelfTestOut; ///< CAN (Hardmon): Whether a self-test message should be sent to the Hardmon over accessoryCAN
    bool powertrainCanSelfTestOut;///< CAN (Hardmon): Whether a self-test message should be sent to the Hardmon over powertrainCAN

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 255;//TODO: CANopen set node ID

    /**
     * The size of the Object Dictionary
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 51;//TODO: CANopen set size of object dictionary

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

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace vcu

#endif//VCU_HPP
