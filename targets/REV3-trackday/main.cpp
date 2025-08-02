/**
 * This is the main target for the MCUC for the 2025 track day
 *
 * Don't get it right; get it running.
 * - LJ Boone, 2023
 */

#include <core/io/types/CANMessage.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <core/utils/types/FixedQueue.hpp>
#include <core/utils/log.hpp>

#include <TrackMCuC.hpp>


namespace io   = core::io;
namespace dev  = core::dev;
namespace time = core::time;
namespace log  = core::log;

///////////////////////////////////////////////////////////////////////////////
// EVT-core CAN callback and CAN setup. This will include logic to set
// aside CANopen messages into a specific queue
///////////////////////////////////////////////////////////////////////////////


/**
 * Interrupt handler to get CAN messages from the powertrain CAN line.
 * @param message[in] the passed in CAN message that was read.
 * @param priv[in] The MCuC instance that contains the queue the message is to be added to. Must be an vcu::MCuC*
 */
void accessoryCANInterrupt(io::CANMessage& message, void* priv) {
    //TODO
}

int main() {
    // Initialize system
    core::platform::init();

    io::GPIO& greenLed = io::getGPIO<TrackMCuC::LED_ONE_PIN>();
    io::GPIO& yellowLed = io::getGPIO<TrackMCuC::LED_TWO_PIN>();
    io::GPIO& redLed = io::getGPIO<TrackMCuC::LED_THREE_PIN>();
    io::GPIO& faultLed = io::getGPIO<TrackMCuC::FAULT_LED_PIN>();
    io::GPIO& superFaultLed = io::getGPIO<TrackMCuC::SUPER_FAULT_LED_PIN>();

    io::GPIO& canSelfTest = io::getGPIO<TrackMCuC::CAN_SELF_TEST_PIN>();
    io::GPIO& mcSelfTest = io::getGPIO<TrackMCuC::MC_SELF_TEST_PIN>();

    io::GPIO& mcToggleP = io::getGPIO<TrackMCuC::MC_TOGGLE_POSITIVE_PIN>();
    io::GPIO& mcToggleN = io::getGPIO<TrackMCuC::MC_TOGGLE_NEGATIVE_PIN>();
    io::GPIO& lvssEnable = io::getGPIO<TrackMCuC::LVSS_ENABLE_PIN>();

    io::GPIO& estop = io::getGPIO<TrackMCuC::ESTOP_A_PIN>(io::GPIO::Direction::INPUT);
    io::GPIO& ignition = io::getGPIO<TrackMCuC::IGNITION_A_PIN>(io::GPIO::Direction::INPUT);
    io::GPIO& interlock = io::getGPIO<TrackMCuC::INTERLOCK_PIN>(io::GPIO::Direction::INPUT);

    io::CAN& accCan = io::getCAN<TrackMCuC::ACCESSORY_CAN_TX_PIN, TrackMCuC::ACCESSORY_CAN_RX_PIN>();
    accCan.addIRQHandler(accessoryCANInterrupt, nullptr);
    accCan.connect();

    TrackMCuC mcuc(greenLed, yellowLed, redLed, faultLed, superFaultLed, canSelfTest, mcSelfTest,
                              mcToggleP, mcToggleN, lvssEnable, estop, ignition, interlock,accCan);

    while(true) {
        mcuc.process();
    }
}
