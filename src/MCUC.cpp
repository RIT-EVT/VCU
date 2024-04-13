#include <MCUC.hpp>

namespace VCU {

MCUC::MCUC(VCU::MCUC::reqGPIO gpios) : gpios(gpios) {

}

CO_OBJ_T* MCUC::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t MCUC::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

uint8_t MCUC::getNodeID() {
    return NODE_ID;
}

void MCUC::handlePowertrainCanMessage(IO::CANMessage& message) {
    uint8_t* message_payload = message.getPayload();
    switch(message.getId()) {
    case MC_INTERNAL_STATES_ID:
        mcState = (message_payload[0]);
        mcDischarge = (message_payload[4] >> 5);
        break;
    case HIB_MESSAGE_ID:
        //TODO: HIB INCORRECT assumption of HIB message format, fix when HIB is finished
        throttle = (message_payload[0]);
        throttle << 8;
        throttle += (message_payload[1]);
        forwardEnable = (message_payload[2] & 0b10000000 ) != 0;
        startPressed  = (message_payload[2] & 0b01000000 ) != 0;
    default:
        //we don't care about this message lol
        break;
    }
}

void MCUC::process() {
    //brakeOn updated over CAN
    eStop = gpios.eStopGPIO.readPin() == IO::GPIO::State::HIGH;
    //forwardEnable, startPressed, mcStateMachine, discharge updated over CAN
    ignitionOn = gpios.ignitionGPIO.readPin() == IO::GPIO::State::HIGH;
    hmFault = gpios.hmFaultGPIO.readPin() == IO::GPIO::State::HIGH;
    //throttle updated over CAN
    lvssOn = gpios.lvssStatusGPIO.readPin() == IO::GPIO::State::HIGH;
    mcOn = gpios.mcStatusGPIO.readPin() == IO::GPIO::State::HIGH;

    //set the inputs and step the model

    //process the outputs
}

}
