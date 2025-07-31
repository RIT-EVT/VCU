#include <TrackMCuC.hpp>

TrackMCuC::TrackMCuC(io::GPIO& greenLed, io::GPIO& yellowLed, io::GPIO& redLed, io::GPIO& faultLed,
                     io::GPIO& superFaultLed, io::GPIO& canSelfTest, io::GPIO& mcSelfTest, io::GPIO& mcToggleP,
                     io::GPIO& mcToggleN, io::GPIO& lvssEnable, io::GPIO& estop, io::GPIO& ignition,
                     io::GPIO& interlock, io::CAN& ptCan) :
      greenLed(greenLed), yellowLed(yellowLed), redLed(redLed), faultLed(faultLed),
      superFaultLed(superFaultLed), canSelfTest(canSelfTest), mcSelfTest(mcSelfTest), mcToggleP(mcToggleP),
      mcToggleN(mcToggleN), lvssEnable(lvssEnable), estop(estop), ignition(ignition), interlock(interlock),
      ptCan(ptCan) {}

void TrackMCuC::process() {
    // TODO
}
