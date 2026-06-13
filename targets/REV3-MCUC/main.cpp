/**
 * This is the main target for the MCUC (the main VCU board).
 * It sends and receives CANopen messages from the accessory boards
 * and sends and receives CAN messages from the powertrain.
 */

#include <core/io/CAN.hpp>
#include <core/io/UART.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <core/utils/types/FixedQueue.hpp>

#include <core/io/CANopen.hpp>
#include <core/utils/log.hpp>

#include <co_core.h>
#include <co_if.h>
#include <co_tmr.h>

#include <MCuC.hpp>
#include <PowertrainCAN.hpp>

// rtos includes
#include <core/rtos/BytePool.hpp>
#include <core/rtos/Enums.hpp>
#include <core/rtos/EventFlags.hpp>
#include <core/rtos/Semaphore.hpp>
#include <core/rtos/Thread.hpp>
#include <core/rtos/Timer.hpp>
#include <core/rtos/tsio/ThreadUART.hpp>

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
#define MODEL_THREAD_TRIGGER_RATE MS_TO_TICKS(200)

/// How long until start the model trigger rates (give long enough to start)
#define MODEL_THREAD_TRIGGER_START MS_TO_TICKS(75)

/// How often to send the isolation state request to the ground fault detection board
#define GFDB_TRIGGER_START              MS_TO_TICKS(100)
#define GFDB_REQUEST_TIMER_TRIGGER_RATE MS_TO_TICKS(100)

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
#define HEALTH_THREAD_PRIORITY          4
#define HEALTH_THREAD_PREEMPT_THRESHOLD 4
#define HEALTH_THREAD_TIME_SLICE        MS_TO_TICKS(10)
#define HEALTH_THREAD_AUTOSTART         true

// Accessory CAN Receive Thread Parameters
#define ACC_CAN_RECEIVE_THREAD_STACK_SIZE        1024
#define ACC_CAN_RECEIVE_THREAD_PRIORITY          5
#define ACC_CAN_RECEIVE_THREAD_PREEMPT_THRESHOLD 5
#define ACC_CAN_RECEIVE_THREAD_TIME_SLICE        MS_TO_TICKS(10)
#define ACC_CAN_RECEIVE_THREAD_AUTOSTART         true

#define CANOPEN_NODE_ID_MASK 0x7F

// Model Thread sharedFlag Mask
constexpr uint32_t MODEL_TRIGGER_FLAG_MASK = 0x01;

// Constexpr's for sharedFlag masks for Health thread related flags
constexpr int MODEL_THREAD_SLOW_LS = 1;
constexpr int MODEL_THREAD_LS      = 2;
constexpr int PT_CAN_THREAD_LS     = 3;
constexpr int PTCAN_ERR_LS         = 4;
constexpr int CANOPEN_THREAD_LS    = 5;
constexpr int GFDB_TIMER_THREAD_LS = 6;

constexpr uint32_t MODEL_THREAD_SLOW_MASK = 1 << MODEL_THREAD_SLOW_LS;
constexpr uint32_t MODEL_THREAD_MASK      = 1 << MODEL_THREAD_LS;
constexpr uint32_t PT_CAN_THREAD_MASK     = 1 << PT_CAN_THREAD_LS;
constexpr uint32_t PT_CAN_ERR_MASK        = 1 << PTCAN_ERR_LS;
constexpr uint32_t CANOPEN_THREAD_MASK    = 1 << CANOPEN_THREAD_LS;
constexpr uint32_t GFDB_TIMER_THREAD_MASK = 1 << GFDB_TIMER_THREAD_LS;

constexpr uint32_t LVSS_OUT_CHANGED_MASK = 1 << 14;
constexpr uint32_t VCU_STATE_CHANGE_MASK = 1 << 15;

constexpr uint32_t FULL_HEALTH_THREAD_MASK = MODEL_THREAD_SLOW_MASK | MODEL_THREAD_MASK | PT_CAN_THREAD_MASK
    | PT_CAN_ERR_MASK | CANOPEN_THREAD_MASK | GFDB_TIMER_THREAD_MASK;

