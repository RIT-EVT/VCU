#include <Hardmon.hpp>

namespace vcu {

Hardmon::Hardmon(HardmonGPIO gpio, io::CAN& ptCAN) : powertrainCAN(ptCAN), gpios(gpio) {
    model.initialize();
}

CO_OBJ_T* Hardmon::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t Hardmon::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

uint8_t Hardmon::getNodeID() {
    return NODE_ID;
}

void Hardmon::handlePowertrainCanMessage(io::CANMessage& message) {
    switch (message.getId()) {
    case dev::PowertrainCAN::HIB_MESSAGE_ID:
        forwardEnable = powertrainCAN.parseHIBForwardEnable(message);
        break;
    case dev::PowertrainCAN::UC_SELF_TEST_MESSAGE_ID:
        powertrainCAN.sendHardmonSelfTestResponse();
        break;
    default:
        //we don't care about this message lol
        break;
    }
}

core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, io::CANMessage>* Hardmon::getPowertrainQueue() {
    return &powertrainCAN.queue;
}

void Hardmon::process() {
    //handle all the powertrain CAN messages
    io::CANMessage message;
    while (!powertrainCAN.queue.isEmpty()) {
        powertrainCAN.queue.pop(&message);
        handlePowertrainCanMessage(message);
    }

    //update inputs
    //forwardEnable has been updated over CAN
    //update the gpio inputs in a loop using the unions
    for (int i = 0; i < 11; i++) {
        modelGPIOInputs.arr[i] = gpios.inputArr[i]->readPin() == io::GPIO::State::HIGH;
    }
    // lvssEnableUC should be a pin, but electrical forgot to add it
    // so instead we are calculating it based on the microcontroller state.
    // ucStates 1 through 5 should make this true
    // TODO: check with EEs if a fault should make this true or not
    // getting the state value from the array.
    uint8_t states = modelGPIOInputs.ucState[3];
    states <<= 1;
    states += modelGPIOInputs.ucState[2];
    states <<= 1;
    states += modelGPIOInputs.ucState[1];
    states <<= 1;
    states += modelGPIOInputs.ucState[0];
    lvssEnableUC = (states >= 1 && states <= 5);

    //step the model
    const Hardmon_Model::ExtU_Hardmon_Model_T modelInputs = {
        forwardEnable,
        modelGPIOInputs.ignitionCheck,
        modelGPIOInputs.ignition3v3,
        modelGPIOInputs.lvssStatus,
        modelGPIOInputs.mcStatus,
        {modelGPIOInputs.ucState[0],
         modelGPIOInputs.ucState[1],
         modelGPIOInputs.ucState[2],
         modelGPIOInputs.ucState[3]},
        modelGPIOInputs.eStopCheck,
        discharge,
        modelGPIOInputs.watchdog,
        modelGPIOInputs.eStop3v3,
        lvssEnableUC};

    model.setExternalInputs(&modelInputs);
    model.step();
    modelOutputs.modelOutputStruct = model.getExternalOutputs();

    //use outputs
    gpios.mcToggleOverrideGPIO.writePin(modelOutputs.mcSwitchEnable ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.lvssEnableOverrideGPIO.writePin(modelOutputs.lvssSwitchEnable ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    //inverter discharge will be handled over CAN
    gpios.mcToggleNegativeGPIO.writePin(modelOutputs.mcToggleNeg ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.mcTogglePositiveGPIO.writePin(modelOutputs.mcTogglePos ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucResetGPIO.writePin(modelOutputs.ucReset ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.lvssEnableHardmonGPIO.writePin(modelOutputs.lvssEnableHardMon ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.hmFaultGPIO.writePin(modelOutputs.hmFault ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

    //TODO: right now the message sets all values but inverter discharge to be 0. This might be REALLY BAD,
    // discuss it more with the EES and maybe Matt. Also make sure that this is okay to send in terms of
    // determining if the MCuC is untrustworthy.
    if (modelOutputs.inverterDischarge) {
        powertrainCAN.setMCInverterDischarge(true);
        powertrainCAN.sendMCMessage();
    }
}

}// namespace vcu