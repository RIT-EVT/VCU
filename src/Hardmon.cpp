#include <Hardmon.hpp>

namespace vcu {

Hardmon::Hardmon(reqGPIO gpios, io::CAN& ptCAN) : gpios(gpios), powertrainCAN(ptCAN) {
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
    //forwardEnable ahs been updated over CAN
    ignitionCheck = gpios.ignitionCheckGPIO.readPin() == io::GPIO::State::HIGH;
    ignition3v3 = gpios.ignition3V3GPIO.readPin() == io::GPIO::State::HIGH;
    lvssStatus = gpios.lvssStatus3V3GPIO.readPin() == io::GPIO::State::HIGH;
    mcStatus = gpios.mcStatusGPIO.readPin() == io::GPIO::State::HIGH;
    ucState[0] = gpios.ucStateZeroGPIO.readPin() == io::GPIO::State::HIGH;
    ucState[1] = gpios.ucStateOneGPIO.readPin() == io::GPIO::State::HIGH;
    ucState[2] = gpios.ucStateTwoGPIO.readPin() == io::GPIO::State::HIGH;
    ucState[3] = gpios.ucStateThreeGPIO.readPin() == io::GPIO::State::HIGH;
    eStopCheck = gpios.eStopCheckGPIO.readPin() == io::GPIO::State::HIGH;
    //discharge will be updated over CAN
    watchdog = gpios.watchdogGPIO.readPin() == io::GPIO::State::HIGH;
    eStop3v3 = gpios.eStop3V3GPIO.readPin() == io::GPIO::State::HIGH;
    // lvssEnableUC should be a pin, but electrical fucked it up and forgot to add it
    // so instead we are calculating it based on the microcontroller state. yay!
    // ucStates 1 through 5 should make this true
    // TODO: check with EEs if a fault should make this true or not
    // getting the state value from the array.
    uint8_t states = ucState[3];
    states <<= 1;
    states += ucState[2];
    states <<= 1;
    states += ucState[1];
    states <<= 1;
    states += ucState[0];
    lvssEnableUC = (states >= 1 && states <= 5);

    //step the model
    const Hardmon_Model::ExtU_Hardmon_Model_T modelInputs = {
        forwardEnable,
        ignitionCheck,
        ignition3v3,
        lvssStatus,
        mcStatus,
        {ucState[0],
         ucState[1],
         ucState[2],
         ucState[3]},
        eStopCheck,
        discharge,
        watchdog,
        eStop3v3,
        lvssEnableUC};

    model.setExternalInputs(&modelInputs);
    model.step();
    Hardmon_Model::ExtY_Hardmon_Model_T modelOutputs = model.getExternalOutputs();
    //save outputs
    mcSwitchEnable = modelOutputs.nMC_Switch_EN;
    lvssSwitchEnable = modelOutputs.nLVSS_Switch_EN;
    inverterDischarge = modelOutputs.Inverter_DIS_CAN;
    mcToggleNeg = modelOutputs.MC_TOG_N_HM;
    mcTogglePos = modelOutputs.MC_TOG_P_HM;
    ucReset = modelOutputs.nReset;
    lvssEnableHardMon = modelOutputs.LVSS_EN_HM;
    hmFault = modelOutputs.HM_Fault;

    //use outputs
    gpios.mcToggleOverrideGPIO.writePin(mcSwitchEnable ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.lvssEnableOverrideGPIO.writePin(lvssSwitchEnable ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    //inverter discharge will be handled over CAN
    gpios.mcToggleNegativeGPIO.writePin(mcToggleNeg ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.mcTogglePositiveGPIO.writePin(mcTogglePos ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucResetGPIO.writePin(ucReset ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.lvssEnableHardmonGPIO.writePin(lvssEnableHardMon ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.hmFaultGPIO.writePin(hmFault ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

    //TODO: right now the message sets all values but inverter discharge to be 0. This might be REALLY BAD,
    // discuss it more with the EES and maybe Matt.
    if (inverterDischarge) {
        powertrainCAN.setMCInverterDischarge(true);
        powertrainCAN.sendMCMessage();
    }
}

}// namespace VCU