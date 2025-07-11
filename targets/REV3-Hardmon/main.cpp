/**
 * This is the main target for the Hardmon (the processor that monitors the MCUC).
 * It takes in CANopen messages and other inputs from the MCUC and accessory boards,
 * and checks if the MCUC is functioning correctly.
 */

// CAN Interrupt Addons (needed so that CAN interrupts can do ThreadX Requests)
#define CAN_IRQ_PRE_INTERRUPT_ROUTINE _tx_thread_context_save()

#define CAN_IRQ_POST_INTERRUPT_ROUTINE _tx_thread_context_restore()

#include <core/io/CAN.hpp>
#include <core/io/UART.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/manager.hpp>
#include <core/utils/log.hpp>

#include <core/io/CANopen.hpp>

#include <co_core.h>
#include <co_if.h>
#include <co_tmr.h>

// rtos includes
#include <core/rtos/BytePool.hpp>
#include <core/rtos/Enums.hpp>
#include <core/rtos/EventFlags.hpp>
#include <core/rtos/Queue.hpp>
#include <core/rtos/Semaphore.hpp>
#include <core/rtos/Thread.hpp>
#include <core/rtos/Timer.hpp>
#include <core/rtos/tsio/ThreadUART.hpp>

#include <Hardmon.hpp>

namespace rtos = core::rtos;
namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;
namespace log  = core::log;

///////////////////////////////////////////////////////////////////////////////
// RTOS GLOBAlS SETUP
///////////////////////////////////////////////////////////////////////////////

/// The size of the memory pool for the tx application
#define TX_APP_MEM_POOL_SIZE 65536
/// How often the model should take 1 step.
#define MODEL_THREAD_TRIGGER_RATE S_TO_TICKS(5)

// Model Thread Parameters
#define MODEL_THREAD_STACK_SIZE        1024
#define MODEL_THREAD_PRIORITY          1
#define MODEL_THREAD_PREEMPT_THRESHOLD 1
#define MODEL_THREAD_TIME_SLICE        MS_TO_TICKS(20)
#define MODEL_THREAD_AUTOSTART         true

// Powertrain CAN Receive Thread Parameters
#define PT_CAN_RECEIVE_STACK_SIZE        1024
#define PT_CAN_RECEIVE_PRIORITY          3
#define PT_CAN_RECEIVE_PREEMPT_THRESHOLD 3
#define PT_CAN_RECEIVE_TIME_SLICE        MS_TO_TICKS(10)
#define PT_CAN_RECEIVE_AUTOSTART         true

// Health Thread Parameters
#define HEALTH_THREAD_STACK_SIZE        1024
#define HEALTH_THREAD_PRIORITY          5
#define HEALTH_THREAD_PREEMPT_THRESHOLD 5
#define HEALTH_THREAD_TIME_SLICE        MS_TO_TICKS(10)
#define HEALTH_THREAD_AUTOSTART         true

// Accessory CAN Receive Thread Parameters
#define ACC_CAN_RECEIVE_THREAD_STACK_SIZE        1024
#define ACC_CAN_RECEIVE_THREAD_PRIORITY          4
#define ACC_CAN_RECEIVE_THREAD_PREEMPT_THRESHOLD 4
#define ACC_CAN_RECEIVE_THREAD_TIME_SLICE        MS_TO_TICKS(10)
#define ACC_CAN_RECEIVE_THREAD_AUTOSTART         true

// Thread Structs

/**
 * Struct that holds information needed for the model thread
 */
typedef struct {
    vcu::Hardmon* hardmon;
    rtos::EventFlags* triggerFlag;
} modelThreadArgs_t;

/**
 * Struct that holds information needed for the powertrain CAN thread
 */
typedef struct {
    vcu::Hardmon* hardmon;
} powertrainCANReceiveThreadArgs_t;

/**
 * Struct that holds information needed for the health thread
 */
typedef struct {
    vcu::Hardmon* hardmon;
} healthThreadArgs_t;

/**
 * Struct that holds information needed for the accessory CANopen Thread
 */
typedef struct {
    vcu::Hardmon* hardmon;
    CO_NODE* accessoryCanNode;
} accessoryCanReceiveThreadArgs_t;

// Timer expiration function
void modelTimerExpiration(rtos::EventFlags* modelTriggerFlag);

// Thread Function Prototypes-- implementation below main.
[[noreturn]] void modelThreadEntry(modelThreadArgs_t* args);
[[noreturn]] void powertrainCANReceiveThreadEntry(powertrainCANReceiveThreadArgs_t* args);
[[noreturn]] void healthThreadEntry(healthThreadArgs_t* args);
[[noreturn]] void accessoryCanReceiveThreadEntry(accessoryCanReceiveThreadArgs_t* args);

