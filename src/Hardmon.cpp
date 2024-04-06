#include <Hardmon.hpp>

namespace VCU {

Hardmon::Hardmon() {

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

    //step the model
    const Hardmon_Model::ExtU_Hardmon_Model_T modelInputs = {
        forwardEnable,
        ignition12V0,
        ignition3v3,
        lvssStatus,
        MCStatus,
        {ucState[0],
        ucState[1],
        ucState[2],
        ucState[3]},
        eStop12V0,
        discharge,
        watchdog,
        eStop3v3,
        LVSSEnableuC};

    model.setExternalInputs(&modelInputs);
    model.step();
    Hardmon_Model::ExtY_Hardmon_Model_T modelOutputs = model.getExternalOutputs();
    //use outputs
    MCSwitchEnable = modelOutputs.nMC_Switch_EN;
    LVSSSwitchEnable = modelOutputs.nLVSS_Switch_EN;
    InverterDischarge = modelOutputs.Inverter_DIS_CAN;
    MCToggleNeg = modelOutputs.MC_TOG_N_HM;
    MCTogglePos = modelOutputs.MC_TOG_P_HM;
    reset = modelOutputs.nReset;
    LVSSEnableHardMon = modelOutputs.LVSS_EN_HM;
    HMFault = modelOutputs.HM_Fault;

}

}