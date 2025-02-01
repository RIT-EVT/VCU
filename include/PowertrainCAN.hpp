#ifndef VCU_POWERTRAINCAN_HPP
#define VCU_POWERTRAINCAN_HPP

#include <core/io/CAN.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/utils/types/FixedQueue.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>

#define POWERTRAIN_QUEUE_SIZE 64
#define MC_COMMAND_MESSAGE_ID 0x0C0

namespace IO = core::io;

namespace vcu::dev {

class PowertrainCAN {
public:
    /**
     * An enumeration of the Powertrain CAN message ids that are relevant to the
     */
    enum MessageIDs {
        MC_INTERNAL_STATES_ID = (uint32_t) 0x0AA,
        HIB_MESSAGE_ID = (uint32_t) 0x012,              //TODO: HIB this is NOT the correct id, must be updated when the HIB is finished
        HARDMON_SELF_TEST_MESSAGE_ID = (uint32_t) 0x044,//TODO: This is not the correct ID, will not work!
        UC_SELF_TEST_MESSAGE_ID = (uint32_t) 0x045      //TODO: This is not the correct ID, will not work!
    };

    /**
     * Queue to store CAN messages.
     */
    core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage> queue;

    /**
     * Constructor for a PowertrainCan instance.
     *
     * @param can[in] powertrain can driver
     */
    PowertrainCAN(IO::CAN& can);

    /**
     * Parses the motor controller state from the motor controller internal state message
     *  (message with id = MC_INTERNAL_STATES_ID)
     *
     * @param message[in] a message from the motor controller describing its internal state
     * @return the state of the motor controller state machine
     */
    uint8_t parseMCState(IO::CANMessage& message);

    /**
     * Parses the motor controller discharge machine state from the motor controller internal state message
     *  (message with id = MC_INTERNAL_STATES_ID)
     *
     * @param message[in] a message from the motor controller describing its internal state.
     * @return the state of the motor controller's discharger internal state machine.
     */
    uint8_t parseMCDischarge(IO::CANMessage& message);

    /**
     * NOTE: EXAMPLE IMPLEMENTATION THAT MUST BE UPDATED
     * Parses the HIB throttle value from the HIB message.
     *  (message with id = HIB_MESSAGE_ID)
     *
     * @param message[in] a message from the HIB that contains throttle information.
     * @return the value of the throttle.
     */
    int16_t parseHIBThrottle(IO::CANMessage& message);

    /**
     * NOTE: EXAMPLE IMPLEMENTATION THAT MUST BE UPDATED
     * Parses the HIB forward enable value from the HIB message.
     *  (message with id = HIB_MESSAGE_ID)
     *
     * @param message[in] a message from the HIB that contains forward enable information.
     * @return whether or not forward enable is on.
     */
    bool parseHIBForwardEnable(IO::CANMessage& message);

    /**
     * NOTE: EXAMPLE IMPLEMENTATION THAT MUST BE UPDATED
     * Parses the HIB start pressed value from the HIB message.
     *  (message with id = HIB_MESSAGE_ID)
     *
     * @param message[in] a message from the HIB that contains start pressed information.
     * @return whether or not start is pressed
     */
    bool parseHIBStartPressed(IO::CANMessage& message);

    /**
     * Sets the inverterEnable value of the Motor Controller Command message.
     *
     * @param inverterEnable[in] the value to set inverterEnable to.
     */
    void setMCInverterEnable(bool inverterEnable);

    /**
     * Sets the inverterDischarge value of the Motor Controller Command message.
     *
     * @param inverterDischarge[in] the value to set inverterDischarge to.
     */
    void setMCInverterDischarge(bool inverterDischarge);

    /**
     * Sets the torqueRequest value of the Motor Controller Command message.
     *
     * @param torqueRequest[in] the value to set torque request to.
     */
    void setMCTorque(int16_t torqueRequest);

    /**
     * Sends the Motor Controller Command message.
     * NOTE: sending the message DOES NOT reset the contents of the message;
     *  i.e. calling this function twice will send two identical messages.
     */
    void sendMCMessage();

    /**
     * Sends a UC Self Test Message that the Hardmon will respond to.
     *  (Message is the same every time)
     */
    void sendUCSelfTestMessage();

    /**
     * Sends the Hardmon Response to the UC Self Test Message.
     *  (Message is the same every time).
     */
    void sendHardmonSelfTestResponse();

private:
    /**
     * Struct that represents the structure of the Motor Controller Command Message.
     */
    struct MCCommandPayload {
        int16_t torque;
        int16_t speed;
        uint8_t direction;
        uint8_t inverterEnable : 1;
        uint8_t inverterDischarge : 1;
        uint8_t speedModeEnable : 1;
        uint8_t padding : 5;
        int16_t CommandedTorqueLimit;
    } __attribute__((packed));

    ///Local instantiation of the command payload.
    MCCommandPayload mcCommandPayload = {
        0, 0, 1, 0, 0, 0, 0, 0};

    /// Can Driver
    IO::CAN& can;

    /// Example payload for the UC selfTest Message.
    /// In the future, could be replaced by a more meaningful payload
    uint8_t UCSelfTestPayload = 4;
    ///the uc self test message
    IO::CANMessage UCSelfTestMessage = IO::CANMessage(UC_SELF_TEST_MESSAGE_ID, 1, &UCSelfTestPayload, false);

    /// Example payload for the Hardmon selfTest Response Message.
    /// In the future, could be replaced by a more meaningful payload
    uint8_t HardmonSelfTestResponsePayload = 3;
    ///the hardmon self test message
    IO::CANMessage HardmonSelfTestResponse = IO::CANMessage(HARDMON_SELF_TEST_MESSAGE_ID, 1, &HardmonSelfTestResponsePayload, false);
};

}// namespace vcu::dev

#endif//VCU_POWERTRAINCAN_HPP
