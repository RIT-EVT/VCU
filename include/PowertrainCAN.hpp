#ifndef VCU_POWERTRAINCAN_HPP
#define VCU_POWERTRAINCAN_HPP

#include <cstddef>
#include <cstdint>

#include <core/io/CAN.hpp>
#include <core/io/types/CANMessage.hpp>
#include <core/rtos/Initializable.hpp>
#include <core/rtos/Queue.hpp>

#define POWERTRAIN_QUEUE_SIZE 64

namespace io   = core::io;
namespace rtos = core::rtos;

namespace vcu::dev {

class PowertrainCAN : public rtos::Initializable {
public:
    /**
     * An enumeration of the Powertrain CAN message ids that are relevant to the MCuC and Hardmon.
     */
    enum MessageIDs : uint32_t {
        // motor controller receive message ids
        MC_TEMPS_1_ID            = 0xA0,
        MC_TEMPS_2_ID            = 0xA1,
        MC_TEMPS_3_ID            = 0xA2,
        MC_INTERNAL_STATES_ID    = 0x0AA,
        MC_FAULT_CODES_ID        = 0x0AB,
        MC_HIGH_SPEED_ID         = 0x0B0,
        MC_PARAMETER_RESPONSE_ID = 0x0C2,
        MC_INTERNAL_VOLTAGES_ID  = 0x0A9,

        // motor controller send message ids
        MC_COMMAND_ID           = 0x0C0,
        MC_PARAMETER_COMMAND_ID = 0x0C1,

        // HIB message
        HIB_MESSAGE_ID          = 0x0D0,

        // BMS message
        BMS_MESSAGE_ID          = 0x202,

        // Ground Fault message
        GFDB_MESSAGE_ID         = 0xA100100,

        // self test message ids
        HARDMON_SELF_TEST_MESSAGE_ID = 0x044, // TODO: This is not the correct ID, will not work!
        UC_SELF_TEST_MESSAGE_ID      = 0x045, // TODO: This is not the correct ID, will not work!
    };

    /**
     * Queue to store CAN messages.
     */
    rtos::Queue queue;

    /**
     * Constructor for a PowertrainCan instance.
     *
     * @param can[in] powertrain can driver
     */
    PowertrainCAN(io::CAN& can);

    /**
     * Parses the motor controller state from the motor controller internal state message
     *  (message with id = MC_INTERNAL_STATES_ID)
     *
     * @param message[in] a message from the motor controller describing its internal state
     * @return the state of the motor controller state machine
     */
    uint8_t parseMCState(io::CANMessage& message);

    /**
     * Parses the motor controller discharge machine state from the motor controller internal state message
     *  (message with id = MC_INTERNAL_STATES_ID)
     *
     * @param message[in] a message from the motor controller describing its internal state.
     * @return the state of the motor controller's discharger internal state machine.
     */
    uint8_t parseMCDischarge(io::CANMessage& message);

    /**
     * NOTE: EXAMPLE IMPLEMENTATION THAT MUST BE UPDATED
     * Parses the HIB throttle value from the HIB message.
     *  (message with id = HIB_MESSAGE_ID)
     *
     * @param message[in] a message from the HIB that contains throttle information.
     * @return the value of the throttle.
     */
    int16_t parseHIBThrottle(io::CANMessage& message);

    // TODO: Heller's point about moving all of this parsing to
    //  separate board classes that takes in a CAN message from that respective board and extracts the relevant data
    /**
     * NOTE: EXAMPLE IMPLEMENTATION THAT MUST BE UPDATED
     * Parses the HIB forward enable value from the HIB message.
     *  (message with id = HIB_MESSAGE_ID)
     *
     * @param message[in] a message from the HIB that contains forward enable information.
     * @return whether or not forward enable is on.
     */
    bool parseHIBForwardEnable(io::CANMessage& message);

    // TODO: All HIB parsing needs to be updated to match actual messages
    /**
     * NOTE: EXAMPLE IMPLEMENTATION THAT MUST BE UPDATED
     * Parses the HIB start pressed value from the HIB message.
     *  (message with id = HIB_MESSAGE_ID)
     *
     * @param message[in] a message from the HIB that contains start pressed information.
     * @return whether or not start is pressed
     */
    bool parseHIBStartPressed(io::CANMessage& message);


    /**
     * Sets the Motor Controller Command message values.
     * @param torque the value to set torque to.
     * @param speed the value to set speed to.
     * @param direction the value to set direction to.
     * @param inverterEn the value to set inverterEn to.
     * @param inverterDC the value to set interverDC to.
     * @param speedModeEn the value to set speedModeEn to.
     * @param rollingCounter the value to set rollingCounter to.
     * @param torqueLimit the value to set torqueLimit to.
     */
    void setMCAll(int16_t torque, int16_t speed, int16_t direction, bool inverterEn,
                  bool inverterDC, int16_t speedModeEn, int8_t rollingCounter, int16_t torqueLimit);

    /**
     * Sets the contactorCommand value for the BMS CAN message.
     * @param contactorCommand the value to set contactorCommand to.
     */
    void setBMSContactor(int16_t contactorCommand);

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
    io::CAN::CANStatus sendMCMessage();

    /**
     * Sends the BMS message.
     * NOTE: sending the message DOES NOT reset the contents of the message;
     *  i.e. calling this function twice will send two identical messages.
     */
    io::CAN::CANStatus sendBMSMessage();

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

    core::rtos::TXError init(rtos::BytePoolBase& pool) override;

private:
    /**
     * Struct that represents the structure of the Motor Controller Command Message.
     */
    struct MCCommandPayload {
        int16_t torque;
        int16_t speed;
        uint8_t direction;
        uint8_t inverterEnable    : 1;
        uint8_t inverterDischarge : 1;
        uint8_t speedModeEnable   : 1;
        uint8_t padding           : 1;
        uint8_t rollingCounter    : 4;
        int16_t CommandedTorqueLimit;
    } __attribute__((packed));

    /// Local instantiation of the command payload.
    MCCommandPayload mcCommandPayload = {0, 0, 1, 0, 0, 0, 0, 0, 0};

    /**
     * Struct that represents the structure of the BMS Output Message.
     */
    struct BMSPayload {
        int16_t contactorCommand;
    } __attribute__((packed));

    /// Local instantiation of the BMS payload.
    BMSPayload bmsPayload = {0};

    /// Can Driver
    io::CAN& can;

    /// Example payload for the UC selfTest Message.
    /// In the future, could be replaced by a more meaningful payload
    uint8_t UCSelfTestPayload = 4;
    /// the uc self test message
    io::CANMessage UCSelfTestMessage = io::CANMessage(UC_SELF_TEST_MESSAGE_ID, 1, &UCSelfTestPayload, false);

    /// Example payload for the Hardmon selfTest Response Message.
    /// In the future, could be replaced by a more meaningful payload
    uint8_t HardmonSelfTestResponsePayload = 3;
    /// the hardmon self test message
    io::CANMessage HardmonSelfTestResponse =
        io::CANMessage(HARDMON_SELF_TEST_MESSAGE_ID, 1, &HardmonSelfTestResponsePayload, false);
};

} // namespace vcu::dev

#endif // VCU_POWERTRAINCAN_HPP
