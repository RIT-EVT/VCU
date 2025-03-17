#include <MCuC.hpp>

namespace vcu {

MCuC::MCuC(vcu::MCuC::MCuC_GPIO gpios, io::CAN& can) : powertrainCAN(can), gpios(gpios) {
}

CO_OBJ_T* MCuC::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t MCuC::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

uint8_t MCuC::getNodeID() {
    return NODE_ID;
}

void MCuC::handlePowertrainCanMessage(io::CANMessage& message) {
    switch (message.getId()) {
    case dev::PowertrainCAN::MC_INTERNAL_STATES_ID:
        mcState = static_cast<MC_VSM_State>(powertrainCAN.parseMCState(message));
        mcDischarge = static_cast<MC_DC_State>(powertrainCAN.parseMCDischarge(message));
        break;
    case dev::PowertrainCAN::HIB_MESSAGE_ID:
        throttle = powertrainCAN.parseHIBThrottle(message);
        forwardEnable = powertrainCAN.parseHIBForwardEnable(message);
        startPressed = powertrainCAN.parseHIBStartPressed(message);
        break;
    case dev::PowertrainCAN::HARDMON_SELF_TEST_MESSAGE_ID:
        powertrainCANSelfTestIn = true;
        break;
    default:
        //do nothing, we don't care about this message
        break;
    }
}

rtos::Queue* MCuC::getPowertrainQueue() {
    return &powertrainCAN.queue;
}

void MCuC::process() {
    //brakeOn updated over CAN
    eStop = gpios.eStopGPIO.readPin() == io::GPIO::State::HIGH;
    //forwardEnable, startPressed, mcStateMachine, discharge updated over CAN
    ignitionOn = gpios.ignitionGPIO.readPin() == io::GPIO::State::HIGH;
    hmFault = gpios.hmFaultGPIO.readPin() == io::GPIO::State::HIGH;
    //throttle updated over CAN
    lvssOn = gpios.lvssStatusGPIO.readPin() == io::GPIO::State::HIGH;
    mcOn = gpios.mcStatusGPIO.readPin() == io::GPIO::State::HIGH;

    //set the inputs and step the model
    vcu::MCuC_Model::ExtU_MCuC_T inputs = {
        ignitionOn,
        startPressed,
        brakeOn,
        eStop,
        forwardEnable,
        mcState,
        mcDischarge,
        false,
        hmFault,
        throttle,
        lvssOn,
        mcOn,
    };
    model.setExternalInputs(&inputs);

    model.step();

    //TODO: in the future when the model is reworked so the inputs and outputs are in separate blocks of CAN & GPIO inputs
    // we can use unions for this and iterate through it.
    //get outputs
    vcu::MCuC_Model::ExtY_MCuC_T outputs = model.getExternalOutputs();
    //save outputs
    lvssEnable = outputs.LVSS_EN_uC;
    inverterEnable = outputs.Inverter_EN;
    ucFault = outputs.Fault;
    watchdog = outputs.Watchdog;
    ucState.stateEnum = outputs.uC_State;
    inverterDischarge = outputs.Inverter_DIS;
    mcEnableUC = outputs.MC_EN_uC;
    torqueRequest = outputs.Torque_Request;
    mcSelfTestOut = outputs.MC_Self_Test;

    //use outputs
    gpios.lvssEnableGPIO.writePin(lvssEnable ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    //set inverterEnable before we send the message
    gpios.ucFaultGPIO.writePin(ucFault ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.watchdogGPIO.writePin(watchdog ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateZeroGPIO.writePin(ucState.stateBits[0] ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateOneGPIO.writePin(ucState.stateBits[1] ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateTwoGPIO.writePin(ucState.stateBits[2] ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ucStateThreeGPIO.writePin(ucState.stateBits[3] ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    //set inverterDisable before we send the message
    // set Motor Controller via the two gpios
    // If mcEnableUC is true, Positive should be High and Negative should be Low, otherwise they should be the opposite.
    gpios.mcTogglePositiveGPIO.writePin(mcEnableUC ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.mcToggleNegativeGPIO.writePin(mcEnableUC ? io::GPIO::State::LOW : io::GPIO::State::HIGH);
    //set torqueRequest before we send the message
    gpios.mcSelfTestGPIO.writePin(mcSelfTestOut ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.estopSelfTestGPIO.writePin(estopSelfTestOut ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    gpios.ignitionSelfTestGPIO.writePin(ignitionSelfTestOut ? io::GPIO::State::HIGH : io::GPIO::State::LOW);
    //We will send accessory CAN SelfTest message over CANopen
    //Send the powertrainCanSelfTest message
    if (powertrainCanSelfTestOut) {
        powertrainCAN.sendUCSelfTestMessage();
    }

    //setting the CAN self test: only true when we are in ucState 10 (self test state)
    //ucState should be 0b1010, we are reading each bit individually cause that's what we get them as.
    //TODO: unknown if this self test is true or not yet
    bool canSelfTest = ucState.stateEnum == UC_State::LVSS_MC_Shutdown;
    gpios.canSelfTestGPIO.writePin(canSelfTest ? io::GPIO::State::HIGH : io::GPIO::State::LOW);

    //Send the Motor Controller CAN message (set values first)
    powertrainCAN.setMCInverterEnable(inverterEnable);
    powertrainCAN.setMCInverterDischarge(inverterDischarge);
    powertrainCAN.setMCTorque(torqueRequest);
    powertrainCAN.sendMCMessage();
}

}// namespace vcu