// Thread Structs

/**
 * Struct that holds information needed for the model thread
 */
typedef struct {
    vcu::MCuC* mcuc;
    CO_NODE* accessoryCanNode;
    rtos::EventFlags* triggerFlag;
} modelThreadArgs_t;

/**
 * Struct that holds information needed for the canopen interrupt
 */
typedef struct {
    vcu::MCuC* mcuc;
    core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage>* queue;
} canOpenInterruptArgs_t;

/**
 * Struct that holds information needed for the powertrain CAN thread
 */
typedef struct {
    vcu::MCuC* mcuc;
    rtos::EventFlags* eventFlags;
} powertrainCANReceiveThreadArgs_t;

/**
 * Struct that holds information needed for the powertrain CAN ISR
 */
typedef struct {
    vcu::MCuC* mcuc;
    rtos::EventFlags* eventFlags;
} powertrainCANReceiveISRArgs_t;

/**
 * Struct that holds information needed for the health thread
 */
typedef struct {
    vcu::MCuC* mcuc;
    CO_NODE* accessoryCanNode;
    rtos::EventFlags* eventFlags;
} healthThreadArgs_t;

/**
 * Struct that holds information needed for the accessory CANopen Thread
 */
typedef struct {
    vcu::MCuC* mcuc;
    CO_NODE* accessoryCanNode;
    rtos::EventFlags* eventFlags;
} accessoryCanReceiveThreadArgs_t;

/**
 * Struct that holds information needed for the gfdb timer expiration function
 */
typedef struct {
    vcu::MCuC* mcuc;
    rtos::EventFlags* eventFlags;
} gfdbArgs_t;

// Timer expiration function for model trigger
void modelTimerExpiration(rtos::EventFlags* modelTriggerFlag);

// Timer expiration function for gfdb isolation state request
void gfdbTimerExpiration(gfdbArgs_t* args);

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
 * @param message[in] The passed in CAN message that was read.
 * @param priv[in] The canOpenInterruptArgs instance that holds the mcuc & canOpen queue
 */
void accessoryCANOpenInterrupt(io::CANMessage& message, void* priv) {
    auto* args = (canOpenInterruptArgs_t*) priv;
    if (args != nullptr) {
        args->queue->append(message);
        uint32_t nodeID = message.getId() & CANOPEN_NODE_ID_MASK;
        args->mcuc->updateCanOpenNodeHeartbeat(nodeID);
    }
}

/**
 * Interrupt handler to get CAN messages from the powertrain CAN line.
 * @param message[in] the passed in CAN message that was read.
 * @param priv[in] The MCuC instance that contains the queue the message is to be added to. Must be an vcu::MCuC*
 */
void powertrainCANInterrupt(io::CANMessage& message, void* priv) {
    auto* args = (powertrainCANReceiveISRArgs_t*) priv;

    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);

    if (args != nullptr) {
        // must be tx_no_wait as it's in an ISR
        rtos::TXError response = args->mcuc->sendToPowertrainQueue(&message, rtos::TXWait::TXW_NO_WAIT);
        if (response != rtos::TXError::TXE_SUCCESS) {
            // Will run if queue is full & message wasn't added; or if there is something seriously wrong with the queue
            args->eventFlags->set(PT_CAN_ERR_MASK);
        }
    }
}

