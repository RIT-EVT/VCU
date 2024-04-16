#ifndef VCU_HARDMON_HPP
#define VCU_HARDMON_HPP

#include <dev/PowertrainCAN.hpp>
#include <EVT/io/types/CANMessage.hpp>
#include <EVT/io/CANDevice.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/GPIO.hpp>
#include <models/Hardmon_Model.hpp>

namespace IO = EVT::core::IO;

namespace VCU {

/**
 * Driver for the Hardware Monitor
 */

class Hardmon : public CANDevice {
public:
    /**
     * Struct that contains all the GPIOs that an instance of this class requires.
     */
    struct reqGPIO {
        //model input pins
        IO::GPIO& ignitionCheckGPIO;
        IO::GPIO& ignition3V3GPIO;
        IO::GPIO& lvssStatus3V3GPIO;
        IO::GPIO& mcStatusGPIO;

        IO::GPIO& ucStateZeroGPIO;
        IO::GPIO& ucStateOneGPIO;
        IO::GPIO& ucStateTwoGPIO;
        IO::GPIO& ucStateThreeGPIO;

        IO::GPIO& eStopCheckGPIO;
        IO::GPIO& watchdogGPIO;
        IO::GPIO& eStop3V3GPIO;

        //model outputs pins
        IO::GPIO& lvssEnableOverrideGPIO;
        IO::GPIO& mcToggleNegativeGPIO;
        IO::GPIO& mcTogglePositiveGPIO;
        IO::GPIO& mcToggleOverrideGPIO;
        IO::GPIO& ucResetGPIO;
        IO::GPIO& lvssEnableHardmonGPIO;
        IO::GPIO& hmFaultGPIO;
    };

    /**
     * Hardmon Pinout
     */

        /** UART Pins */

    /** UART TX pin */
    static constexpr IO::Pin UART_TX_PIN = IO::Pin::PA_0;
    /** UART RX pin */
    static constexpr IO::Pin UART_RX_PIN = IO::Pin::PA_1;

    /** LVSS Enable Pin */
    static constexpr IO::Pin LVSS_EN_PIN = IO::Pin::PA_8;
    /** LVSS Enable Override Pin */
    static constexpr IO::Pin LVSS_EN_OVERRIDE_PIN = IO::Pin::PC_9;
    /** LVSS Status Pin */
    static constexpr IO::Pin LVSS_STATUS_3V3_Pin = IO::Pin::PC_11;

    /** MicroController State Pins */
    static constexpr IO::Pin UC_STATE_ZERO_PIN = IO::Pin::PA_9;
    static constexpr IO::Pin UC_STATE_ONE_PIN = IO::Pin::PA_15;
    static constexpr IO::Pin UC_STATE_TWO_PIN = IO::Pin::PC_2;
    static constexpr IO::Pin UC_STATE_THREE_PIN = IO::Pin::PC_10;
    /** Microcontroller Fault Status Pin */
    static constexpr IO::Pin UC_FAULT_PIN = IO::Pin::PC_1;

    /** Motor Controller Status Pin */
    static constexpr IO::Pin MOTOR_CONTROLLER_STATUS_PIN = IO::Pin::PA_10;
    /** Motor Controller Toggle Negative Pin */
    static constexpr IO::Pin MOTOR_CONTROLLER_TOGGLE_NEG_PIN = IO::Pin::PC_6;
    /** Motor Controler Toggle Positive Pin */
    static constexpr IO::Pin MOTOR_CONTROLLER_TOGGLE_POS_PIN = IO::Pin::PC_7;
    /** Motor Controller Toggle Override Pin */
    static constexpr IO::Pin MOTOR_CONTROLLER_TOGGLE_OVERRIDE_PIN = IO::Pin::PC_8;


    /** Accessory CAN RX Pin */
    static constexpr IO::Pin ACCESSORY_CAN_RX_PIN = IO::Pin::PA_11;
    /** Accessory CAN TX Pin */
    static constexpr IO::Pin ACCESSORY_CAN_TX_PIN = IO::Pin::PA_12;

