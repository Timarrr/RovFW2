#include "debug.h"
#include "logger.h"
#include <Arduino.h>
void Debug::debugHandler() {
    if (flags.analogSensors) {
        Logger::debug("Analog sensors' readings: " + String(tele->current, 3) +
                      " A @ " + String(tele->voltage) + " V\n\r");
    }
    if (flags.depthSensor) {
        Logger::debug("Depth sensor readings: " + String(tele->depth) +
                      "m deep, " + String(tele->temp) + "ºC \n\r");
    }
    if (flags.imu) {
        Logger::debug("IMU readings: " + String(tele->yaw) + "º yaw, " +
                      String(tele->roll) + "º roll, " + String(tele->pitch) +
                      "º pitch\n\r");
    }
    if (flags.thrusters) {
        Logger::debug("Thrusters' power: \n\r");
        Logger::debug("    HFR: " + String(ctrl->thrusterPower[1]) +
                          " HFL: " + String(ctrl->thrusterPower[0]) + "\n\r",
                      false);
        Logger::debug("    HBR: " + String(ctrl->thrusterPower[3]) +
                          " HBL: " + String(ctrl->thrusterPower[2]) + "\n\r",
                      false);
        Logger::debug("    VBR: " + String(ctrl->thrusterPower[7]) +
                          " VBL: " + String(ctrl->thrusterPower[6]) + "\n\r",
                      false);
        Logger::debug("    VFR: " + String(ctrl->thrusterPower[5]) +
                          " VFL: " + String(ctrl->thrusterPower[4]) + "\n\r",
                      false);
    }
    if (flags.manipulator) {
        Logger::debug("Manipualtor control signals: Open-close: " +
                      String(ctrl->manipulatorOpenClose) +
                      " Rotate: " + String(ctrl->manipulatorRotate) + "\n\r");
    }
    if (flags.cameras) {
        Logger::debug(String("Cameras' rotation: ") +
                      "Front: " + String(ctrl->cameraRotationDelta[0]) +
                      " Rear: " + String(ctrl->cameraRotationDelta[1]) +
                      " CamSel: " + String(ctrl->camsel));
    }
    if (flags.regulators) {
        Logger::debug("Regulators debug not implemented!");
        flags.regulators = 0;
    }
}