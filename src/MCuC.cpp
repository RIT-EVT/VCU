#include <MCuC.hpp>

namespace vcu {

MCuC::MCuC(vcu::MCuC::MCuC_GPIO gpios, IO::CAN& can) : powertrainCAN(can), gpios(gpios) {
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

void MCuC::handlePowertrainCanMessage(IO::CANMessage& message) {
    switch (message.getId()) {
    case dev::PowertrainCAN::MC_INTERNAL_STATES_ID:
        mcState = powertrainCAN.parseMCState(message);
        mcDischarge = powertrainCAN.parseMCDischarge(message);
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

core::types::FixedQueue<POWERTRAIN_QUEUE_SIZE, IO::CANMessage>* MCuC::getPowertrainQueue() {
    return &powertrainCAN.queue;
}

void MCuC::process() {
    //handle all the powertrain CAN messages
    IO::CANMessage message;
    while (!powertrainCAN.queue.isEmpty()) {
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
    gpios.lvssEnableGPIO.writePin(lvssEnable ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    //set inverterEnable before we send the message
    gpios.ucFaultGPIO.writePin(ucFault ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.watchdogGPIO.writePin(watchdog ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.ucStateZeroGPIO.writePin(ucState.stateBits[0] ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.ucStateOneGPIO.writePin(ucState.stateBits[1] ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.ucStateTwoGPIO.writePin(ucState.stateBits[2] ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.ucStateThreeGPIO.writePin(ucState.stateBits[3] ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    //set inverterDisable before we send the message
    // set Motor Controller via the two gpios
    //TODO: this should be handled with a latching relay driver- need to make one.
    gpios.mcTogglePositiveGPIO.writePin(mcTogglePositive ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.mcToggleNegativeGPIO.writePin(mcToggleNegative ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    //set torqueRequest before we send the message
    gpios.mcSelfTestGPIO.writePin(mcSelfTestOut ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.estopSelfTestGPIO.writePin(estopSelfTestOut ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.ignitionSelfTestGPIO.writePin(ignitionSelfTestOut ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    //We will send accessory CAN SelfTest message over CANopen
    //Send the powertrainCanSelfTest message
    if (powertrainCanSelfTestOut) {
        powertrainCAN.sendUCSelfTestMessage();
    }

    //setting the CAN self test: only true when we are in ucState 10 (self test state)
    //ucState should be 0b1010, we are reading each bit individually cause that's what we get them as.
    //TODO: unknown if this self test is true or not yet
    bool canSelfTest = ucState.stateEnum == UC_State::LVSS_MC_Shutdown;
    gpios.canSelfTestGPIO.writePin(canSelfTest ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);

    //Send the Motor Controller CAN message (set values first)
    powertrainCAN.setMCInverterEnable(inverterEnable);
    powertrainCAN.setMCInverterDischarge(inverterDischarge);
    powertrainCAN.setMCTorque(torqueRequest);
    powertrainCAN.sendMCMessage();
}

}// namespace vcu
