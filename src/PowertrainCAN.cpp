#include <PowertrainCAN.hpp>

namespace vcu::dev {

PowertrainCAN::PowertrainCAN(io::CAN& can)
    : Initializable("Powertrain CAN"), can(can),
      queue("Powertrain Queue", sizeof(io::CANMessage), POWERTRAIN_QUEUE_SIZE) {}

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
                             int16_t speedModeEn, int16_t rollingCounter, int16_t torqueLimit) {
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

io::CAN::CANStatus PowertrainCAN::sendGFDBStateRequest() {
    return can.transmit(GFDBStateRequestMessage);
}

io::CAN::CANStatus PowertrainCAN::sendShutdownWarningMessage() {
    return can.transmit(GUBShutdownWarningMessage);
}

io::CAN::CANStatus PowertrainCAN::sendUCSelfTestMessage() {
    can.transmit(UCSelfTestMessage);
}

io::CAN::CANStatus PowertrainCAN::sendHardmonSelfTestResponse() {
    can.transmit(UCSelfTestMessage);
}

core::rtos::TXError PowertrainCAN::init(rtos::BytePoolBase& pool) {
    return queue.init(pool);
}

} // namespace vcu::dev
