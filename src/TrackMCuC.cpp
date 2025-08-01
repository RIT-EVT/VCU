#include <TrackMCuC.hpp>

TrackMCuC::TrackMCuC(io::GPIO& greenLed, io::GPIO& yellowLed, io::GPIO& redLed, io::GPIO& faultLed,
                     io::GPIO& superFaultLed, io::GPIO& canSelfTest, io::GPIO& mcSelfTest, io::GPIO& mcToggleP,
                     io::GPIO& mcToggleN, io::GPIO& lvssEnable, io::GPIO& estop, io::GPIO& ignition,
                     io::GPIO& interlock, io::CAN& ptCan) :
      greenLed(greenLed), yellowLed(yellowLed), redLed(redLed), faultLed(faultLed),
      superFaultLed(superFaultLed), canSelfTest(canSelfTest), mcSelfTest(mcSelfTest), mcToggleP(mcToggleP),
      mcToggleN(mcToggleN), lvssEnable(lvssEnable), estop(estop), ignition(ignition), interlock(interlock),
      ptCan(ptCan) {
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

        // Enable MC and LVSS
        //lvssEnable.writePin(io::GPIO::State::HIGH); TODO: Actually enable the LVSS
        // TODO: Also tell  LVSS to turn on
        mcToggleP.writePin(io::GPIO::State::HIGH);
        mcToggleN.writePin(io::GPIO::State::LOW);

        stateChanged = false;
    }

    if (estop.readPin() == ESTOP_ACTIVE || ignition.readPin() != IGNITION_ACTIVE) {
        state = State::MC_DISCHARGING;
        // TODO: Turn off LVSS
        greenLed.writePin(LED_OFF);
        stateChanged = true;
    }
}

void TrackMCuC::mcDischargingState() {
    if (stateChanged) {
        // TODO: Tell MC to discharge

        // Turn MC off
        mcToggleP.writePin(io::GPIO::State::LOW);
        mcToggleN.writePin(io::GPIO::State::HIGH);

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