///////////////////////////////////////////////////////////////////////////////
// EVT-core CAN callback and CAN setup. This will include logic to set
// aside CANopen messages into a specific queue
///////////////////////////////////////////////////////////////////////////////

/**
 * Interrupt handler to get CAN messages. A function pointer to this function
 * will be passed to the EVT-core CAN interface which will in turn call this
 * function each time a new CAN message comes in.
 *
 * NOTE: For this sample, every non-extended (so 11 bit CAN IDs) will be
 * assumed to be intended to be passed as a CANopen message.
 *
 * @param message[in] The passed in CAN message that was read.
 * @param priv[in] the passed in Queue to add the message to. Must be a FixedQueue*.
 */
void canOpenInterrupt(io::CANMessage& message, void* priv) {
    auto* queue = (core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage>*) priv;
    if (queue != nullptr) {
        // TODO: determine if WaitForever is what we want to do in the interrupt- could be bad
        queue->append(message);
    }
}

/**
 * Interrupt handler to get CAN messages from the powertrain CAN line.
 * @param message[in] the passed in CAN message that was read.
 * @param priv[in] the Hardmon instance that will handle the message.
 */
void powertrainCANInterrupt(io::CANMessage& message, void* priv) {
    auto* hardmon = (vcu::Hardmon*) priv;
    if (hardmon != nullptr) {
        // TODO: determine if WaitForever is what we want to do in the interrupt- could be bad
        hardmon->sendToPowertrainQueue(&message, rtos::TXWait::TXW_WAIT_FOREVER);
    }
}

