#ifndef VCU_POWERTRAINCAN_HPP
#define VCU_POWERTRAINCAN_HPP

#include <cstdint>
#include <EVT/utils/types/FixedQueue.hpp>
#include <EVT/io/types/CANMessage.hpp>
#include <EVT/io/CAN.hpp>

#define POWERTRAIN_QUEUE_SIZE 64
#define MC_COMMAND_MESSAGE_ID 0x0C0

namespace IO = EVT::core::IO;

namespace VCU::DEV {

class PowertrainCAN {
public:

    /**
     * An enumeration of the Powertrain CAN message ids that are relevant to the
     */
    enum MessageIDs {
        MC_INTERNAL_STATES_ID = (uint32_t) 0x0AA,
        HIB_MESSAGE_ID = (uint32_t) 0x012, //TODO: HIB this is NOT the correct id, must be updated when the HIB is finished
    };

    /**
     * Queue to store CAN messages.
     */
    EVT::core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage> queue;

    /**
     * Constructor for a PowertrainCan instance.
     */
    PowertrainCAN(IO::CAN& can);

    uint8_t parseMCState(IO::CANMessage& message);

    uint8_t parseMCDischarge(IO::CANMessage& message);

    uint16_t parseHIBThrottle(IO::CANMessage& message);

    bool parseHIBForwardEnable(IO::CANMessage& message);

    bool parseHIBStartPressed(IO::CANMessage& message);

    void setMCInverterEnable(bool inverterEnable);

    void setMCInverterDischarge(bool inverterDischarge);

    void setMCTorque(uint16_t torqueRequest);

    void sendMCMessage();
private:
    struct MCCommandPayload {
        int16_t torque;
        int16_t speed;
        uint8_t direction;
        uint8_t inverterEnable:1;
        uint8_t inverterDischarge:1;
        uint8_t speedModeEnable:1;
        uint8_t padding:5;
        int16_t CommandedTorqueLimit;
    };

    MCCommandPayload mcCommandPayload = {
        0,0,1,0,0,0,0,0
    };
    IO::CAN& can;
    IO::CANMessage message;
};

} //namespace VCU::DEV

#endif//VCU_POWERTRAINCAN_HPP
