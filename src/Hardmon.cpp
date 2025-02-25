#include <Hardmon.hpp>

#include <core/rtos/Enums.hpp>

namespace vcu {

Hardmon::Hardmon(HardmonGPIO gpio, io::CAN& ptCAN) : powertrainCAN(ptCAN), gpios(gpio),
                                                     mutex((char*)"Hardmon Mutex", true) {
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
        mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
        forwardEnable = powertrainCAN.parseHIBForwardEnable(message);
        mutex.put(rtos::TXWait::TXW_WAIT_FOREVER);
        break;
    case dev::PowertrainCAN::UC_SELF_TEST_MESSAGE_ID:
        mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
        powertrainCAN.sendHardmonSelfTestResponse();
        mutex.put(rtos::TXWait::TXW_WAIT_FOREVER);
        break;
    default:
        //we don't care about this message lol
        break;
    }
}

rtos::Queue* Hardmon::getPowertrainQueue() {
    return &powertrainCAN.queue;
}

void Hardmon::process() {
    mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);
    //update inputs
    //forwardEnable has been updated over CAN
    //update the gpio inputs in a loop using the unions
    for (int i = 0; i < 11; i++) {
        modelGPIOInputs.arr[i] = gpios.inputArr[i]->readPin() == io::GPIO::State::HIGH;
    }
    // lvssEnableUC should be a pin, but electrical forgot to add it
    // so instead we are calculating it based on the microcontroller state.
    // ucStates 1 through 5 should make this true
    // getting the state value from the array.
    uint16_t state = modelGPIOInputs.ucState[0];
    state <<= 1;
    state += modelGPIOInputs.ucState[1];
    state <<= 1;
    state += modelGPIOInputs.ucState[2];
    state <<= 1;
    state += modelGPIOInputs.ucState[3];
    lvssEnableUC = (state >= 1 && state <= 5);
    ucState = static_cast<UC_State>(state);

    //step the model
    const Hardmon_Model::ExtU_Hardmon_T modelInputs = {
        forwardEnable,
        modelGPIOInputs.ignitionCheck,
        modelGPIOInputs.ignition3v3,
        modelGPIOInputs.lvssStatus,
        modelGPIOInputs.mcStatus,
        ucState,
        modelGPIOInputs.eStopCheck,
        discharge,
        modelGPIOInputs.watchdog,
        modelGPIOInputs.eStop3v3,
        lvssEnableUC};

    model.setExternalInputs(&modelInputs);
    model.step();

    mutex.get(rtos::TXWait::TXW_WAIT_FOREVER);

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

rtos::TXError Hardmon::init(rtos::BytePoolBase& pool) {
    rtos::TXError status = mutex.init(pool);
    if (status != rtos::TXError::TXE_SUCCESS) {
        //we fucked up on initializing mutex.
        return status;
    } else {
        return powertrainCAN.init(pool);
    }
}

}// namespace vcu