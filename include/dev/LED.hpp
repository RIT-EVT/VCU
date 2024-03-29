#ifndef VCU_LED
#define VCU_LED

#include <EVT/io/GPIO.hpp>

namespace VCU::DEV {

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
    LED(EVT::core::IO::GPIO& gpio, ActiveState activeState);

    /**
     * Toggle the current state of the LED
     */
    void toggle();

    /**
     * Set the current state of the LED.
     *
     * @param state The state to set the LED to.
     */
    void setState(EVT::core::IO::GPIO::State state);

private:
    /// The gpio pin used by the LED
    EVT::core::IO::GPIO& gpio;
    /// If the LED is active high or active low
    ActiveState activeState;
};

}//namespace VCU::DEV

#endif//VCU_LED
