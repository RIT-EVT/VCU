#ifndef VCU_INCLUDE_TRACKMCUC_HPP
#define VCU_INCLUDE_TRACKMCUC_HPP

#include <core/io/pin.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/CAN.hpp>


namespace io = core::io;

class TrackMCuC {
public:
    /** LED Pins */
    static constexpr io::Pin LED_ONE_PIN   = io::Pin::PC_10;
    static constexpr io::Pin LED_TWO_PIN   = io::Pin::PC_11;
    static constexpr io::Pin LED_THREE_PIN = io::Pin::PC_12;
    /** Self Test Led Pin */
    static constexpr io::Pin FAULT_LED_PIN       = io::Pin::PB_4;
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
    static constexpr io::Pin UC_STATE_ZERO_PIN  = io::Pin::PC_6;
    static constexpr io::Pin UC_STATE_ONE_PIN   = io::Pin::PC_7;
    static constexpr io::Pin UC_STATE_TWO_PIN   = io::Pin::PC_8;
    static constexpr io::Pin UC_STATE_THREE_PIN = io::Pin::PC_9;

    /** Self Test Pins */

    /** CAN Self Test Pin */
    static constexpr io::Pin CAN_SELF_TEST_PIN = io::Pin::PB_14;
    /** Motor Controller Self Test Pin */
    static constexpr io::Pin MC_SELF_TEST_PIN      = io::Pin::PC_3;
    static constexpr io::Pin LS_SELF_TEST_OUT_PIN  = io::Pin::PA_4;
    static constexpr io::Pin LS_SELF_TEST_IN_A_PIN = io::Pin::PA_5;
    static constexpr io::Pin LS_SELF_TEST_IN_B_PIN = io::Pin::PA_6;

    /** Motor Controller Pins */

    /** Motor Controller Toggle Pins */
    static constexpr io::Pin MC_TOGGLE_POSITIVE_PIN = io::Pin::PC_1;
    static constexpr io::Pin MC_TOGGLE_NEGATIVE_PIN = io::Pin::PC_0;
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
    static constexpr io::Pin INTERLOCK_PIN = io::Pin::PA_7;
    /** LVSS Enable Pin */
    static constexpr io::Pin LVSS_ENABLE_PIN = io::Pin::PC_4;

    io::GPIO& greenLed;
    io::GPIO& yellowLed;
    io::GPIO& redLed;
    io::GPIO& faultLed;
    io::GPIO& superFaultLed;

    io::GPIO& canSelfTest;
    io::GPIO& mcSelfTest;

    io::GPIO& mcToggleP;
    io::GPIO& mcToggleN;
    io::GPIO& lvssEnable;

    io::GPIO& estop;
    io::GPIO& ignition;
    io::GPIO& interlock;

    io::CAN& ptCan;

    TrackMCuC(io::GPIO& greenLed,
         io::GPIO& yellowLed,
         io::GPIO& redLed,
         io::GPIO& faultLed,
         io::GPIO& superFaultLed,
         io::GPIO& canSelfTest,
         io::GPIO& mcSelfTest,
         io::GPIO& mcToggleP,
         io::GPIO& mcToggleN,
         io::GPIO& lvssEnable,
         io::GPIO& estop,
         io::GPIO& ignition,
         io::GPIO& interlock,
         io::CAN& ptCan);

    void process();
};

#endif // VCU_INCLUDE_TRACKMCUC_HPP
