#include "debug.h"
#include "logger.h"
#include <Arduino.h>
inline char *padding(int val) {
    if (val > 0)
        (val >= 100) ? " " : (val >= 10) ? "  " : "   ";
    else
        (val <= -100) ? "-" : (val <= -10) ? " -" : "  -";
}
void Debug::debugHandler() {
    String         out;
    unsigned short nl_count = 0;
    if (flags.analogSensors) {
        out += "Analog sensors' readings: ";
        out += padding(tele->current);
        out += String(abs(tele->current), 2) + " A @ ";
        out += padding(tele->voltage);
        out += String(abs(tele->voltage)) + " V\n\r";
        nl_count++;
    }
    if (flags.depthSensor) {
        out += "Depth sensor readings: " + String(tele->depth) + "m deep, " +
               String(tele->temp) + "ºC \n\r";
        nl_count++;
    }
    if (flags.imu) {
        out += "IMU readings: " + String(tele->yaw) + "º yaw, " +
               String(tele->roll) + "º roll, " + String(tele->pitch) +
               "º pitch\n\r";
        nl_count++;
    }
    if (flags.thrusters) {
        out += "Thrusters' power: \n\r";
        out += "    HFR: " + String(ctrl->thrusterPower[1]) +
               " HFL: " + String(ctrl->thrusterPower[0]) + "\n\r",
            false;
        out += "    HBR: " + String(ctrl->thrusterPower[3]) +
               " HBL: " + String(ctrl->thrusterPower[2]) + "\n\r",
            false;
        out += "    VBR: " + String(ctrl->thrusterPower[7]) +
               " VBL: " + String(ctrl->thrusterPower[6]) + "\n\r",
            false;
        out += "    VFR: " + String(ctrl->thrusterPower[5]) +
               " VFL: " + String(ctrl->thrusterPower[4]) + "\n\r",
            false;
        nl_count += 5;
    }
    if (flags.manipulator) {
        out += "Manipualtor control signals: Open-close: " +
               String(ctrl->manipulatorOpenClose) +
               " Rotate: " + String(ctrl->manipulatorRotate) + "\n\r";
        nl_count++;
    }
    if (flags.cameras) {
        out += String("Cameras' rotation: ") +
               "Front: " + String(ctrl->cameraRotationDelta[0]) +
               " Rear: " + String(ctrl->cameraRotationDelta[1]) +
               " CamSel: " + String(ctrl->camsel) + "\n\r";
        nl_count++;
    }
    if (flags.regulators) {
        out              += "Regulators debug not implemented!\n\r";
        flags.regulators = 0;
    }

    String prefix;
    for (; nl_count > 0; nl_count--) {
        prefix += "\033[F";
        prefix += "\033[K";
    }
    Logger::info(prefix + out, false);
}

void Debug::menu() {
    Logger::warn(F("!!!!Suspending execution!!!!\n\r"));
    Logger::info(F("User debug menu, ver. 0.7\n\r"));
    Logger::info(F("Flags listed below:\n\r"
                   "    1. Depth sensor\n\r"
                   "    2. Analog sensors\n\r"
                   "    3. Imu\n\r"
                   "    4. Thrusters\n\r"
                   "    5. Manipulator\n\r"
                   "    6. Cameras\n\r"
                   "    7. Regulators\n\r"));
    Logger::info(F("Input: [1,d/2,a/3,i/4,t/5,m/6,c/7,r/0,e]: "));

    String     input;
    debugFlags pendingFlags = flags;
    while (!input.endsWith("\n") && !input.endsWith("\r")) {
        while (!SerialUSB.available()) {
        }
        String pendingInput = SerialUSB.readString();
        if (pendingInput.indexOf('1') >= 0 || pendingInput.indexOf('d') >= 0) {
            Logger::debug(F("Toggling depth sensor debug mode\n\r"));
            pendingFlags.depthSensor = !flags.depthSensor;
        }
        if (pendingInput.indexOf('2') >= 0 || pendingInput.indexOf('a') >= 0) {
            Logger::debug(F("Toggling analog sensors debug mode\n\r"));
            pendingFlags.analogSensors = !flags.analogSensors;
        }
        if (pendingInput.indexOf('3') >= 0 || pendingInput.indexOf('i') >= 0) {
            Logger::debug(F("Toggling IMU debug mode\n\r"));
            pendingFlags.imu = !flags.imu;
        }
        if (pendingInput.indexOf('4') >= 0 || pendingInput.indexOf('t') >= 0) {
            Logger::debug(F("Toggling thrusters debug mode\n\r"));
            pendingFlags.thrusters = !flags.thrusters;
        }
        if (pendingInput.indexOf('5') >= 0 || pendingInput.indexOf('m') >= 0) {
            Logger::debug(F("Toggling manipulator debug mode\n\r"));
            pendingFlags.manipulator = !flags.manipulator;
        }
        if (pendingInput.indexOf('6') >= 0 || pendingInput.indexOf('c') >= 0) {
            Logger::debug(F("Toggling cameras debug mode\n\r"));
            pendingFlags.cameras = !flags.cameras;
        }
        if (pendingInput.indexOf('7') >= 0 || pendingInput.indexOf('r') >= 0) {
            Logger::debug(F("Toggling regulators debug mode\n\r"));
            pendingFlags.regulators = !flags.regulators;
        }
        if (pendingInput.indexOf('0') >= 0 || pendingInput.indexOf('e') >= 0) {
            pendingInput += "\n\r";
        }
        Logger::info(pendingInput, false);
        input += pendingInput;
    }
    Logger::info("Flags was " + String(flags.flags, 2) + " now " +
                 String(pendingFlags.flags, 2) + "\n\r");
    flags.flags = pendingFlags.flags;
    // Logger::info(F("Debug active for: "));
    // if (pendingFlags.depthSensor) {
    //     Logger::info(F("Depth sensor, "), false);
    // }
    // if (pendingFlags.analogSensors) {
    //     Logger::info(F("Analog sensors, "), false);
    // }
    // if (pendingFlags.imu) {
    //     Logger::info(F("IMU, "), false);
    // }
    // if (pendingFlags.thrusters) {
    //     Logger::info(F("Thrusters, "), false);
    // }
    // if (pendingFlags.manipulator) {
    //     Logger::info(F("Manipulator, "), false);
    // }
    // if (pendingFlags.cameras) {
    //     Logger::info(F("Cameras, "), false);
    // }
    // if (pendingFlags.regulators) {
    //     Logger::info(F("Regulators, "), false);
    // }
    // Logger::info(F("\b\b\n\r"), false);
}