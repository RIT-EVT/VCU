/**
* This is the main target for the Hardmon (the processor that monitors the MCUC).
* It takes in CANopen messages and other inputs from the MCUC and accessory boards,
* and checks if the MCUC is functioning correctly.
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

#include <Hardmon.hpp>

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
void canInterrupt(IO::CANMessage& message, void* priv) {
    auto* queue = (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;
    if (queue != nullptr)
        queue->append(message);
}

/**
 * Interrupt handler to get CAN messages from the powertrain CAN line.
 * @param message[in] the passed in in CAN message that was read.
 * @param priv[in] the private data this mesasge requires. Should be the mcuc instance we want to update.
 */
void powertrainCANInterrupt(IO::CANMessage& message, void* priv) {
    auto* queue = (EVT::core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage>*) priv;
    if (queue != nullptr)
        queue->append(message);
}

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Initialize the timer
    DEV::Timer& timer = DEV::getTimer<DEV::MCUTimer::Timer2>(100);

    // UART for testing
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    //TODO: uncomment when we figure out Accessory CAN
    /*
   ///////////////////////////////////////////////////////////////////////////
   // Setup CAN configuration, this handles making drivers, applying settings.
   // And generally creating the CANopen stack node which is the interface
   // between the application (the code we write) and the physical CAN network
   ///////////////////////////////////////////////////////////////////////////

   // Will store CANopen messages that will be populated by the EVT-core CAN
   // interrupt
   EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

   // Initialize CAN, add an IRQ which will add messages to the queue above
   IO::CAN& can = IO::getCAN<VCU::Hardmon::ACCESSORY_CAN_TX_PIN, VCU::Hardmon::ACCESSORY_CAN_RX_PIN>();
   can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));

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
   IO::CAN::CANStatus result = can.connect();

   //test that the board is connected to the can network
   if (result != IO::CAN::CANStatus::OK) {
       uart.printf("Failed to connect to CAN network\r\n");
       return 1;
   }

   // Initialize all the CANOpen drivers.
   IO::initializeCANopenDriver(&canOpenQueue, &can, &timer, &canStackDriver, &nvmDriver, &timerDriver, &canDriver);

   // Initialize the CANOpen node we are using.
   IO::initializeCANopenNode(&canNode, &hardmon, &canStackDriver, sdoBuffer, appTmrMem);

   // Set the node to operational mode
   CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

   time::wait(500);

   //print any CANopen errors
   uart.printf("Error: %d\r\n", CONodeGetErr(&canNode));

    */

    ////////////////////////
    //Initialize Powertrain CAN
    ////////////////////////////

    VCU::Hardmon::reqGPIO hmGPIOS = {
        IO::getGPIO<VCU::Hardmon::IGNITION_CHECK_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::Hardmon::IGNITION_3V3_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::Hardmon::LVSS_STATUS_3V3_Pin>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::Hardmon::MOTOR_CONTROLLER_STATUS_PIN>(IO::GPIO::Direction::INPUT),

        IO::getGPIO<VCU::Hardmon::UC_STATE_ONE_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::Hardmon::UC_STATE_ONE_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::Hardmon::UC_STATE_TWO_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::Hardmon::UC_STATE_THREE_PIN>(IO::GPIO::Direction::INPUT),

        IO::getGPIO<VCU::Hardmon::ESTOP_CHECK_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::Hardmon::WATCHDOG_PIN>(IO::GPIO::Direction::INPUT),
        IO::getGPIO<VCU::Hardmon::ESTOP_3V3_PIN>(IO::GPIO::Direction::INPUT),

        IO::getGPIO<VCU::Hardmon::LVSS_EN_OVERRIDE_PIN>(),
        IO::getGPIO<VCU::Hardmon::MOTOR_CONTROLLER_TOGGLE_NEG_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::Hardmon::MOTOR_CONTROLLER_TOGGLE_POS_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::Hardmon::MOTOR_CONTROLLER_TOGGLE_OVERRIDE_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::Hardmon::UC_RESET_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::Hardmon::LVSS_EN_PIN>(IO::GPIO::Direction::OUTPUT),
        IO::getGPIO<VCU::Hardmon::HM_FAULT_PIN>(IO::GPIO::Direction::OUTPUT),
    };

    IO::CAN& ptCAN = IO::getCAN<VCU::Hardmon::POWERTRAIN_CAN_TX_PIN, VCU::Hardmon::POWERTRAIN_CAN_RX_PIN>();

    VCU::Hardmon hardmon(hmGPIOS, ptCAN);
    ptCAN.addIRQHandler(powertrainCANInterrupt, reinterpret_cast<void*>(hardmon.getPowertrainQueue()));

    ///////////////////////////////////////////////////////////////////////////
    // Main loop
    ///////////////////////////////////////////////////////////////////////////

    while (1) {
        //IO::processCANopenNode(&canNode);
        hardmon.process();
        // Wait for new data to come in
        time::wait(10);
    }
}