int main() {

    // Initialize system
    core::platform::init();

    // Initialize the timer
    dev::Timer& timer = dev::getTimer<dev::MCUTimer::Timer2>(100);

    // UART for testing
    io::UART& uart = io::getUART<vcu::Hardmon::UART_TX_PIN, vcu::Hardmon::UART_RX_PIN>(9600);
    rtos::tsio::ThreadUART threadUART(uart);

    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "MCuC Debug Logger Started.");

    //////////////////////////////
    // Initialize Powertrain CAN //
    //////////////////////////////
    vcu::Hardmon::HardmonGPIO hmGPIOS = {
        {io::getGPIO<vcu::Hardmon::IGNITION_CHECK_PIN>(io::GPIO::Direction::INPUT),
         io::getGPIO<vcu::Hardmon::IGNITION_3V3_PIN>(io::GPIO::Direction::INPUT),
         io::getGPIO<vcu::Hardmon::LVSS_STATUS_3V3_Pin>(io::GPIO::Direction::INPUT),
         io::getGPIO<vcu::Hardmon::MOTOR_CONTROLLER_STATUS_PIN>(io::GPIO::Direction::INPUT),

         io::getGPIO<vcu::Hardmon::UC_STATE_ONE_PIN>(io::GPIO::Direction::INPUT),
         io::getGPIO<vcu::Hardmon::UC_STATE_ONE_PIN>(io::GPIO::Direction::INPUT),
         io::getGPIO<vcu::Hardmon::UC_STATE_TWO_PIN>(io::GPIO::Direction::INPUT),
         io::getGPIO<vcu::Hardmon::UC_STATE_THREE_PIN>(io::GPIO::Direction::INPUT),

         io::getGPIO<vcu::Hardmon::ESTOP_CHECK_PIN>(io::GPIO::Direction::INPUT),
         io::getGPIO<vcu::Hardmon::WATCHDOG_PIN>(io::GPIO::Direction::INPUT),
         io::getGPIO<vcu::Hardmon::ESTOP_3V3_PIN>(io::GPIO::Direction::INPUT),

         io::getGPIO<vcu::Hardmon::MOTOR_CONTROLLER_TOGGLE_OVERRIDE_PIN>(io::GPIO::Direction::OUTPUT),
         io::getGPIO<vcu::Hardmon::LVSS_EN_OVERRIDE_PIN>(),
         io::getGPIO<vcu::Hardmon::MOTOR_CONTROLLER_TOGGLE_NEG_PIN>(io::GPIO::Direction::OUTPUT),
         io::getGPIO<vcu::Hardmon::MOTOR_CONTROLLER_TOGGLE_POS_PIN>(io::GPIO::Direction::OUTPUT),
         io::getGPIO<vcu::Hardmon::UC_RESET_PIN>(io::GPIO::Direction::OUTPUT),
         io::getGPIO<vcu::Hardmon::LVSS_EN_PIN>(io::GPIO::Direction::OUTPUT),
         io::getGPIO<vcu::Hardmon::HM_FAULT_PIN>(io::GPIO::Direction::OUTPUT)}};

    io::CAN& ptCAN = io::getCAN<vcu::Hardmon::POWERTRAIN_CAN_TX_PIN, vcu::Hardmon::POWERTRAIN_CAN_RX_PIN>();

    vcu::Hardmon hardmon(hmGPIOS, ptCAN);
    ptCAN.addIRQHandler(powertrainCANInterrupt, &hardmon);

    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////

    // Will store CANopen messages that will be populated by the EVT-core CAN
    // interrupt
    core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ which will add messages to the queue above
    io::CAN& can = io::getCAN<vcu::Hardmon::ACCESSORY_CAN_TX_PIN, vcu::Hardmon::ACCESSORY_CAN_RX_PIN>();
    can.addIRQHandler(canOpenInterrupt, reinterpret_cast<void*>(&canOpenQueue));

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE];
    CO_TMR_MEM appTmrMem[16];

    // Reserve driver variables
    CO_IF_DRV canStackDriver;

    CO_IF_CAN_DRV canDriver;
    CO_IF_TIMER_DRV timerDriver;
    CO_IF_NVM_DRV nvmDriver;

    CO_NODE canNode;

    // Attempt to join the CAN network
    io::CAN::CANStatus result = can.connect();

    // test that the board is connected to the can network
    if (result != io::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    // Initialize all the CANOpen drivers.
    io::initializeCANopenDriver(&canOpenQueue, &can, &timer, &canStackDriver, &nvmDriver, &timerDriver, &canDriver);

    // Initialize the CANOpen node we are using.
    io::initializeCANopenNode(&canNode, &hardmon, &canStackDriver, sdoBuffer, appTmrMem);

    // Set the node to operational mode
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    time::wait(500);

    // print any CANopen errors
    uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));
    ////////////////////////
    // Initialize Threadx //
    ////////////////////////

    // Initialize Bytepool

    rtos::BytePool<TX_APP_MEM_POOL_SIZE> txPool((char*) "txBytePool");

    // Initialize Threads

    // Model Thread
    /// eventflag that triggers the model to run
    rtos::EventFlags modelTriggerFlag((char*) "Model Trigger Flag");

    /// timer that triggers the model eventflag (and thus steps the model)
    rtos::Timer<rtos::EventFlags*> modelTriggerTimer((char*) "Model Trigger Timer",
                                                     modelTimerExpiration,
                                                     &modelTriggerFlag,
                                                     MODEL_THREAD_TRIGGER_RATE,
                                                     MODEL_THREAD_TRIGGER_RATE,
                                                     true);

    /// Argument struct the modelThread takes in
    modelThreadArgs_t modelThreadArgs = {
        &hardmon,
        &modelTriggerFlag,
    };

    /// Thread that runs the model
    rtos::Thread<modelThreadArgs_t*> modelThread((char*) "Model Thread",
                                                 modelThreadEntry,
                                                 &modelThreadArgs,
                                                 MODEL_THREAD_STACK_SIZE,
                                                 MODEL_THREAD_PRIORITY,
                                                 MODEL_THREAD_PREEMPT_THRESHOLD,
                                                 MODEL_THREAD_TIME_SLICE,
                                                 MODEL_THREAD_AUTOSTART);

    // PowerTrain CAN input Thread
    /// argument struct the thread takes in
    powertrainCANReceiveThreadArgs_t powertrainCANReceiveThreadArgs = {&hardmon};

    /// Thread that processes the Powertrain CAN Receive queue
    rtos::Thread<powertrainCANReceiveThreadArgs_t*> powertrainCANReceiveThread((char*) "Powertrain CAN Receive Thread",
                                                                               powertrainCANReceiveThreadEntry,
                                                                               &powertrainCANReceiveThreadArgs,
                                                                               PT_CAN_RECEIVE_STACK_SIZE,
                                                                               PT_CAN_RECEIVE_PRIORITY,
                                                                               PT_CAN_RECEIVE_PREEMPT_THRESHOLD,
                                                                               PT_CAN_RECEIVE_TIME_SLICE,
                                                                               PT_CAN_RECEIVE_AUTOSTART);

    /// Argument struct the healthThread takes in
    healthThreadArgs_t healthThreadArgs{&hardmon};

    /// Thread that checks the health of the other threads
    rtos::Thread<healthThreadArgs_t*> healthThread((char*) "Hardmon Health Monitoring Thread",
                                                   healthThreadEntry,
                                                   &healthThreadArgs,
                                                   HEALTH_THREAD_STACK_SIZE,
                                                   HEALTH_THREAD_PRIORITY,
                                                   HEALTH_THREAD_PREEMPT_THRESHOLD,
                                                   HEALTH_THREAD_TIME_SLICE,
                                                   HEALTH_THREAD_AUTOSTART);

    /// Argument struct the Accessory Can Receive takes in
    accessoryCanReceiveThreadArgs_t accessoryCanReceiveThreadArgs{&hardmon};

    /// Thread that checks the health of the other threads
    rtos::Thread<accessoryCanReceiveThreadArgs_t*> accessoryCanReceiveThread(
        (char*) "Hardmon Accessory Can Recieve Thread",
        accessoryCanReceiveThreadEntry,
        &accessoryCanReceiveThreadArgs,
        ACC_CAN_RECEIVE_THREAD_STACK_SIZE,
        ACC_CAN_RECEIVE_THREAD_PRIORITY,
        ACC_CAN_RECEIVE_THREAD_PREEMPT_THRESHOLD,
        ACC_CAN_RECEIVE_THREAD_TIME_SLICE,
        ACC_CAN_RECEIVE_THREAD_AUTOSTART);

    rtos::Initializable* initArr[] = {&hardmon,
                                      &modelThread,
                                      &modelTriggerFlag,
                                      &modelTriggerTimer,
                                      &powertrainCANReceiveThread,
                                      &healthThread,
                                      &accessoryCanReceiveThread};

    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Starting Kernel.");

    rtos::startKernel(initArr, sizeof(initArr) / sizeof(initArr[0]), txPool);
}

