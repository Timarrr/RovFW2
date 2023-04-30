#include "regulators.h"
#include "api/Common.h"
#include "config.h"
#include "helpers.h"
#include "logger.h"
#include "rovdatatypes.h"
#include <cstdint>

PDRegulator::PDRegulator(int kP, int kD)
    : kP(kP), kD(kD), lastErr(0), lastTime(millis()) {}

int PDRegulator::eval(int data, int target) {
    int err = target - data;
    int u =
        kP * (err + kD * ((lastErr - err) /
                          (lastTime - millis()))); // kP(error + kD(dError/dt))
    lastErr  = err;
    lastTime = millis();
    return u;
}

float FPPDRegulator::eval(float data, float target) {
    if (lastData > 320 && data < 40) { // if overflowing, account for it
        offset += 1;
    }
    if (lastData < 40 && data > 320) { // if underflowing, account for it
        offset -= 1;
    }

    float err = target - (data + 360 * offset);
    float uP  = kP * err;                                         // kP(error)
    float uD  = kD * ((err - lastError) / (micros() - lastTime)); // kD(de/dt)

    // Logger::trace("Evaluated regualtor value: " + String(uP + uD) +
    //               " (uP: " + String(uP) + ", uD: " + String(uD) +
    //               ", offset: " + String(offset, 10) + ") \n\r");
    lastError = err;
    lastTime  = micros();
    lastData  = data;

    return helpers::clamp(uP + uD, -128.0f, 127.0f);
}

void FPPDRegulator::reset() {
    lastData  = 0;
    offset    = 0;
    lastError = 0;
}

RovRegulators::RovRegulators()
    : rDepth(config::regulators::depth_p, config::regulators::depth_d),
      rYaw(config::regulators::yaw_p, config::regulators::yaw_d),
      rRoll(config::regulators::roll_p, config::regulators::roll_d),
      rPitch(config::regulators::pitch_p, config::regulators::pitch_d) {}

RovControl RovRegulators::evaluate(RovControl ctrl, RovAuxControl &auxCtrl,
                                   RovTelemetry &tele) {
    if (auxCtrl.auxFlags.eDepth &&
        tele.depth !=
            MAXFLOAT) { // If depth reg is enabled, evaluate its control signal
        uD = rDepth.eval(tele.depth, auxCtrl.dDepth);
    } else {
        rDepth.reset(); // Else reset everything in depth regulator
        uD = 0;
    }
    if (auxCtrl.auxFlags.eYaw) { // Same for yaw
        uY = rYaw.eval(tele.yaw, auxCtrl.dYaw);
    } else {
        rYaw.reset();
        uY = 0;
    }

    if (auxCtrl.auxFlags.eRoll) { // Same for roll
        uR = rRoll.eval(tele.roll, auxCtrl.dRoll);
    } else {
        rRoll.reset();
        uR = 0;
    }
    if (auxCtrl.auxFlags.ePitch) { // Same for pitch
        uP = rPitch.eval(tele.pitch, auxCtrl.dPitch);
    } else {
        rPitch.reset();
        uP = 0;
    }

    ctrl.thrusterPower[0] =
        helpers::clamp(int16_t(ctrl.thrusterPower[0]) + int16_t(uY), -128, 127);
    ctrl.thrusterPower[1] =
        helpers::clamp(int16_t(ctrl.thrusterPower[1]) - int16_t(uY), -128, 127);
    ctrl.thrusterPower[2] =
        helpers::clamp(int16_t(ctrl.thrusterPower[2]) + int16_t(uY), -128, 127);
    ctrl.thrusterPower[3] =
        helpers::clamp(int16_t(ctrl.thrusterPower[3]) - int16_t(uY), -128, 127);

    // Logger::trace("[0]: " + String(ctrl.thrusterPower[0], 10) +
    //               " [1]: " + String(ctrl.thrusterPower[1], 10) +
    //               " [2]: " + String(ctrl.thrusterPower[2], 10) +
    //               " [3]: " + String(ctrl.thrusterPower[3], 10) +
    //               "\n\r"); // Vertical thrusters
    ctrl.thrusterPower[4] = helpers::clamp(
        -int16_t(uD) - int16_t(uP) + int16_t(uR) + ctrl.thrusterPower[4], -128,
        127);
    ctrl.thrusterPower[5] = helpers::clamp(
        -int16_t(uD) - int16_t(uP) - int16_t(uR) + ctrl.thrusterPower[5], -128,
        127);
    ctrl.thrusterPower[6] = helpers::clamp(
        -int16_t(uD) + int16_t(uP) + int16_t(uR) + ctrl.thrusterPower[6], -128,
        127);
    ctrl.thrusterPower[7] = helpers::clamp(
        -int16_t(uD) + int16_t(uP) - int16_t(uR) + ctrl.thrusterPower[7], -128,
        127);
    return RovControl(ctrl);
}