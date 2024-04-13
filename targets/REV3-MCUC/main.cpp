/**
 * This is the main target for the MCUC (the main VCU board).
 * It sends and receives CANopen messages from the accessory boards
 * and sends and receives CAN messages from the powertrain.
 */


#include <EVT/io/CAN.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/types/CANMessage.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

#include <EVT/io/CANopen.hpp>

#include <co_core.h>
#include <co_if.h>
#include <co_tmr.h>

#include <MCUC.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

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
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* queue =
        (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;
    if (queue != nullptr)
        queue->append(message);
}

/**
 * Interrupt handler to get CAN messages from the powertrain CAN line.
 * @param message[in] the passed in in CAN message that was read.
 * @param priv[in] the private data this mesasge requires. Should be the mcuc instance we want to update.
 */
void powertrainCANInterrupt(IO::CANMessage& message, void* priv) {
    VCU::MCUC mcuc = *(VCU::MCUC*) priv;
    mcuc.handlePowertrainCanMessage(message);
}

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Initialize the timer
    DEV::Timer& timer = DEV::getTimer<DEV::MCUTimer::Timer2>(100);

    // UART for testing
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    VCU::MCUC::reqGPIO gpios = {
        IO::getGPIO<VCU::MCUC::ESTOP_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::MCUC::IGNITION_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::MCUC::HM_FAULT_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::MCUC::LVSS_STATUS_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::MCUC::MC_STATUS_PIN>(IO::GPIO::Direction::INPUT),

        IO::getGPIO<VCU::MCUC::UC_FAULT_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::MCUC::LVSS_ENABLE_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::MCUC::WATCHDOG_PIN>(IO::GPIO::Direction::OUTPUT),

        IO::getGPIO<VCU::MCUC::UC_STATE_ZERO_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::MCUC::UC_STATE_ONE_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::MCUC::UC_STATE_TWO_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::MCUC::UC_STATE_THREE_PIN>(IO::GPIO::Direction::OUTPUT),

        IO::getGPIO<VCU::MCUC::MC_TOGGLE_NEGATIVE_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::MCUC::MC_TOGGLE_POSITIVE_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::MCUC::MC_SELF_TEST_PIN>(IO::GPIO::Direction::OUTPUT),
    };

    VCU::MCUC mcuc(gpios);

    //TODO: CANopen uncomment when we add in Accessory CAN configuration
    /*

    ///////////////////////////////////////////////////////////////////////////
    // Setup ACCESSORY CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////

    // Will store CANopen messages that will be populated by the EVT-core CAN interrupt
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

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
    // Setup the POWERTRAIN CAN configurations
    //////////////////////////////////////////////////////////

    IO::CAN& powertrainCAN = IO::getCAN<VCU::MCUC::POWERTRAIN_CAN_TX_PIN, VCU::MCUC::POWERTRAIN_CAN_RX_PIN>();
    powertrainCAN.addIRQHandler(powertrainCANInterrupt, reinterpret_cast<void*>(&mcuc));

    ///////////////////////////////////////////////////////////////////////////
    // Main loop
    ///////////////////////////////////////////////////////////////////////////

    uint8_t lastValue = 0;
    while (1) {
        //IO::processCANopenNode(&canNode); //TODO CANopen uncomment when we add in Accessory can
        mcuc.process();
        // Wait for new data to come in
        time::wait(10);
    }
}