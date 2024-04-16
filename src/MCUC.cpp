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
    switch(message.getId()) {
    case DEV::PowertrainCAN::MC_INTERNAL_STATES_ID:
        mcState = powertrainCAN.parseMCState(message);
        mcDischarge = powertrainCAN.parseMCDischarge(message);
        break;
    case DEV::PowertrainCAN::HIB_MESSAGE_ID:
        throttle = powertrainCAN.parseHIBThrottle(message);
        forwardEnable = powertrainCAN.parseHIBForwardEnable(message);
        startPressed  = powertrainCAN.parseHIBStartPressed(message);
    default:
        //do nothing, we don't care about this message
        break;
    }
}

EVT::core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage>* MCUC::getPowertrainQueue() {
    return &powertrainCAN.queue;
}

void MCUC::process() {
    //handle all the powertrain CAN messages
    IO::CANMessage message;
    while(!powertrainCAN.queue.isEmpty()) {
        powertrainCAN.queue.pop(&message);
        handlePowertrainCanMessage(message);
    }

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
