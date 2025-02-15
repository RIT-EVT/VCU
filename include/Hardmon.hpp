#ifndef VCU_HARDMON_HPP
#define VCU_HARDMON_HPP

#include <PowertrainCAN.hpp>
#include <core/io/CAN.hpp>
#include <core/io/CANDevice.hpp>
#include <core/io/CANOpenMacros.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/utils/types/FixedQueue.hpp>
#include <models/Hardmon_Model.hpp>

namespace io = core::io;

namespace vcu {

/**
 * Driver for the Hardware Monitor
 */
class Hardmon : public CANDevice {
public:
    //////////////////////////////////////////////
    ///           HardMon Pinout               ///
    //////////////////////////////////////////////

    /** UART Pins */

    /** UART TX pin */
    static constexpr io::Pin UART_TX_PIN = io::Pin::PA_0;
    /** UART RX pin */
    static constexpr io::Pin UART_RX_PIN = io::Pin::PA_1;

    /** LVSS Enable Pin */
    static constexpr io::Pin LVSS_EN_PIN = io::Pin::PA_8;
    /** LVSS Enable Override Pin */
    static constexpr io::Pin LVSS_EN_OVERRIDE_PIN = io::Pin::PC_9;
    /** LVSS Status Pin */
    static constexpr io::Pin LVSS_STATUS_3V3_Pin = io::Pin::PC_11;

    /** MicroController State Pins */
    static constexpr io::Pin UC_STATE_ZERO_PIN = io::Pin::PA_9;
    static constexpr io::Pin UC_STATE_ONE_PIN = io::Pin::PA_15;
    static constexpr io::Pin UC_STATE_TWO_PIN = io::Pin::PC_2;
    static constexpr io::Pin UC_STATE_THREE_PIN = io::Pin::PC_10;
    /** Microcontroller Fault Status Pin */
    static constexpr io::Pin UC_FAULT_PIN = io::Pin::PC_1;

    /** Motor Controller Status Pin */
    static constexpr io::Pin MOTOR_CONTROLLER_STATUS_PIN = io::Pin::PA_10;
    /** Motor Controller Toggle Negative Pin */
    static constexpr io::Pin MOTOR_CONTROLLER_TOGGLE_NEG_PIN = io::Pin::PC_6;
    /** Motor Controller Toggle Positive Pin */
    static constexpr io::Pin MOTOR_CONTROLLER_TOGGLE_POS_PIN = io::Pin::PC_7;
    /** Motor Controller Toggle Override Pin */
    static constexpr io::Pin MOTOR_CONTROLLER_TOGGLE_OVERRIDE_PIN = io::Pin::PC_8;

    /** Accessory CAN RX Pin */
    static constexpr io::Pin ACCESSORY_CAN_RX_PIN = io::Pin::PA_11;
    /** Accessory CAN TX Pin */
    static constexpr io::Pin ACCESSORY_CAN_TX_PIN = io::Pin::PA_12;

    /** Powertrain Network CAN RX Pin */
    static constexpr io::Pin POWERTRAIN_CAN_RX_PIN = io::Pin::PB_12;
    /** Powertrain Network CAN TX Pin */
    static constexpr io::Pin POWERTRAIN_CAN_TX_PIN = io::Pin::PB_13;

    /** Watchdog Pin */
    static constexpr io::Pin WATCHDOG_PIN = io::Pin::PB_4;
    /** Hardmon Fault Pin */
    static constexpr io::Pin HM_FAULT_PIN = io::Pin::PB_5;

    /** Ignition "12v" Pin */
    static constexpr io::Pin IGNITION_CHECK_PIN = io::Pin::PB_6;
    /** Ignition 3V3 Pin */
    static constexpr io::Pin IGNITION_3V3_PIN = io::Pin::PC_12;

    /** Estop "12v" Pin */
    static constexpr io::Pin ESTOP_CHECK_PIN = io::Pin::PB_7;
    /** Estop 3V3 Pin */
    static constexpr io::Pin ESTOP_3V3_PIN = io::Pin::PD_2;

    /** MicroController Reset Pin */
    static constexpr io::Pin UC_RESET_PIN = io::Pin::PB_8;
    /** CAN Selftest Pin */
    static constexpr io::Pin CAN_SELFTEST_PIN = io::Pin::PB_9;
    /** MCUC CAN Override Pin */
    static constexpr io::Pin CAN_OVERRIDE_PIN = io::Pin::PC_13;

    /**
     * Union that primarily contains a struct that contains all the GPIOs that an instance of this class requires.
     * THE ORDER OF THIS MUST MATCH THE MODEL GPIO INPUTS.
    */
    union HardmonGPIO {
        // allows specific named access to outputs and inputs
        struct {
            //model input pins
            io::GPIO& ignitionCheckGPIO;
            io::GPIO& ignition3V3GPIO;
            io::GPIO& lvssStatus3V3GPIO;
            io::GPIO& mcStatusGPIO;

