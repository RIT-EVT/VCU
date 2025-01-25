#ifndef VCU_LED
#define VCU_LED

#include <core/io/GPIO.hpp>

namespace vcu::dev {

class LED {
public:
    /**
     * Represents if the LED is active high or active low.
     */
    enum class ActiveState {
        HIGH = 0u,
        LOW = 1u
    };

    /**
     * Create an instance of the LED based on the given GPIO pin.
     *
     * @param gpio GPIO pin
     * @param activeState Represents if the LED is active high or active low
     */
    LED(core::io::GPIO& gpio, ActiveState activeState);

    /**
     * Toggle the current state of the LED
     */
    void toggle();

    /**
     * Set the current state of the LED.
     *
     * @param state The state to set the LED to.
     */
    void setState(core::io::GPIO::State state);

private:
    /// The gpio pin used by the LED
    core::io::GPIO& gpio;
    /// If the LED is active high or active low
    ActiveState activeState;
};

}//namespace VCU::DEV

#endif//VCU_LED
