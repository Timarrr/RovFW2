#include "rovdatatypes.h"
#include <Arduino.h>
#include <cstdint>

class Debug {
  public:
    union debugFlags {
        int8_t flags = 0;
        bool depthSensor : 1;
        bool analogSensors : 1;
        bool imu : 1;
        bool thrusters : 1;
        bool manipulator : 1;
        bool cameras : 1;
        bool regulators : 1;
        bool unused : 1;
    };
    Debug() = delete;
    Debug(RovTelemetry *tele, RovControl *ctrl, RovAuxControl *auxCtrl)
        : tele(tele), ctrl(ctrl), auxCtrl(auxCtrl){};
    void setDebugFlags(debugFlags flags) { this->flags = flags; };
    void debugHandler();

  private:
    RovTelemetry *tele;
    RovControl *ctrl;
    RovAuxControl *auxCtrl;
    debugFlags flags;
};