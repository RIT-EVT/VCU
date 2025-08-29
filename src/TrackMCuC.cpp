#include <TrackMCuC.hpp>

#include <core/utils/time.hpp>

TrackMCuC::TrackMCuC(io::GPIO& greenLed, io::GPIO& yellowLed, io::GPIO& redLed, io::GPIO& faultLed,
                     io::GPIO& superFaultLed, io::GPIO& canSelfTest, io::GPIO& mcSelfTest, io::GPIO& mcToggleP,
                     io::GPIO& mcToggleN, io::GPIO& lvssEnable, io::GPIO& estop, io::GPIO& ignition,
                     io::GPIO& interlock, io::CAN& accCan, io::CAN& pwtCan) :
      greenLed(greenLed), yellowLed(yellowLed), redLed(redLed), faultLed(faultLed),
      superFaultLed(superFaultLed), canSelfTest(canSelfTest), mcSelfTest(mcSelfTest), mcToggleP(mcToggleP),
      mcToggleN(mcToggleN), lvssEnable(lvssEnable), estop(estop), ignition(ignition), interlock(interlock),
      accCan(accCan), pwtCan(pwtCan) {
    // Initialize all output GPIOs to low
    greenLed.writePin(io::GPIO::State::LOW);
    yellowLed.writePin(io::GPIO::State::LOW);
    redLed.writePin(io::GPIO::State::LOW);
    faultLed.writePin(io::GPIO::State::LOW);
    superFaultLed.writePin(io::GPIO::State::LOW);
    canSelfTest.writePin(io::GPIO::State::LOW);
    mcSelfTest.writePin(io::GPIO::State::LOW);
    mcToggleP.writePin(io::GPIO::State::LOW);
    mcToggleN.writePin(io::GPIO::State::LOW);
    lvssEnable.writePin(io::GPIO::State::LOW);
}

void TrackMCuC::process() {
    if (
        highestCellTemp > CELL_TEMP_MAX ||
        lowestCellVoltage < CELL_VOLT_MIN ||
        bmsMasterTemp > BMS_TEMP_MAX
    ) {
        // If the red LED is on at the same time as the green or blue LED, this is meant to indicate a warning
        // This will give the rider time to stop the bike before something actually bad happens
        redLed.writePin(LED_ON);
    }

    switch (state) {
    case State::START:
        startState();
        break;
    case State::MC_OFF:
        mcOffState();
        break;
    case State::MC_ACTIVE:
        mcActiveState();
        break;
    case State::MC_DISCHARGING:
        mcDischargingState();
        break;
    case State::ESTOP:
        estopState();
        break;
    case State::FAULT:
        faultState();
        break;
    }
}

void TrackMCuC::receiveAccMessage(io::CANMessage& message) {
    // No messages yet
}

void TrackMCuC::receivePwtMessage(io::CANMessage& message) {
    uint8_t* payload = message.getPayload();
    switch (message.getId()) {
    case 0x2D0A:
        highestCellTemp = payload[6];
        lowestCellVoltage = (((uint16_t) payload[0]) << 8) + payload[1];
        break;
    case 0x2C0A:
        bmsMasterTemp = (((uint16_t) payload[0]) << 8) + payload[1];
        break;
    default:
        // Do nothing
        break;
    }

}

void TrackMCuC::startState() {
    if (stateChanged) {
        // TODO: Checks on startup
        if (estop.readPin() == ESTOP_ACTIVE) {
            state = State::ESTOP;
        } else {
            state = State::MC_OFF;
        }

        // Don't need to change stateChanged because it's already true
        //stateChanged = true;
    } else {
        // Should never get here
        state = State::FAULT;
        stateChanged = true;
    }
}

void TrackMCuC::mcOffState() {
    if (stateChanged) {
        yellowLed.writePin(LED_ON);
        stateChanged = false;
    }

    if (estop.readPin() == ESTOP_ACTIVE) {
        state = State::ESTOP;
        yellowLed.writePin(LED_OFF);
        stateChanged = true;
    } else if (ignition.readPin() == IGNITION_ACTIVE) {
        state = State::MC_ACTIVE;
        yellowLed.writePin(LED_OFF);
        stateChanged = true;
    }
}

void TrackMCuC::mcActiveState() {
    if (stateChanged) {
        greenLed.writePin(LED_ON);

        // Enable LVSS
        lvssEnable.writePin(io::GPIO::State::HIGH);
        uint8_t buf[] = {0xFF, 0xFF};
        io::CANMessage txMessage(0x180, 2, buf, false);
        core::time::wait(70);
        accCan.transmit(txMessage);

        // Enable MC
        mcToggleP.writePin(io::GPIO::State::HIGH);
        core::time::wait(100);
        mcToggleP.writePin(io::GPIO::State::LOW);

        stateChanged = false;
    }

    if (estop.readPin() == ESTOP_ACTIVE || ignition.readPin() != IGNITION_ACTIVE) {
        state = State::MC_DISCHARGING;
        lvssEnable.writePin(io::GPIO::State::LOW);
        greenLed.writePin(LED_OFF);
        stateChanged = true;
    }
}

void TrackMCuC::mcDischargingState() {
    if (stateChanged) {
        // TODO: Tell MC to discharge

        // Turn MC off
        mcToggleN.writePin(io::GPIO::State::HIGH);
        core::time::wait(100);
        mcToggleN.writePin(io::GPIO::State::LOW);

        if (estop.readPin() == ESTOP_ACTIVE) {
            state = State::ESTOP;
        } else {
            state = State::MC_OFF;
        }
        // Don't need to change stateChanged because it's already true
        //stateChanged = true;
    } else {
        // Should never get here
        state = State::FAULT;
        stateChanged = true;
    }
}

void TrackMCuC::estopState() {
    if (stateChanged) {
        redLed.writePin(LED_ON);
        stateChanged = false;
    }

    if (estop.readPin() != ESTOP_ACTIVE && ignition.readPin() != IGNITION_ACTIVE) {
        state = State::MC_OFF;
        redLed.writePin(LED_OFF);
        stateChanged = true;
    }
}
void TrackMCuC::faultState() {
    if (stateChanged) {
        redLed.writePin(LED_ON);
        faultLed.writePin(LED_ON);
        superFaultLed.writePin(LED_ON);
        stateChanged = false;
    }
}