int main() {
    // Initialize system
    core::platform::init();

    // Initialize the timer
    dev::Timer& timer = dev::getTimer<dev::MCUTimer::Timer2>(100);

    // UART for testing on VCU
    io::UART& uart = io::getUART<vcu::MCuC::UART_TX, vcu::MCuC::UART_RX>(9600);

    // UART for testing not on VCU
    //    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);

    log::LOGGER.setUART(&uart);
    log::LOGGER.setLogLevel(log::Logger::LogLevel::DEBUG);

    // Initialize MCuC and Powertrain CAN
    vcu::MCuC::MCuC_GPIO gpios = {{
        // Inputs
        io::getGPIO<vcu::MCuC::ESTOP_A_PIN>(io::GPIO::Direction::INPUT),
        io::getGPIO<vcu::MCuC::ESTOP_B_PIN>(io::GPIO::Direction::INPUT),

        io::getGPIO<vcu::MCuC::IGNITION_A_PIN>(io::GPIO::Direction::INPUT),
        io::getGPIO<vcu::MCuC::IGNITION_B_PIN>(io::GPIO::Direction::INPUT),

        io::getGPIO<vcu::MCuC::INTERLOCK_PIN>(io::GPIO::Direction::INPUT),
        io::getGPIO<vcu::MCuC::MC_STATUS_PIN>(io::GPIO::Direction::INPUT),

        io::getGPIO<vcu::MCuC::LS_SELF_TEST_IN_A_PIN>(io::GPIO::Direction::INPUT),
        io::getGPIO<vcu::MCuC::LS_SELF_TEST_IN_B_PIN>(io::GPIO::Direction::INPUT),

        // Outputs
        io::getGPIO<vcu::MCuC::LS_SELF_TEST_OUT_PIN>(io::GPIO::Direction::OUTPUT),
        io::getGPIO<vcu::MCuC::WATCHDOG_PIN>(io::GPIO::Direction::OUTPUT),

        io::getGPIO<vcu::MCuC::FAULT_LED_PIN>(io::GPIO::Direction::OUTPUT),
        io::getGPIO<vcu::MCuC::SUPER_FAULT_LED_PIN>(io::GPIO::Direction::OUTPUT),

        io::getGPIO<vcu::MCuC::CAN_SELF_TEST_PIN>(io::GPIO::Direction::OUTPUT),

        io::getGPIO<vcu::MCuC::MC_TOGGLE_NEGATIVE_PIN>(io::GPIO::Direction::OUTPUT),
        io::getGPIO<vcu::MCuC::MC_TOGGLE_POSITIVE_PIN>(io::GPIO::Direction::OUTPUT),
        io::getGPIO<vcu::MCuC::MC_RELAY_SELF_TEST_PIN>(io::GPIO::Direction::OUTPUT),

        io::getGPIO<vcu::MCuC::LVSS_ENABLE_PIN>(io::GPIO::Direction::OUTPUT),

        io::getGPIO<vcu::MCuC::UC_STATE_ZERO_PIN>(io::GPIO::Direction::OUTPUT),
        io::getGPIO<vcu::MCuC::UC_STATE_ONE_PIN>(io::GPIO::Direction::OUTPUT),
        io::getGPIO<vcu::MCuC::UC_STATE_TWO_PIN>(io::GPIO::Direction::OUTPUT),
        io::getGPIO<vcu::MCuC::UC_STATE_THREE_PIN>(io::GPIO::Direction::OUTPUT),

        io::getGPIO<vcu::MCuC::LED_ONE_PIN>(io::GPIO::Direction::OUTPUT),
        io::getGPIO<vcu::MCuC::LED_TWO_PIN>(io::GPIO::Direction::OUTPUT),
        io::getGPIO<vcu::MCuC::LED_THREE_PIN>(io::GPIO::Direction::OUTPUT),
    }};

    ///////////////////////////////////////////////////////////
    // Setup the POWERTRAIN CAN configurations- this is RAW can
    // so it is simpler than the CANopen setup.
    ///////////////////////////////////////////////////////////

    io::CAN& ptCAN = io::getCAN<vcu::MCuC::POWERTRAIN_CAN_TX_PIN, vcu::MCuC::POWERTRAIN_CAN_RX_PIN>();

    vcu::MCuC mcuc(gpios, ptCAN);

    /// eventflag that stores between thread flags, and most importantly has flag to trigger the main thread to run
    rtos::EventFlags sharedFlags((char*) "Shared Flags");

    powertrainCANReceiveISRArgs_t ptCanISRArgs = {&mcuc, &sharedFlags};

    ptCAN.addIRQHandler(reinterpret_cast<void (*)(io::CANMessage&, void*)>(powertrainCANInterrupt), &ptCanISRArgs);

    //
    //    io::GPIO::State stat = io::GPIO::State::LOW;
    //
    //    auto& pos = io::getGPIO<vcu::MCuC::MC_TOGGLE_POSITIVE_PIN>(io::GPIO::Direction::OUTPUT);
    //    auto& neg = io::getGPIO<vcu::MCuC::MC_TOGGLE_NEGATIVE_PIN>(io::GPIO::Direction::OUTPUT);
    //
    //    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "stloop");
    //    while (1) {
    //        pos.writePin(io::GPIO::State::HIGH);
    //        neg.writePin(io::GPIO::State::LOW);
    //        time::wait(1000);
    //
    //        pos.writePin(io::GPIO::State::LOW);
    //        neg.writePin(io::GPIO::State::HIGH);
    //        time::wait(1000);
    //
    //        gpios.faultLEDGPIO.writePin(stat);
    //        stat = (stat == io::GPIO::State::LOW ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    //        //        pos.writePin(io::GPIO::State::LOW);
    //        //        neg.writePin(io::GPIO::State::LOW);
    //        //        time::wait(100);
    //        //
    //        //
    //        //        pos.writePin(io::GPIO::State::LOW);
    //        //        neg.writePin(io::GPIO::State::HIGH);
    //        //        time::wait(300);
    //        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "eol");
    //    }
    //
    //
    //
    //
    //
    //

    io::CAN::CANStatus ptRes = ptCAN.connect(true);

    // test that the board is connected to the can network
    if (ptRes != io::CAN::CANStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to connect to Powertrain CAN network");
        return 1;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Setup ACCESSORY CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////

    // Will store CANopen messages that will be populated by the EVT-core CAN interrupt
    core::types::FixedQueue<CANOPEN_QUEUE_SIZE, io::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ which will add messages to the queue above
    // CAN init for testing not on VCU
    //    io::CAN& accessoryCAN = io::getCAN<io::Pin::PA_12, io::Pin::PA_11>();

    // Actual CAN init
    io::CAN& accessoryCAN = io::getCAN<vcu::MCuC::ACCESSORY_CAN_TX_PIN, vcu::MCuC::ACCESSORY_CAN_RX_PIN>();
    canOpenInterruptArgs_t canOpenIntArgs = {&mcuc, &canOpenQueue};
    accessoryCAN.addIRQHandler(accessoryCANOpenInterrupt, reinterpret_cast<void*>(&canOpenIntArgs));

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
    io::CAN::CANStatus result = accessoryCAN.connect(true);

    // test that the board is connected to the can network
    if (result != io::CAN::CANStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to connect to Accessory CAN network");
        return 1;
    }

    // Initialize all the CANOpen drivers.
    io::initializeCANopenDriver(
        &canOpenQueue, &accessoryCAN, &timer, &canStackDriver, &nvmDriver, &timerDriver, &canDriver);

    // Initialize the CANOpen node we are using.
    io::initializeCANopenNode(&canNode, &mcuc, &canStackDriver, sdoBuffer, appTmrMem);

    // Set the node to operational mode
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    time::wait(500);

    // print any CANopen errors
    log::LOGGER.log(log::Logger::LogLevel::WARNING, "CanOpen Error: %d\r\n", CONodeGetErr(&canNode));

    ////////////////////////
    // Initialize Threadx //
    ////////////////////////

    // Initialize Bytepool

    rtos::BytePool<TX_APP_MEM_POOL_SIZE> txPool((char*) "txBytePool");

    // Initialize Threads
    /// timer that triggers the main threads eventflag bit (and thus steps the model)
    rtos::Timer<rtos::EventFlags*> modelTriggerTimer((char*) "Model Trigger Timer",
                                                     modelTimerExpiration,
                                                     &sharedFlags,
                                                     MODEL_THREAD_TRIGGER_START,
                                                     MODEL_THREAD_TRIGGER_RATE,
                                                     true);

    gfdbArgs_t gfdbArgs = {&mcuc, &sharedFlags};

    /// timer that triggers the gfdb request message
    rtos::Timer<gfdbArgs_t*> gfdbTriggerTimer((char*) "GFDB Trigger Timer",
                                              gfdbTimerExpiration,
                                              &gfdbArgs,
                                              GFDB_TRIGGER_START,
                                              GFDB_REQUEST_TIMER_TRIGGER_RATE,
                                              true);

    /// Argument struct the modelThread takes in
    modelThreadArgs_t modelThreadArgs = {&mcuc, &canNode, &sharedFlags};

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
    powertrainCANReceiveThreadArgs_t powertrainCANReceiveThreadArgs = {&mcuc, &sharedFlags};

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
    healthThreadArgs_t healthThreadArgs{&mcuc, &canNode, &sharedFlags};

    /// Thread that checks the health of the other threads
    rtos::Thread<healthThreadArgs_t*> healthThread((char*) "MCuC Health Monitoring Thread",
                                                   healthThreadEntry,
                                                   &healthThreadArgs,
                                                   HEALTH_THREAD_STACK_SIZE,
                                                   HEALTH_THREAD_PRIORITY,
                                                   HEALTH_THREAD_PREEMPT_THRESHOLD,
                                                   HEALTH_THREAD_TIME_SLICE,
                                                   HEALTH_THREAD_AUTOSTART);

    /// Argument struct the Accessory Can Receive takes in
    accessoryCanReceiveThreadArgs_t accessoryCanReceiveThreadArgs{&mcuc, &canNode, &sharedFlags};

    /// Thread that checks the health of the other threads
    rtos::Thread<accessoryCanReceiveThreadArgs_t*> accessoryCanReceiveThread(
        (char*) "MCuC Accessory Can Recieve Thread",
        accessoryCanReceiveThreadEntry,
        &accessoryCanReceiveThreadArgs,
        ACC_CAN_RECEIVE_THREAD_STACK_SIZE,
        ACC_CAN_RECEIVE_THREAD_PRIORITY,
        ACC_CAN_RECEIVE_THREAD_PREEMPT_THRESHOLD,
        ACC_CAN_RECEIVE_THREAD_TIME_SLICE,
        ACC_CAN_RECEIVE_THREAD_AUTOSTART);

    // Start kernel
    rtos::Initializable* initArr[] = {
        &mcuc,
        &modelThread,
        &sharedFlags,
        &modelTriggerTimer,
        &gfdbTriggerTimer,
        &powertrainCANReceiveThread,
        &healthThread,
        &accessoryCanReceiveThread,
    };

    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Starting Kernel");
    rtos::startKernel(initArr, sizeof(initArr) / sizeof(initArr[0]), txPool);
}

/**
 * Triggers every time the gfdb timer expires
 *
 * @param args the arguments for this timer expiration function
 */
void gfdbTimerExpiration(gfdbArgs_t* args) {
    args->mcuc->setGroundFaultFlag();
    args->eventFlags->set(GFDB_TIMER_THREAD_MASK); // Mark as ran
}

/**
 * Triggers every time the model timer runs its course. Sets the first flag of the modelTriggerFlag to 1
 *
 * @param modelTriggerFlag the eventFlags that controls the model triggering.
 */
void modelTimerExpiration(rtos::EventFlags* modelTriggerFlag) {
    uint32_t flags;
    modelTriggerFlag->getCurrentFlags(&flags);
    if ((flags & MODEL_TRIGGER_FLAG_MASK) == MODEL_TRIGGER_FLAG_MASK) {
        // If last trigger flag hasn't been cleared yet (model missed a run)
        modelTriggerFlag->set(MODEL_THREAD_SLOW_LS);
    }
    modelTriggerFlag->set(MODEL_TRIGGER_FLAG_MASK);
}

/**
 * Entry Function for the thread that runs the model. Waits for the modelTimer to
 * set the modelTriggerFlag to run one step of the process method
 *
 * @param args the arguments for this thread
 */
[[noreturn]] void modelThreadEntry(modelThreadArgs_t* args) {
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Model Thread Started");

    while (true) {
        uint32_t flagOutput;
        args->triggerFlag->get(MODEL_TRIGGER_FLAG_MASK, true, true, rtos::TXWait::TXW_WAIT_FOREVER, &flagOutput);

        args->mcuc->process(args->triggerFlag);

        // If state has changed, alert canOpen to send the flags when it gets the chance
        uint32_t current;
        args->triggerFlag->getCurrentFlags(&current);
        if (current & VCU_STATE_CHANGE_MASK) {
            io::alertTPDO(args->accessoryCanNode, vcu::MCuC::SIM_STATE_TPDO_NUM);
            args->triggerFlag->clear(VCU_STATE_CHANGE_MASK);
        }

        // If LVSS enable signal has been changed, alert canOpen
        if (current & LVSS_OUT_CHANGED_MASK) {
            io::alertTPDO(args->accessoryCanNode, 0);
            args->triggerFlag->clear(LVSS_OUT_CHANGED_MASK);
        }

        args->triggerFlag->set(MODEL_THREAD_MASK); // Mark as ran
    }
}

/**
 * Entry Function for the powertrainCANThread.
 *
 * @param args the arguments for this thread
 */
[[noreturn]] void powertrainCANReceiveThreadEntry(powertrainCANReceiveThreadArgs_t* args) {
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Powertrain CAN Receive Thread started");
    io::CANMessage message;
    while (true) {
        // suspends if there are no messages to receive
        args->mcuc->receiveFromPowertrainQueue(&message, rtos::TXWait::TXW_WAIT_FOREVER);

        // save that this node has sent a message
        args->mcuc->updateCanNodeHeartbeat(message.getId());

        // process the message
        args->mcuc->handlePowertrainCanMessage(message);

        args->eventFlags->set(PT_CAN_THREAD_MASK); // mark as ran
    }
}

/**
 * Entry Function for the healthThread.
 *
 * @param args the arguments for this thread
 */
[[noreturn]] void healthThreadEntry(healthThreadArgs_t* args) {
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Health Thread Started");
    rtos::TXError error;
    while (true) {
        uint32_t flagOutput;
        args->eventFlags->getCurrentFlags(&flagOutput);

#define SEND_HEALTH_THREAD_CAN // todo: decide where to put this define. prolly in the cmake
#ifdef SEND_HEALTH_THREAD_CAN
        // This flag will be set up to 3 times every time the device goes through contactor opening / closing states
        bool modelTooSlow = (flagOutput & MODEL_THREAD_SLOW_MASK);

        bool modelNotRun = !(flagOutput & MODEL_THREAD_MASK);

        bool ptcanNotRun = !(flagOutput & PT_CAN_THREAD_MASK);

        bool ptcanISRErr = (flagOutput & PT_CAN_ERR_MASK);

        bool canopenNotRun = !(flagOutput & CANOPEN_THREAD_MASK);

        bool gfdbReqNotRun = !(flagOutput & GFDB_TIMER_THREAD_MASK);

        // set canOpen data values
        args->mcuc->setHealthFlags(modelTooSlow, modelNotRun, ptcanNotRun, ptcanISRErr, canopenNotRun, gfdbReqNotRun);

        // alert canOpen to send the flags when it gets the chance
        io::alertTPDO(args->accessoryCanNode, vcu::MCuC::HEALTH_FLAG_TPDO_NUM);

#endif
        // clear flags for fresh data next loop
        args->eventFlags->clear(FULL_HEALTH_THREAD_MASK);
        rtos::sleep(MS_TO_TICKS(250));
    }
}

/**
 * Entry Function for the Accessory CAN Receive Thread.
 *
 * @param args the arguments for this thread
 */
[[noreturn]] void accessoryCanReceiveThreadEntry(accessoryCanReceiveThreadArgs_t* args) {
    log::LOGGER.log(core::log::Logger::LogLevel::DEBUG, "Accessory CAN Thread Started");
    args->mcuc->sendOutputDataToUnsafeBuffer();
    //  todo:  args->mcuc->accessoryCanDataUnsafeBuffer.LVSS_out_EnableBoardSignal.val = 63; // todo: temporary
    rtos::TXError error;
    while (true) {
        io::processCANopenNode(args->accessoryCanNode);

        args->eventFlags->set(CANOPEN_THREAD_MASK); // Mark as ran

        rtos::sleep(MS_TO_TICKS(10));
    }
}
