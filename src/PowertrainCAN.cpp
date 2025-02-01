#include <dev/PowertrainCAN.hpp>

namespace vcu::dev {

PowertrainCAN::PowertrainCAN(IO::CAN& can) : can(can) {
    queue = core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage>();
}

uint8_t PowertrainCAN::parseMCState(IO::CANMessage& message) {
    return (message.getPayload()[0]);
}

uint8_t PowertrainCAN::parseMCDischarge(IO::CANMessage& message) {
    return ((message.getPayload()[4] >> 5));
}

int16_t PowertrainCAN::parseHIBThrottle(IO::CANMessage& message) {
    //TODO: HIB example implementation, update when HIB is completed
    uint8_t* message_payload = message.getPayload();
    uint16_t throttle = (message_payload[0]);
    throttle <<= 8;
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

void PowertrainCAN::setMCInverterEnable(bool inverterEnable) {
    mcCommandPayload.inverterEnable = inverterEnable;
}

void PowertrainCAN::setMCInverterDischarge(bool inverterDischarge) {
    mcCommandPayload.inverterDischarge = inverterDischarge;
}

void PowertrainCAN::setMCTorque(int16_t torqueRequest) {
    mcCommandPayload.torque = torqueRequest;
}

void PowertrainCAN::sendMCMessage() {
    //gotta be a uint8_t array, so we memcpy into it.
    uint8_t payload[8];
    std::memcpy(payload, &mcCommandPayload, 8u);
    //make the message
    IO::CANMessage message = IO::CANMessage(MC_COMMAND_MESSAGE_ID, 8u, payload, false);
    //send the message
    can.transmit(message);
}

void PowertrainCAN::sendUCSelfTestMessage() {
    can.transmit(UCSelfTestMessage);
}

void PowertrainCAN::sendHardmonSelfTestResponse() {
    can.transmit(UCSelfTestMessage);
}

}// namespace VCU::DEV
