#include <BoardMessageParsers.hpp>

namespace boards {

BMSParsed parseBMSMessage(io::CANMessage& message) {
    // TODO: BMS example implementation, update when BMS CAN message format is known
    uint8_t* payload     = message.getPayload();
    bool contactorClosed = ((payload[0] & 0b10000000) == 0);
    return BMSParsed{{0}, {0}, contactorClosed};
}

HIBParsed parseHIBMessage(io::CANMessage& message) {
    uint8_t* payload  = message.getPayload();
    uint16_t throttle = (payload[0] << 8) | payload[1];

    uint16_t brakeMillivolts = (payload[2] << 8) | payload[3];  // 0 to 12_000 (millivolts)
    bool brakeOn         = brakeMillivolts > 6000;  // Completely magical number decided by EE's

    bool comparisonFault = ((payload[4] & 0b00000100) | (payload[5] & 0b00000100)) != 0;
    bool forwardEnable   = (payload[6] & 0b00000001) != 0;  // todo: double check these once HIB code is merged
    bool startPressed    = (payload[6] & 0b00000010) != 0;

    return HIBParsed{static_cast<int16_t>(throttle), forwardEnable,
                     startPressed, brakeOn, comparisonFault};
}

GFDBParsed parseGFDBMessage(io::CANMessage& message) {
    // Based on SIM200 CAN Documentation
    uint8_t* payload       = message.getPayload();
    uint8_t isolationState = payload[1];
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
