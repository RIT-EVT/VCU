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
        HIB_MESSAGE_ID = 0x0D0,

        // BMS message
        BMS_MESSAGE_ID = 0x202,

        // Ground Fault message
        GFDB_MESSAGE_ID = 0xA100100,

        // Early shutdown warning for the GUB message
        GUB_EARLY_SHUTDOWN_WARNING_ID = 0x0FF,  // it spells off :)

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
    void setMCAll(int16_t torque, int16_t speed, int16_t direction, bool inverterEn, bool inverterDC,
                  int16_t speedModeEn, int8_t rollingCounter, int16_t torqueLimit);

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
     *  Returns CAN status of the attempted send
     */
    io::CAN::CANStatus sendUCSelfTestMessage();

    /**
     * Sends a Shutdown Message to the GUB. This message is sent ~100ms before power will be cut!
     *  (Message is the same every time)
     *  Returns CAN status of the attempted send
     */
    io::CAN::CANStatus sendShutdownWarningMessage();

    /**
     * Sends the Hardmon Response to the UC Self Test Message.
     *  (Message is the same every time).
     *  Returns CAN status of the attempted send
     */
    io::CAN::CANStatus sendHardmonSelfTestResponse();

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

    /// Payload for gub to be warned about power about to turn off
    uint8_t GUBShutdownPayload = 255;
    /// the pre-shutdown warning message for the GUV
    io::CANMessage GUBShutdownWarningMessage = io::CANMessage(GUB_EARLY_SHUTDOWN_WARNING_ID, 1, &GUBShutdownPayload, false);

    /// Example payload for the Hardmon selfTest Response Message.
    /// In the future, could be replaced by a more meaningful payload
    uint8_t HardmonSelfTestResponsePayload = 3;
    /// the hardmon self test message
    io::CANMessage HardmonSelfTestResponse =
        io::CANMessage(HARDMON_SELF_TEST_MESSAGE_ID, 1, &HardmonSelfTestResponsePayload, false);
};

} // namespace vcu::dev

#endif // VCU_POWERTRAINCAN_HPP
