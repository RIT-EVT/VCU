#include <dev/PowertrainCAN.hpp>

namespace VCU::DEV {

PowertrainCAN::PowertrainCAN() {
    queue = EVT::core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage>();
}

uint8_t PowertrainCAN::parseMCState(IO::CANMessage& message) {
    return (message.getPayload()[0]);
}

uint8_t PowertrainCAN::parseMCDischarge(IO::CANMessage& message) {
    return ((message.getPayload()[4] >> 5));
}


uint16_t PowertrainCAN::parseHIBThrottle(IO::CANMessage& message) {
    //TODO: HIB example implementation, update when HIB is completed
    uint8_t* message_payload = message.getPayload();
    uint16_t throttle = (message_payload[0]);
    throttle << 8;
    throttle += (message_payload[1]);
    return throttle;
}

bool PowertrainCAN::parseHIBForwardEnable(IO::CANMessage& message) {
    //TODO: HIB example implementation, update when HIB is completed
    bool forwardEnable = (message.getPayload()[2] & 0b10000000) != 0;
    return forwardEnable;
}

bool PowertrainCAN::parseHIBStartPressed(IO::CANMessage& message) {
    //TODO: HIB example implementation, update when HIB is completed
    bool startPressed = (message.getPayload()[2] & 0b01000000) != 0;
    return startPressed;
}

}
