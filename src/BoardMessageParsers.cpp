#include <BoardMessageParsers.hpp>

namespace boards {

BMSParsed parseBMSMessage(io::CANMessage& message) {
    // TODO: BMS example implementation, update when BMS CAN message format is known
    uint8_t* payload     = message.getPayload();
    bool contactorClosed = ((payload[0] & 0b10000000) == 0);
    return BMSParsed{{0}, {0}, contactorClosed};
}

HIBParsed parseHIBMessage(io::CANMessage& message) {
    // TODO: HIB example implementation, update when HIB is completed
    uint8_t* payload  = message.getPayload();
    uint16_t throttle = (payload[0]);
    throttle <<= 8;
    throttle += (payload[1]);

    bool forwardEnable   = ((payload[2] & 0b10000000) != 0);
    bool startPressed    = ((payload[2] & 0b01000000) != 0);
    bool brakeOn         = ((payload[2] & 0b00100000) != 0);
    bool comparisonFault = ((payload[2] & 0b00010000) != 0);

    return HIBParsed{static_cast<int16_t>(throttle), forwardEnable,
                     startPressed, brakeOn, comparisonFault};
}

GFDBParsed parseGFDBMessage(io::CANMessage& message) {
    // TODO: GFDB example implementation, update when GFDB is completed
    uint8_t* payload     = message.getPayload();
    bool isolationState = payload[0];
    return GFDBParsed{isolationState};
}

MCInternalParsed parseMCInternalMessage(io::CANMessage& message) {
    uint8_t state     = message.getPayload()[0];
    uint8_t discharge = (message.getPayload()[4] >> 5);
    return MCInternalParsed{discharge, state};
}

HardmonParsed parseHardmonMessage(io::CANMessage& message) {
    // TODO: Hardmon example implementation, update when Hardmon is completed
    uint8_t* payload  = message.getPayload();
    bool selfTestMode = ((payload[0] & 0b10000000) != 0);
    return HardmonParsed{selfTestMode};
}
}
