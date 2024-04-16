#ifndef VCU_POWERTRAINCAN_HPP
#define VCU_POWERTRAINCAN_HPP

#include <cstdint>
#include <EVT/io/types/CANMessage.hpp>
#include <EVT/utils/types/FixedQueue.hpp>

#define POWERTRAIN_QUEUE_SIZE 64

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
    PowertrainCAN();

    uint8_t parseMCState(IO::CANMessage& message);

    uint8_t parseMCDischarge(IO::CANMessage& message);

    uint16_t parseHIBThrottle(IO::CANMessage& message);

    bool parseHIBForwardEnable(IO::CANMessage& message);

    bool parseHIBStartPressed(IO::CANMessage& message);

private:

};

} //namespace VCU::DEV

#endif//VCU_POWERTRAINCAN_HPP
