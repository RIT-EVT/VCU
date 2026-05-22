#ifndef VCU_BOARD_MSG_PARSER
#define VCU_BOARD_MSG_PARSER

#include <core/io/types/CANMessage.hpp>
#include <MCuC.hpp>

namespace io = core::io;

namespace boards {
struct BMSParsed {
    int32_t cellTemps[vcu::MCuC::BMS_CELL_TEMP_LEN];    ///< Array of Cell Temperatures
    int16_t cellVolts[vcu::MCuC::BMS_CELL_VOLT_LEN];    ///< Array of Cell Voltages
    bool contactorClosed;                               ///< Whether the contactor is closed or not
};

/**
 * Struct for important data to pull from HIB CAN messages
 */
struct HIBParsed {
    int16_t throttle;       ///< The throttle value
    bool forwardEn;         ///< Whether the forwardEn is on or not
    bool startPressed;      ///< Whether the start is pressed or not
    bool brakeOn;           ///< Whether the brake is on or not
    bool comparisonFault;   ///< Whether there is a comparison fault or not
};

/**
 * Struct for important data to pull from MC Internal State CAN messages
 */
struct MCInternalParsed {
    int16_t mcDischarge;    ///< The discharge value
    int16_t mcState;        ///< The state the motor controller is in
};

/**
 * Struct for important data to pull from GFDB CAN messages
 */
struct GFDBParsed {
    uint8_t isolationState; ///< The isolation state of the ground fault detection board
};

/**
 * Struct for important data to pull from Hardmon Self Test CAN messages
 */
struct HardmonParsed {
    bool powertrainCANSelfTest; ///<
};

/**
 * todo NOTE: EXAMPLE IMPLEMENTATION THAT MUST BE UPDATED
 * Parses the BMS values from the BMS CAN message.
 *  (message with id = BMS_MESSAGE_ID)
 *
 * @param message[in] a CAN message from the BMS.
 * @return the parsed BMS values.
 */
 BMSParsed parseBMSMessage(io::CANMessage& message);

/**
 * Parses the necessary HIB values from the HIB CAN message.
 *  (message with id = HIB_MESSAGE_ID)
 *
 * @param message[in] a CAN message from the HIB.
 * @return the parsed HIB values.
 */
HIBParsed parseHIBMessage(io::CANMessage& message);

/**
 * Parses the isolation state from the GFDB message.
 *  (message with id = GFDB_MESSAGE_ID && proper iso state msg identifier)
 *
 * @param message[in] a CAN message from the GFDB.
 * @return the parsed GFDB values.
 */
GFDBParsed parseGFDBMessage(io::CANMessage& message);

/**
 * Parses the motor controller internal information from the motor controller internal state message
 *  (message with id = MC_INTERNAL_STATES_ID)
 *
 * @param message[in] a message from the motor controller describing its internal state
 * @return the parsed information from the CAN message
 */
MCInternalParsed parseMCInternalMessage(io::CANMessage& message);

/**
 * todo NOTE: EXAMPLE IMPLEMENTATION THAT MUST BE UPDATED
 * Parses the necessary Hardmon values from the Hardmon message.
 *  (message with id = HARDMON_SELF_TEST_MESSAGE_ID)
 *
 * @param message[in] a CAN message from the Hardmon.
 * @return the parsed Hardmon values.
 */
HardmonParsed parseHardmonMessage(io::CANMessage& message);
}

#endif // VCU_BOARD_MSG_PARSER