    /** Powertrain Network CAN RX Pin */
    static constexpr IO::Pin POWERTRAIN_CAN_RX_PIN = IO::Pin::PB_12;
    /** Powertrain Network CAN TX Pin */
    static constexpr IO::Pin POWERTRAIN_CAN_TX_PIN = IO::Pin::PB_13;

    /** Watchdog Pin */
    static constexpr IO::Pin WATCHDOG_PIN = IO::Pin::PB_4;
    /** Hardmon Fault Pin */
    static constexpr IO::Pin HM_FAULT_PIN = IO::Pin::PB_5;

    /** Ignition "12v" Pin */
    static constexpr IO::Pin IGNITION_CHECK_PIN = IO::Pin::PB_6;
    /** Ignition 3V3 Pin */
    static constexpr IO::Pin IGNITION_3V3_PIN = IO::Pin::PC_12;

    /** Estop "12v" Pin */
    static constexpr IO::Pin ESTOP_CHECK_PIN = IO::Pin::PB_7;
    /** Estop 3V3 Pin */
    static constexpr IO::Pin ESTOP_3V3_PIN = IO::Pin::PD_2;

    /** MicroController Reset Pin */
    static constexpr IO::Pin UC_RESET_PIN = IO::Pin::PB_8;
    /** CAN Selftest Pin */
    static constexpr IO::Pin CAN_SELFTEST_PIN = IO::Pin::PB_9;
    /** MCUC CAN Override Pin */
    static constexpr IO::Pin CAN_OVERRIDE_PIN = IO::Pin::PC_13;

    /**
     * Constructor for Hardmon object
     */
    Hardmon(reqGPIO gpios);

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
     * Handles the passed in Powertrain CAN message.
     *
     * @param message message to handle
     */
    void handlePowertrainCanMessage(IO::CANMessage& message);

    /**
    * Get the device's node ID
    *
    * @return The node ID of the can device.
     */
    uint8_t getNodeID() override;

    /**
     * Returns a pointer to the queue for Powertrain CANopen messages
     * @return
     */
    EVT::core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage>* getPowertrainQueue();

    void process();
private:

    /**
     * Local instance of PowertrainCan (handles PowertrainCAN messages
     */
    DEV::PowertrainCAN powertrainCAN;

    /**
     * The model that is determining our control flow
     * Automatically constructed here (not to be passed in)
     */
    Hardmon_Model model;

    //Model input data

     bool forwardEnable;       ///< handlebar forward enable
     bool ignitionCheck;       ///< whether the ignition is on or off on 12v line
     bool ignition3v3;         ///< whether the ignition is on or off on 3.3v line
     bool lvssStatus;          ///< whether the lvss is on or not
     bool mcStatus;            ///< whether the Motor Controller is on or off
     bool ucState[4];          ///< what state the microcontroller is in
     bool eStopCheck;          ///< whether the estop is on or off on 12v line
     uint8_t discharge;        ///< current state of the discharge state machine
     bool watchdog;            ///< alternating on and off signal from the MCUC to the Hardmon
     bool eStop3v3;            ///< whether the estop is on or off on 3.3v line
     bool lvssEnableUC;        ///< whether or not the MCUC is telling the LVSS to be enabled

     //Model output data

     bool mcSwitchEnable;      ///< whether or not the MCUC Motor Controller Enable is disabled
     bool lvssSwitchEnable;    ///< whether or not the MCUC LVSS Enable is disabled
     bool inverterDischarge;   ///< whether or not the Motor Controller is commanded to discharge (sent over CAN)
     bool mcToggleNeg;         ///< Together with MCTogglePos commands the Motor Controller being on or not
     bool mcTogglePos;         ///< Together with MCToggleNeg commands the Motor Controller being on or not
     bool ucReset;             ///< Whether or not the Hardmon is commanding the MCUC to reset (0 = reset)
     bool lvssEnableHardMon;   ///< Whether or not the Hardmon is commanding the LVSS to be enabled
     bool hmFault;             ///< Whether or not the Hardmon is commanding the MCUC to go into a fault state

     ///The gpios
     reqGPIO gpios;

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 254; //TODO set node ID

    /**
     * The size of the Object Dictionary
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 51; //TODO set size of object dictionary

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

}

#endif//VCU_HARDMON_HPP
