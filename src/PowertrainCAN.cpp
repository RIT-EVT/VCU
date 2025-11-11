#include <PowertrainCAN.hpp>

namespace vcu::dev {

PowertrainCAN::PowertrainCAN(io::CAN& can)
    : Initializable("Powertrain CAN"), can(can),
      queue("Powertrain Queue", sizeof(io::CANMessage), POWERTRAIN_QUEUE_SIZE) {}

uint8_t PowertrainCAN::parseMCState(io::CANMessage& message) {
    return (message.getPayload()[0]);
}

uint8_t PowertrainCAN::parseMCDischarge(io::CANMessage& message) {
    return ((message.getPayload()[4] >> 5));
}

int16_t PowertrainCAN::parseHIBThrottle(io::CANMessage& message) {
    // TODO: HIB example implementation, update when HIB is completed
    uint8_t* message_payload = message.getPayload();
    uint16_t throttle        = (message_payload[0]);
    throttle <<= 8;
    throttle += (message_payload[1]);
    return throttle;
}

bool PowertrainCAN::parseHIBForwardEnable(io::CANMessage& message) {
    // TODO: HIB example implementation, update when HIB is completed
    bool forwardEnable = (message.getPayload()[2] & 0b10000000) != 0;
    return forwardEnable;
}

bool PowertrainCAN::parseHIBStartPressed(io::CANMessage& message) {
    // TODO: HIB example implementation, update when HIB is completed
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

void PowertrainCAN::setMCAll(int16_t torque, int16_t speed, int16_t direction, bool inverterEn, bool inverterDC,
                             int16_t speedModeEn, int8_t rollingCounter, int16_t torqueLimit) {
    mcCommandPayload.torque               = torque;
    mcCommandPayload.speed                = speed;
    mcCommandPayload.direction            = direction;
    mcCommandPayload.inverterEnable       = inverterEn;
    mcCommandPayload.inverterDischarge    = inverterDC;
    mcCommandPayload.speedModeEnable      = speedModeEn;
    mcCommandPayload.rollingCounter       = rollingCounter;
    mcCommandPayload.CommandedTorqueLimit = torqueLimit;
}

io::CAN::CANStatus PowertrainCAN::sendMCMessage() {
    // gotta be a uint8_t array, so we memcpy into it.
    uint8_t payload[8];
    memcpy(payload, &mcCommandPayload, 8u);
    // make the message
    io::CANMessage message = io::CANMessage(PowertrainCAN::MessageIDs::MC_COMMAND_ID, 8u, payload, false);
    // send the message
    return can.transmit(message);
}

void PowertrainCAN::setBMSContactor(int16_t contactorCommand) {
    bmsPayload.contactorCommand = contactorCommand;
}

io::CAN::CANStatus PowertrainCAN::sendBMSMessage() {
    // gotta be a uint8_t array, so we memcpy into it.
    uint8_t payload[8];
    memcpy(payload, &bmsPayload, 8u);
    // make the message
    io::CANMessage message = io::CANMessage(PowertrainCAN::MessageIDs::BMS_MESSAGE_ID, 8u, payload, false);
    // send the message
    return can.transmit(message);
}

void PowertrainCAN::sendUCSelfTestMessage() {
    can.transmit(UCSelfTestMessage);
}

void PowertrainCAN::sendHardmonSelfTestResponse() {
    can.transmit(UCSelfTestMessage);
}

core::rtos::TXError PowertrainCAN::init(rtos::BytePoolBase& pool) {
    return queue.init(pool);
}

} // namespace vcu::dev
