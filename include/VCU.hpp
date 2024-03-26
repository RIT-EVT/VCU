#ifndef VCU_HPP
#define VCU_HPP

#include <EVT/io/pin.hpp>
#include <EVT/io/CANDevice.hpp>
#include <EVT/io/CANOpenMacros.hpp>

namespace IO = EVT::core::IO;

namespace VCU {

/**
 * Driver for the VCU device
 */
class VCU : public CANDevice {
public:

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
    static constexpr IO::Pin CAN_A_RX_PIN = IO::Pin::PA_11;
    /** Accessory CAN TX pin */
    static constexpr IO::Pin CAN_A_TX_PIN = IO::Pin::PA_12;
    /** Motor Controller CAN RX Pin*/
    static constexpr IO::Pin CAN_MC_RX_PIN = IO::Pin::PB_12;
    /** Motor Controller CAN TX Pin*/
    static constexpr IO::Pin CAN_MC_TX_PIN = IO::Pin::PB_13;

        /** Hardware Monitor Pins */

    /** Watchdog pin */
    static constexpr IO::Pin WATCHDOG_PIN = IO::Pin::PA_15;
    /** Microcontroller State Pins */
    static constexpr IO::Pin CONTROLLER_STATE_ZERO_PIN = IO::Pin::PB_5;
    static constexpr IO::Pin CONTROLLER_STATE_ONE_PIN = IO::Pin::PB_4;
    static constexpr IO::Pin CONTROLLER_STATE_TWO_PIN = IO::Pin::PB_15;
    static constexpr IO::Pin CONTROLLER_STATE_THREE_PIN = IO::Pin::PD_2;
    /** Hardware Monitor Fault Pin */
    static constexpr IO::Pin HARDWARE_MONITOR_FAULT_PIN = IO::Pin::PC_8;
    /** Microcontroller Fault Pin */
    static constexpr IO::Pin MICROCONTROLLER_FAULT_PIN = IO::Pin::PC_9;

        /** Self Test Pins */

    /** CAN Self Test Pin */
    static constexpr IO::Pin CAN_SELF_TEST_PIN = IO::Pin::PB_8;
    /** Ignition Self Test Pin */
    static constexpr IO::Pin IGNITION_SELF_TEST_PIN = IO::Pin::PC_2;
    /** ESTOP Self Test Pin */
    static constexpr IO::Pin ESTOP_SELF_TEST_PIN = IO::Pin::PC_3;
    /** Motor Controller Self Test Pin */
    static constexpr IO::Pin MOTOR_CONTROLLER_SELF_TEST_PIN = IO::Pin::PC_4;

        /** Motor Controller Pins */

    /** Motor Controller Toggle Pins */
    static constexpr IO::Pin MOTOR_CONTROLLER_TOGGLE_POSITIVE_PIN = IO::Pin::PC_1;
    static constexpr IO::Pin MOTOR_CONTROLLER_TOGGLE_NEGATIVE_PIN = IO::Pin::PC_0;
    /** Motor Controller Enable Pins */
    static constexpr IO::Pin MOTOR_CONTROLLER_ENABLE_POSITIVE_PIN = IO::Pin::PB_6;
    static constexpr IO::Pin MOTOR_CONTROLLER_ENABLE_NEGATIVE_PIN = IO::Pin::PB_7;
    /** Motor Controller Status Pin */
    static constexpr IO::Pin MOTOR_CONTROLLER_STATUS_PIN = IO::Pin::PB_9;

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
     * Constructor for VCU object
     */
     VCU();

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

    void process();

private:

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 0; //TODO set node ID

    /**
     * The size of the Object Dictionary
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 12; //TODO set size of object dictionary
    //(all the mandatory macros are in total 12 entries)

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

        // End of dictionary marker
        CO_OBJ_DICT_ENDMARK,
    };
};

}//namespace VCU

#endif//VCU_HPP
