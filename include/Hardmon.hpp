#ifndef VCU_HARDMON_HPP
#define VCU_HARDMON_HPP

#include <core/io/CAN.hpp>
#include <core/io/CANDevice.hpp>
#include <core/io/CANOpenMacros.hpp>
#include <core/io/GPIO.hpp>
#include <core/io/pin.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/rtos/Initializable.hpp>
#include <core/rtos/Mutex.hpp>
#include <core/utils/types/FixedQueue.hpp>

#include <PowertrainCAN.hpp>
#include <models/Hardmon_Model.hpp>

namespace io   = core::io;
namespace rtos = core::rtos;

namespace vcu {

/**
 * TODO: HARDMON HAS NOT BEEN DONE YET. Waiting for EE's to make a model...
 *  Will go through n copy n paste the shared stuff soon...
 *
 * The Hardmon (Hardware Monitor) is one of two microcontrollers on the VCU board.
It monitors the MCuC (Motor Controller Microcontroller) in order to determine whether or not it is operating safely and
logically.
 * If it determines that the MCuC is not operating correctly, it can override the MCuC's control over the CAN lines
 * and reset the MCuC in order to hopefully fix the issue.
 *
 * The Hardmon also primarily feeds input data into the Hardmon Model, which is a Simulink model compiled to C++ code.
 * It is designed to be run as one thread of a process, and thus has built in mutual exclusion properties.
 */
class Hardmon : public CANDevice, public rtos::Initializable {
public:
    //////////////////////////////////////////////
    ///           HardMon Pinout               ///
    //////////////////////////////////////////////

    /** Interlock 3v3 Pin */
    static constexpr io::Pin INTERLOCK_PIN = io::Pin::PA_7;

    /** nRST (MCuC reset) Pin */
    static constexpr io::Pin N_RST_PIN = io::Pin::PA_10;

    /** UART TX pin */
    static constexpr io::Pin UART_TX_PIN = io::Pin::PA_8;
    /** UART RX pin */
    static constexpr io::Pin UART_RX_PIN = io::Pin::PA_9;

    /** LVSS Enable Pin */
    static constexpr io::Pin LVSS_EN_PIN = io::Pin::PC_4;
    /** LVSS Enable Override Pin */
    static constexpr io::Pin LVSS_EN_OVERRIDE_PIN = io::Pin::PC_5;

    /** Level Shifter (LS) Self Test In A Pin */
    static constexpr io::Pin LS_SELF_TEST_A_PIN = io::Pin::PA_5;
    /** Level Shifter (LS) Self Test In B Pin */
    static constexpr io::Pin LS_SELF_TEST_B_PIN = io::Pin::PA_6;

    /** MicroController State Pins */
    static constexpr io::Pin UC_STATE_ZERO_PIN  = io::Pin::PC_6;
    static constexpr io::Pin UC_STATE_ONE_PIN   = io::Pin::PC_7;
    static constexpr io::Pin UC_STATE_TWO_PIN   = io::Pin::PC_8;
    static constexpr io::Pin UC_STATE_THREE_PIN = io::Pin::PC_9;

    /** Motor Controller Enable Feedback Pin */
    static constexpr io::Pin MOTOR_CONTROLLER_STATUS_PIN = io::Pin::PC_2;
    /** Motor Controller Toggle Negative Pin */
    static constexpr io::Pin MOTOR_CONTROLLER_TOGGLE_NEG_PIN = io::Pin::PC_0;
    /** Motor Controller Toggle Positive Pin */
    static constexpr io::Pin MOTOR_CONTROLLER_TOGGLE_POS_PIN = io::Pin::PC_1;
    /** Motor Controller Toggle Override Pin */
    static constexpr io::Pin MOTOR_CONTROLLER_TOGGLE_OVERRIDE_PIN = io::Pin::PC_3;

    /** CAN A RX Pin */
    static constexpr io::Pin ACCESSORY_CAN_RX_PIN = io::Pin::PA_11;
    /** CAN A TX Pin */
    static constexpr io::Pin ACCESSORY_CAN_TX_PIN = io::Pin::PA_12;

    /** Powertrain Network (with MC) CAN RX Pin */
    static constexpr io::Pin POWERTRAIN_CAN_RX_PIN = io::Pin::PB_12;
    /** Powertrain Network (with MC) CAN TX Pin */
    static constexpr io::Pin POWERTRAIN_CAN_TX_PIN = io::Pin::PB_13;

    /** Watchdog Pin */
    static constexpr io::Pin WATCHDOG_PIN = io::Pin::PA_15;

    /** Ignition Level Shifter (LS) A Pin */
    static constexpr io::Pin IGNITION_A_PIN = io::Pin::PA_2;
    /** Ignition Level Shifter (LS) B Pin */
    static constexpr io::Pin IGNITION_B_PIN = io::Pin::PA_3;

    /** Estop from Level Shifter (LS) A Pin */
    static constexpr io::Pin ESTOP_A_PIN = io::Pin::PA_0;
    /** Estop from Level Shifter (LS) B Pin */
    static constexpr io::Pin ESTOP_B_PIN = io::Pin::PA_1;

    /** CAN Selftest Pin */
    static constexpr io::Pin CAN_SELFTEST_PIN = io::Pin::PB_14;
    /** MCUC CAN Override Pin */
    static constexpr io::Pin CAN_OVERRIDE_PIN = io::Pin::PB_15;

    /**
     * Union that primarily contains a struct that contains all the GPIOs that an instance of this class requires.
     * THE ORDER OF THIS MUST MATCH THE MODEL GPIO INPUTS.
     */
    union HardmonGPIO {
        // allows specific named access to outputs and inputs
        struct {
            // model input pins
            io::GPIO& eStopAGPIO;
            io::GPIO& eStopBGPIO;