/**
 * Triggers every time the model timer runs its course. Sets the first flag of the modelTriggerFlag to 1
 *
 * @param modelTriggerFlag the eventFlags that controls the model triggering.
 */
void modelTimerExpiration(rtos::EventFlags* modelTriggerFlag) {
    uint32_t flags;
    modelTriggerFlag->getCurrentFlags(&flags);
    if ((flags & 0x01) == 0x1) {
        // THE MODEL IS NOT RUNNING FAST ENOUGH THROW AN ERROR
        // todo: determine what error to throw
        log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Model Thread not running fast enough.");
    }
    modelTriggerFlag->set(0x01);
}

/**
 * Entry Function for the thread that runs the model. Waits for the modelTimer to
 * set the modelTriggerFlag to run one step of the process method
 *
 * @param args the arguments for this thread
 */
[[noreturn]] void modelThreadEntry(modelThreadArgs_t* args) {
    rtos::TXError error;
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Model Thread Started.");
    while (true) {
        uint32_t flagOutput;
        args->triggerFlag->get(0x01, true, true, rtos::TXWait::TXW_WAIT_FOREVER, &flagOutput);
        log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Model Starting.");
        args->hardmon->process();
        log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Model Processed.");
    }
}

/**
 * Entry Function for the powertrainCANThread.
 *
 * @param args the arguments for this thread
 */
[[noreturn]] void powertrainCANReceiveThreadEntry(powertrainCANReceiveThreadArgs_t* args) {
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Powertrain CAN Thread Started.");
    io::CANMessage message;
    while (true) {
        // suspends if there are no messages to receive
        args->hardmon->receiveFromPowertrainQueue(&message, rtos::TXWait::TXW_WAIT_FOREVER);
        args->hardmon->handlePowertrainCanMessage(message);
    }
}

/**
 * Entry Function for the healthThread.
 *
 * @param args the arguments for this thread
 */
[[noreturn]] void healthThreadEntry(healthThreadArgs_t* args) {
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Health Thread Started.");

    rtos::TXError error;
    while (true) {
        log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Health Thread Triggered.");
        // do healthThread stuff
        error = rtos::sleep(MS_TO_TICKS(50));
        log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Health Thread Sleep Code %d", error);
    }
}

/**
 * Entry Function for the Accessory CAN Receive Thread.
 *
 * @param args the arguments for this thread
 */
[[noreturn]] void accessoryCanReceiveThreadEntry(accessoryCanReceiveThreadArgs_t* args) {
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Accessory Can Thread Started.");
    rtos::TXError error;
    // todo: Hardmon CANOpen dictionary is incomplete so trying to process the CAN will break it. This needs to be
    // fixed.

    //    while(true) {
    //        //process accessory CAN
    //        io::processCANopenNode(args->accessoryCanNode);
    //    }
}
