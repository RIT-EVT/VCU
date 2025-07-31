/**
 * This is the main target for the MCUC for the 2025 track day
 *
 * Don't get it right; get it running.
 * - LJ Boone, 2023
 */

#include <core/io/CAN.hpp>
#include <core/io/UART.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <core/utils/types/FixedQueue.hpp>

#include <core/utils/log.hpp>

#include <PowertrainCAN.hpp>


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
void powertrainCANInterrupt(io::CANMessage& message, void* priv) {
    //TODO
}

int main() {
    // Initialize system
    core::platform::init();

}
