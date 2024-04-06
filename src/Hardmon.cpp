#include <Hardmon.hpp>

namespace VCU {

Hardmon::Hardmon(reqGPIO gpios) : gpios(gpios) {

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

void Hardmon::process() {
    //update inputs
    ignitionCheck = gpios.ignitionCheckGPIO.readPin() == IO::GPIO::State::HIGH;
    ignition3v3 = gpios.ignition3V3GPIO.readPin() == IO::GPIO::State::HIGH;
    lvssStatus = gpios.lvssStatus3V3GPIO.readPin() == IO::GPIO::State::HIGH;
    mcStatus = gpios.mcStatusGPIO.readPin() == IO::GPIO::State::HIGH;
    ucState[0] = gpios.ucStateZeroGPIO.readPin() == IO::GPIO::State::HIGH;
    ucState[1] = gpios.ucStateOneGPIO.readPin() == IO::GPIO::State::HIGH;
    ucState[2] = gpios.ucStateTwoGPIO.readPin() == IO::GPIO::State::HIGH;
    ucState[3] = gpios.ucStateThreeGPIO.readPin() == IO::GPIO::State::HIGH;
    eStopCheck = gpios.eStopCheckGPIO.readPin() == IO::GPIO::State::HIGH;
    //discharge has been updated via canOpen
    watchdog = gpios.watchdogGPIO.readPin() == IO::GPIO::State::HIGH;
    eStop3v3 = gpios.eStop3V3GPIO.readPin() == IO::GPIO::State::HIGH;
    //TODO lvssEnableUC is read from the ucState


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
        lvssEnableUC
    };

    model.setExternalInputs(&modelInputs);
    model.step();
    Hardmon_Model::ExtY_Hardmon_Model_T modelOutputs = model.getExternalOutputs();
    //save outputs
    mcSwitchEnable = modelOutputs.nMC_Switch_EN;
    lvssSwitchEnable = modelOutputs.nLVSS_Switch_EN;
    InverterDischarge = modelOutputs.Inverter_DIS_CAN;
    mcToggleNeg = modelOutputs.MC_TOG_N_HM;
    mcTogglePos = modelOutputs.MC_TOG_P_HM;
    ucReset = modelOutputs.nReset;
    lvssEnableHardMon = modelOutputs.LVSS_EN_HM;
    hmFault = modelOutputs.HM_Fault;

    //use outputs
    //TODO check with electrical to make sure these are correct true/false to HIGH/LOW mappings
    gpios.mcToggleOverrideGPIO.writePin( mcSwitchEnable ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW );
    gpios.lvssEnableOverrideGPIO.writePin( lvssSwitchEnable ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW );
    //inverter discharge will be handled over CAN
    gpios.mcToggleNegativeGPIO.writePin( mcToggleNeg ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW );
    gpios.mcTogglePositiveGPIO.writePin( mcTogglePos ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW );
    gpios.ucResetGPIO.writePin( ucReset ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW );
    gpios.lvssEnableHardmonGPIO.writePin( lvssEnableHardMon ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW );
    gpios.hmFaultGPIO.writePin( hmFault ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW );
}

}