            io::GPIO& ignitionAGPIO;
            io::GPIO& ignitionBGPIO;

            io::GPIO& ucStateZeroGPIO;
            io::GPIO& ucStateOneGPIO;
            io::GPIO& ucStateTwoGPIO;
            io::GPIO& ucStateThreeGPIO;

            io::GPIO& mcStatusGPIO;
            io::GPIO& interlockGPIO;

            io::GPIO& watchdogGPIO;

            io::GPIO& lsSelfTestAGPIO;
            io::GPIO& lsSelfTestBGPIO;

            io::GPIO& canSelfTestGPIO;

            // model outputs pins
            io::GPIO& mcToggleNegativeGPIO;
            io::GPIO& mcTogglePositiveGPIO;
            io::GPIO& mcToggleOverrideGPIO;

            io::GPIO& lvssEnableOverrideGPIO;
            io::GPIO& lvssEnableHardmonGPIO;

            io::GPIO& mcucResetGPIO;
            io::GPIO& canOverrideGPIO;
        };
        // allows iteration through outputs and inputs
        struct {
            io::GPIO* inputArr[14];
            io::GPIO* outputArr[7];
        };
    };

    /**
     * Union that contains all the data that AccessoryCan should read in.
     * Used for double buffering for threadsafety with CANOpen
     */
    typedef union {
        // named signals
        struct {
            uint16_t LVSS_out_EnableBoardSignal;        ///< LVSS (out) Determines which boards it will send power to
            uint16_t LVSS_in_HVCurrent[2];              ///< LVSS (in)
            uint16_t LVSS_in_PowerSwitchCurrents[4];    ///< LVSS (in)
            uint16_t LVSS_in_Temperatures[2];           ///< LVSS (in)
            uint16_t LVSS_in_PowerSwitchErrorStatus[3]; ///< LVSS (in)
        };
        // signal groups (for memcpy use)
        struct {
            uint16_t outputs[1];
            uint16_t inputs[11];
        };
    } AccessoryCanData_t;

    /**
     * Constructor for Hardmon object
     */
    Hardmon(HardmonGPIO gpios, io::CAN& ptCAN);

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

private:
    /**
     * Mutex that protects internal access to the Hardmon
     */
    rtos::Mutex mutex;

    /**
     * Safe (mutexed) Buffer Data that comes in or is sent out over Accessory CAN.
     */
    AccessoryCanData_t accessoryCanDataSafeBuffer;

    /**
     * Struct that contains all the GPIOs that an instance of this class requires.
     * THE ORDER OF THIS MUST MATCH THE HARDMON GPIO ORDER.
     */
    union ModelGPIOInputs_t {
        struct {
            bool eStopA;        ///< GPIO: whether the estop is on or off on A
            bool eStopB;        ///< GPIO: whether the estop is on or off on B
            bool ignitionA;     ///< GPIO: whether the ignition is on or off A
            bool ignitionB;     ///< GPIO: whether the ignition is on or off B
            bool ucState[4];    ///< GPIO: what state the MCuC is in
            bool mcStatus;      ///< GPIO: whether the Motor Controller is on or off
            bool interlock;     ///< GPIO: whether the interlock is on or off
            bool watchdog;      ///< GPIO: alternating on and off signal from the MCUC to the Hardmon
            bool lvssStatus;    ///< GPIO: whether the lvss is on or not
        };
        bool arr[12];
    };

    /**
     * Union that lets us rename the Hardmon outputs into better names.
     */
    union ModelOutputs_t {
        struct {
            bool mcSwitchEnable;   ///< GPIO: whether or not the Hardmon is taking over mcEnable control from the MCUC
            bool lvssSwitchEnable; ///< GPIO: whether or not the Hardmon is taking over lvssEnable control from the MCUC
            bool inverterDischarge; ///< CAN (MC): whether or not the Motor Controller is commanded to discharge
            bool mcToggleNeg;       ///< GPIO: Together with MCTogglePos commands the Motor Controller being on or not
            bool mcTogglePos;       ///< GPIO: Together with MCToggleNeg commands the Motor Controller being on or not
            bool ucReset;           ///< GPIO: Whether or not the Hardmon is commanding the MCUC to reset (0 = reset)
            bool lvssEnableHardMon; ///< GPIO: Whether or not the Hardmon is commanding the LVSS to be enabled
            bool hmFault;           ///< GPIO: Whether or not the Hardmon is commanding the MCUC to go into a fault state
        };
        Hardmon_Model::ExtY_Hardmon_T modelOutputStruct;
    };

    /**
     * Local instance of PowertrainCan (handles PowertrainCAN messages)
     */
    dev::PowertrainCAN powertrainCAN;

    /**
     * The model that is determining our control flow
     * Automatically constructed here (not to be passed in)
     */
    Hardmon_Model model;

    /// The gpios (in a union)
    HardmonGPIO gpios;

    // Model gpio input data
    ModelGPIOInputs_t modelGPIOInputs;

    /**
     * Union that holds the gpio struct and an array
     * Lets us index into it like an array
     */
    UC_State ucState;
    bool lvssEnableUC; ///< GPIO: whether or not the MCuC is telling the LVSS to be enabled (calculated from ucState)
    MC_DC_State discharge; ///< CAN (MC): current state of the Motor Controller's discharge state machine
    bool forwardEnable;    ///< CAN (HIB): handlebar forward enable

    // Model output data
    ModelOutputs_t modelOutputs;

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 0;

    /**
     * The size of the Object Dictionary
     */
    static constexpr uint8_t OBJECT_DICTIONARY_SIZE = 51; // TODO set size of object dictionary

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

} // namespace vcu

#endif // VCU_HARDMON_HPP
