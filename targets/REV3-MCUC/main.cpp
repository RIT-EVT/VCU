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

#include <co_core.h>
#include <co_if.h>
#include <co_tmr.h>

#include <MCuC.hpp>
#include <PowertrainCAN.hpp>

namespace io = core::io;
namespace dev = core::dev;
namespace time = core::time;

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
 */
void accessoryCANOpenInterrupt(IO::CANMessage& message, void* priv) {
    auto* queue = (core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;
    if (queue != nullptr)
        queue->append(message);
}

/**
 * Interrupt handler to get CAN messages from the powertrain CAN line.
 * @param message[in] the passed in in CAN message that was read.
 * @param priv[in] the private data this mesasge requires. Should be the mcuc instance we want to update.
 */
void powertrainCANInterrupt(IO::CANMessage& message, void* priv) {
    auto* queue = (core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage>*) priv;
    if (queue != nullptr)
        queue->append(message);
}

int main() {
    // Initialize system
    core::platform::init();

    // Initialize the timer
    dev::Timer& timer = dev::getTimer<dev::MCUTimer::Timer2>(100);

    // UART for testing
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    //TODO: CANopen uncomment when we add in Accessory CAN configuration
    /*

    ///////////////////////////////////////////////////////////////////////////
    // Setup ACCESSORY CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////

    // Will store CANopen messages that will be populated by the EVT-core CAN interrupt
    core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ which will add messages to the queue above
    IO::CAN& accessoryCAN = IO::getCAN<VCU::MCUC::ACCESSORY_CAN_TX_PIN, VCU::MCUC::ACCESSORY_CAN_RX_PIN>();
    accessoryCAN.addIRQHandler(accessoryCANOpenInterrupt, reinterpret_cast<void*>(&canOpenQueue));

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
    IO::CAN::CANStatus result = accessoryCAN.connect();

    //test that the board is connected to the can network
    if (result != IO::CAN::CANStatus::OK) {
        uart.printf("Failed to connect to CAN network\r\n");
        return 1;
    }

    // Initialize all the CANOpen drivers.
    IO::initializeCANopenDriver(&canOpenQueue, &accessoryCAN, &timer, &canStackDriver, &nvmDriver, &timerDriver, &canDriver);

    // Initialize the CANOpen node we are using.
    IO::initializeCANopenNode(&canNode, &mcuc, &canStackDriver, sdoBuffer, appTmrMem);

    // Set the node to operational mode
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    time::wait(500);

    //print any CANopen errors
    uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));

    */

    ///////////////////////////////////////////////////////////
    // Setup the POWERTRAIN CAN configurations- this is RAW can
    // so it is simpler than te CANopen setup.
    //////////////////////////////////////////////////////////

    IO::CAN& ptCAN = IO::getCAN<vcu::MCuC::POWERTRAIN_CAN_TX_PIN, vcu::MCuC::POWERTRAIN_CAN_RX_PIN>();

    vcu::MCuC::MCuC_GPIO gpios = {
            {
            io::getGPIO<vcu::MCuC::ESTOP_PIN>(io::GPIO::Direction::INPUT),
            io::getGPIO<vcu::MCuC::IGNITION_PIN>(io::GPIO::Direction::INPUT),
            io::getGPIO<vcu::MCuC::HM_FAULT_PIN>(io::GPIO::Direction::INPUT),
            io::getGPIO<vcu::MCuC::LVSS_STATUS_PIN>(io::GPIO::Direction::INPUT),
            io::getGPIO<vcu::MCuC::MC_STATUS_PIN>(io::GPIO::Direction::INPUT),

            io::getGPIO<vcu::MCuC::UC_FAULT_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::LVSS_ENABLE_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::WATCHDOG_PIN>(io::GPIO::Direction::OUTPUT),

            io::getGPIO<vcu::MCuC::UC_STATE_ZERO_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::UC_STATE_ONE_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::UC_STATE_TWO_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::UC_STATE_THREE_PIN>(io::GPIO::Direction::OUTPUT),

            io::getGPIO<vcu::MCuC::MC_TOGGLE_NEGATIVE_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::MC_TOGGLE_POSITIVE_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::MC_SELF_TEST_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::ESTOP_SELF_TEST_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::IGNITION_SELF_TEST_PIN>(io::GPIO::Direction::OUTPUT),
            io::getGPIO<vcu::MCuC::CAN_SELF_TEST_PIN>(io::GPIO::Direction::OUTPUT)
            }
        };

    vcu::MCuC mcuc(gpios, ptCAN);
    ptCAN.addIRQHandler(powertrainCANInterrupt, reinterpret_cast<void*>(mcuc.getPowertrainQueue()));

    ///////////////////////////////////////////////////////////////////////////
    // Main loop
    ///////////////////////////////////////////////////////////////////////////

    while (true) {
        //IO:processCANopenNode(&canNode); //TODO CANopen uncomment when we add in Accessory can
        mcuc.process();
        // Wait for new data to come in
        time::wait(10);
    }
}