            io::GPIO& ucStateZeroGPIO;
            io::GPIO& ucStateOneGPIO;
            io::GPIO& ucStateTwoGPIO;
            io::GPIO& ucStateThreeGPIO;

            io::GPIO& eStopCheckGPIO;
            io::GPIO& watchdogGPIO;
            io::GPIO& eStop3V3GPIO;

            //model outputs pins

            io::GPIO& mcToggleOverrideGPIO;
            io::GPIO& lvssEnableOverrideGPIO;
            io::GPIO& mcToggleNegativeGPIO;
            io::GPIO& mcTogglePositiveGPIO;
            io::GPIO& ucResetGPIO;
            io::GPIO& lvssEnableHardmonGPIO;
            io::GPIO& hmFaultGPIO;
        };
        // allows iteration through outputs and inputs
        struct {
            IO::GPIO* inputArr[11];
            IO::GPIO* outputArr[7];
        };
    };

    /**
     * Constructor for Hardmon object
     */
    Hardmon(HardmonGPIO gpios, io::CAN& ptCAN);

    /**
     * Handles the passed in Powertrain CAN message.
     *
     * @param message message to handle
     */
    void handlePowertrainCanMessage(io::CANMessage& message);

    /**
     * Returns a pointer to the queue for Powertrain CANopen messages
     * @return the pointer to the queue
     */
    core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, io::CANMessage>* getPowertrainQueue();

    /**
     * Runs one step of the McUc model, including processing and handling inputs and outputs of the model.
     */
    void process();

    //override methods from CANDevice

    CO_OBJ_T* getObjectDictionary() override;

    uint8_t getNumElements() override;

    uint8_t getNodeID() override;

private:
    /**
     * Struct that contains all the GPIOs that an instance of this class requires.
     * THE ORDER OF THIS MUST MATCH THE HARDMON GPIO ORDER.
    */
    union ModelGPIOInputs_t {
        struct {
            bool ignitionCheck;///< GPIO: whether the ignition is on or off on 12v line
            bool ignition3v3;  ///< GPIO: whether the ignition is on or off on 3.3v line
            bool lvssStatus;   ///< GPIO: whether the lvss is on or not
            bool mcStatus;     ///< GPIO: whether the Motor Controller is on or off
            bool ucState[4];   ///< GPIO: what state the MCuC is in
            bool eStopCheck;   ///< GPIO: whether the estop is on or off on 12v line
            bool watchdog;     ///< GPIO: alternating on and off signal from the MCUC to the Hardmon
            bool eStop3v3;     ///< GPIO: whether the estop is on or off on 3.3v line
        };
        bool arr[11];
    };

    /**
     * Union that lets us rename the Hardmon outputs into better names.
     */
    union ModelOutputs_t {
        struct {
            bool mcSwitchEnable;   ///< GPIO: whether or not the Hardmon is taking over mcEnable control from the MCUC
            bool lvssSwitchEnable; ///< GPIO: whether or not the Hardmon is taking over lvssEnable control from the MCUC
            bool inverterDischarge;///< CAN (MC): whether or not the Motor Controller is commanded to discharge
            bool mcToggleNeg;      ///< GPIO: Together with MCTogglePos commands the Motor Controller being on or not
            bool mcTogglePos;      ///< GPIO: Together with MCToggleNeg commands the Motor Controller being on or not
            bool ucReset;          ///< GPIO: Whether or not the Hardmon is commanding the MCUC to reset (0 = reset)
            bool lvssEnableHardMon;///< GPIO: Whether or not the Hardmon is commanding the LVSS to be enabled
            bool hmFault;          ///< GPIO: Whether or not the Hardmon is commanding the MCUC to go into a fault state
        };
        Hardmon_Model::ExtY_Hardmon_Model_T modelOutputStruct;
    };

    /**
     * Union that holds the gpio struct and an array
     * Lets us index into it like an array
     */

    bool lvssEnableUC;///< GPIO: whether or not the MCuC is telling the LVSS to be enabled

    /**
     * Local instance of PowertrainCan (handles PowertrainCAN messages)
     */
    dev::PowertrainCAN powertrainCAN;

    /**
     * The model that is determining our control flow
     * Automatically constructed here (not to be passed in)
     */
    Hardmon_Model model;

    ///The gpios (in a union)
    HardmonGPIO gpios;

    //TODO: ask EEs about initial values (i.e. if they should be 0 or whatever)

    //Model input data
    ModelGPIOInputs_t modelGPIOInputs;
    uint8_t discharge; ///< CAN (MC): current state of the Motor Controller's discharge state machine
    bool forwardEnable;///< CAN (HIB): handlebar forward enable

    //Model output data
    ModelOutputs_t modelOutputs;

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 254;//TODO set node ID

    /**
     * The size of the Object Dictionary
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 51;//TODO set size of object dictionary

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

}// namespace vcu

#endif//VCU_HARDMON_HPP
