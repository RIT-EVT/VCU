#ifndef VCU_HPP
#define VCU_HPP

#include <dev/PowertrainCAN.hpp>
#include <EVT/io/CANDevice.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <EVT/io/GPIO.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/types/CANMessage.hpp>

namespace IO = EVT::core::IO;

namespace VCU {

/**
 * Driver for the VCU device
 */
class MCUC : public CANDevice {
public:

    /**
     * Struct that contains all the GPIOs that an instance of this class requires.
     */
     struct reqGPIO {
         //model input GPIOs
        IO::GPIO& eStopGPIO;
        IO::GPIO& ignitionGPIO;
        IO::GPIO& hmFaultGPIO;
        IO::GPIO& lvssStatusGPIO;
        IO::GPIO& mcStatusGPIO;

        //model output GPIOs
        IO::GPIO& ucFaultGPIO;
        IO::GPIO& lvssEnableGPIO;
        IO::GPIO& watchdogGPIO;

        IO::GPIO& ucStateZeroGPIO;
        IO::GPIO& ucStateOneGPIO;
        IO::GPIO& ucStateTwoGPIO;
        IO::GPIO& ucStateThreeGPIO;

        IO::GPIO& mcToggleNegativeGPIO;
        IO::GPIO& mcTogglePositiveGPIO;
        IO::GPIO& mcSelfTestGPIO;
     };
    /**
     * VCU Pinout
    */

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
     * Constructor for VCU object
     */
     MCUC(reqGPIO gpios);

    /**
     * Get a pointer to the start of the object dictionary
     *
     * @return Pointer to the start of the object dictionary
     */
    CO_OBJ_T* getObjectDictionary() override;

    /**
     * Get the number of elements in the object dictionary.
     *
     * @return The number of elements in the object dictionary
     */
    uint8_t getNumElements() override;

    /**
    * Get the device's node ID
    *
    * @return The node ID of the can device.
     */
    uint8_t getNodeID() override;

    /**
     * Handles the passed in Powertrain CAN message.
     *
     * @param message message to handle
     */
    void handlePowertrainCanMessage(IO::CANMessage& message);

    /**
     * Returns a pointer to the queue for Powertrain CANopen messages
     * @return
     */
    EVT::core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage>* getPowertrainQueue();

    void process();

private:

    /**
     * Local instance of PowertrainCan
     */
     DEV::PowertrainCAN powertrainCAN;

    /**
     * The model that is determining our control flow
     * Automatically constructed here (not to be passed in)
     */
    //MCUC_Model model;

    //Model input data
    bool brakeOn;           ///< CAN (HIB): Whether or not the brake is on.
    bool eStop;             ///< GPIO: Whether or not the emergency stop is enabled.
    bool forwardEnable;     ///< CAN (HIB): Whether or not the bike is commanded to go forward.
    bool startPressed;      ///< CAN (HIB): Whether or not the bike is starting.
    uint8_t mcState;        ///< CAN (MC): What state the motor controller state machine is in. [0,14] range
    uint8_t mcDischarge;    ///< CAN (MC): What state the motor controller discharger is in. [0,4] range
    bool ignitionOn;        ///< GPIO: Whether or not the ignition is on.
    bool hmFault;           ///< GPIO: Whether or not the hardware monitor is telling the MCUC to go into a fault state.
    uint16_t throttle;      ///< CAN (HIB): Signal state of the throttle.
    bool lvssOn;            ///< GPIO: Whether or not the LVSS is on.
    bool mcOn;              ///< GPIO: Whether or not the motor controller is on.

    //Model output data
    bool ucFault;           ///< GPIO: Whether or not the MCUC is in a fault state
    bool inverterEnable;    ///< CAN (MC): Whether or not the inverter on the motor controller should be enabled.
    bool lvssEnable;        ///< GPIO: Whether or not the lvss should be on.
    bool watchdog;          ///< GPIO: watchdog signal between the Hardmon and MCUC.
    bool ucState[4];        ///< GPIO: Current State of the MCUC;
    bool inverterDischarge; ///< CAN (MC): Whether or not the inverter on the motor controller should be discharging.
    bool mcToggleNegative;  ///< GPIO: Together with mcTogglePositive controls whether the motor controller should be on or not.
    bool mcTogglePositive;  ///< GPIO: Together with mcToggleNegative controls whether the motor controller should be on or not.
    uint16_t torqueRequest; ///< CAN (MC): How much torque the MCUC is requesting the motor controller to output.
    bool mcSelfTest;        ///< GPIO: Whether or not the motor controller should be self-testing.

    ///the gpios
    reqGPIO gpios;

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 255; //TODO: CANopen set node ID

    /**
     * The size of the Object Dictionary
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 51; //TODO: CANopen set size of object dictionary

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

}//namespace VCU

#endif//VCU_HPP
