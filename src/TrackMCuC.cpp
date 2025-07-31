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

void TrackMCuC::startState() {}
void TrackMCuC::mcOffState() {}
void TrackMCuC::mcActiveState() {}
void TrackMCuC::mcDischargingState() {}
void TrackMCuC::estopState() {}
void TrackMCuC::faultState() {}
