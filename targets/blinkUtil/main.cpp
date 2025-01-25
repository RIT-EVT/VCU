/**
 * This example is the basic LED blink sample. This samples shows how the
 * device level drivers (the LED in this case) can be established by providing
 * the driver with the corresponding io level driver (GPio in this case).
 *
 * The code will setup a GPio pin as an output then setup an LED using that
 * GPio.
 */

#include <core/manager.hpp>
#include <core/utils/time.hpp>
#include <dev/LED.hpp>

namespace io = core::io;
namespace dev = vcu::dev;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    // Setup the GPio pin.
    // Notice that the pin used is called "LED". Each platform has a dedicated
    // LED pin, for the f3xx that is PB_13.
    io::UART& uart = io::getUART<io::Pin::UART_TX, io::Pin::UART_RX>(9600);
    uart.printf("Test\n");
    io::GPIO& ledGPio = io::getGPIO<io::Pin::LED>();
    dev::LED led(ledGPio, dev::LED::ActiveState::HIGH);

    uint32_t counter = 0;
    while (1) {
        led.toggle();
        counter++;
        uart.printf("Toggle %d\n", counter);

        // Wait half a second
        time::wait(500);
    }

    return 0;
}
