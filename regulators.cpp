#include "regulators.h"
#include "api/Common.h"
#include "config.h"

PDRegulator::PDRegulator(int kP, int kD)
    : kP(kP), kD(kD), lastErr(0), lastTime(0) {}

int PDRegulator::eval(int data, int target) {
    int err = target - data;
    int u =
        kP * (err + kD * ((lastErr - err) /
                          (lastTime - millis()))); // kP(error + kD(dError/dt))
    lastErr = err;
    lastTime = millis();
    return u;
}

float FPPDRegulator::eval(float data, float target) {
    float err = target - data;
    float u =
        kP * (err + kD * ((lastError - err) /
                          (lastTime - millis()))); // kP(error + kD(dError/dt))
    lastError = err;
    lastTime = millis();
    return u;
}

RovRegulators::RovRegulators()
    : depthReg(config::regulators::depth_p, config::regulators::depth_d),
      yawReg(config::regulators::yaw_p, config::regulators::yaw_d),
      rollReg(config::regulators::roll_p, config::regulators::roll_d),
      pitchReg(config::regulators::pitch_p, config::regulators::pitch_d) {}

void RovRegulators::evaluate(RovControl &ctrl, RovAuxControl &auxCtrl,
                             RovTelemetry &tele) {
    int uD = 0, uY = 0, uR = 0, uP = 0;
    if (auxCtrl.auxFlags.eDepth)
        depthReg.eval(tele.depth * 100, auxCtrl.dDepth);
    if (auxCtrl.auxFlags.eYaw)
        yawReg.eval(tele.yaw * 100, auxCtrl.dYaw * 100);
    if (auxCtrl.auxFlags.eRoll)
        rollReg.eval(tele.roll * 100, auxCtrl.dRoll * 100);
    if (auxCtrl.auxFlags.ePitch)
        pitchReg.eval(tele.pitch * 100, auxCtrl.dPitch * 100);

    ctrl.thrusterPower[0] -= uY;
    ctrl.thrusterPower[1] += uY;
    ctrl.thrusterPower[2] -= uY;
    ctrl.thrusterPower[3] += uY;
    // Vertical thrusters
    ctrl.thrusterPower[4] += uD + uP + uR;
    ctrl.thrusterPower[5] += uD + uP + uR;
    ctrl.thrusterPower[6] += uD - uP + uR;
    ctrl.thrusterPower[7] += uD - uP - uR;
}