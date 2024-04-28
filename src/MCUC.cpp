#include <MCUC.hpp>

namespace VCU {

MCUC::MCUC(VCU::MCUC::reqGPIO gpios, IO::CAN& can) : gpios(gpios), powertrainCAN(can) {
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
    switch (message.getId()) {
    case DEV::PowertrainCAN::MC_INTERNAL_STATES_ID:
        mcState = powertrainCAN.parseMCState(message);
        mcDischarge = powertrainCAN.parseMCDischarge(message);
        break;
    case DEV::PowertrainCAN::HIB_MESSAGE_ID:
        throttle = powertrainCAN.parseHIBThrottle(message);
        forwardEnable = powertrainCAN.parseHIBForwardEnable(message);
        startPressed = powertrainCAN.parseHIBStartPressed(message);
        break;
    case DEV::PowertrainCAN::HARDMON_SELF_TEST_MESSAGE_ID:
        powertrainCANSelfTestIn = true;
        break;
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
    VCU::MCuC_Model::ExtU_MCuC_Model_T inputs = {
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
        powertrainCANSelfTestIn,
        accessoryCANSelfTestIn};
    model.setExternalInputs(&inputs);

    model.step();

    //get outputs
    VCU::MCuC_Model::ExtY_MCuC_Model_T outputs = model.getExternalOutputs();
    //save outputs
    lvssEnable = outputs.LVSS_EN_uC;
    inverterEnable = outputs.Inverter_EN_CAN;
    ucFault = outputs.Fault;
    watchdog = outputs.Watchdog;
    ucState[0] = outputs.uC_State & 0b00000001;
    ucState[1] = outputs.uC_State >> 1 & 0b00000001;
    ucState[2] = outputs.uC_State >> 2 & 0b00000001;
    ucState[3] = outputs.uC_State >> 3 & 0b00000001;
    inverterDischarge = outputs.Inverter_DIS_CAN;
    mcTogglePositive = outputs.MC_TOGp_uC;
    mcToggleNegative = outputs.MC_TOGn_uC;
    torqueRequest = outputs.Torque_Request;
    mcSelfTestOut = outputs.MC_Self_Test;
    estopSelfTestOut = outputs.ESTOP_Self_Test;
    ignitionSelfTestOut = outputs.Ignition_Self_Test1;
    accessoryCanSelfTestOut = outputs.CAN_A_out;
    powertrainCanSelfTestOut = outputs.MC_CAN_out;

    //use outputs
    gpios.lvssEnableGPIO.writePin(lvssEnable ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    //set inverterEnable before we send the message
    gpios.ucFaultGPIO.writePin(ucFault ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.watchdogGPIO.writePin(watchdog ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.ucStateZeroGPIO.writePin(ucState[0] ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.ucStateOneGPIO.writePin(ucState[1] ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.ucStateTwoGPIO.writePin(ucState[2] ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    gpios.ucStateThreeGPIO.writePin(ucState[3] ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);
    //set inverterDisable before we send the message
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
    bool canSelfTest = ucState[3] && !ucState[2] && ucState[1] && !ucState[0];
    gpios.canSelfTestGPIO.writePin(canSelfTest ? IO::GPIO::State::HIGH : IO::GPIO::State::LOW);

    //Send the Motor Controller CAN message (set values first)
    powertrainCAN.setMCInverterEnable(inverterEnable);
    powertrainCAN.setMCInverterDischarge(inverterDischarge);
    powertrainCAN.setMCTorque(torqueRequest);
    powertrainCAN.sendMCMessage();
}

}// namespace VCU
