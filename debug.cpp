#include "debug.h"
#include "logger.h"
#include <Arduino.h>
#include <cstdio>
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
        char buffer[96];
        sprintf(buffer, "Analog sensors' readings: \t%07.3f A, \t%07.3f V \n\r",
                tele->current, tele->voltage);
        out += buffer;
        nl_count++;
    }
    if (flags.depthSensor) {
        char buffer[64];
        sprintf(buffer, "Depth sensor readings: \t%07.3f m, \t%07.3f ºC\n\r",
                tele->depth, tele->temp);
        out += buffer;
        nl_count++;
    }
    if (flags.imu) {
        char buffer[196];
        int  ccount;
        sprintf(buffer,
                "IMU readings:\t%07.3f º Yaw,\t%07.3f º Rol,\t%07.3f º Pit\n\r "
                "Calibration: \t%07.3f º Yaw,\t%07.3f º Rol,\t%07.3f º Pit\n\r "
                "             \t%09.3f G X,\t%09.3f G Y,\t%09.3f G Z, %n%d\n\r",
                tele->yaw, tele->roll, tele->pitch, tele->yawCal, tele->rollCal, tele->pitchCal, tele->accel0, tele->accel1,
                tele->accel2, &ccount, ccount);
        out      += buffer;
        nl_count += 3;
    }
    if (flags.thrusters) {
        char buffer[196];
        sprintf(buffer,
                "Thrusters' power: \n\r"
                "0:  %5d, 1:  %5d, 2: %5d, 3: %5d\n\r"
                "4:  %5d, 5:  %5d, 6: %5d, 7: %5d\n\r",
                ctrl->thrusterPower[0], ctrl->thrusterPower[1],
                ctrl->thrusterPower[2], ctrl->thrusterPower[3],
                ctrl->thrusterPower[4], ctrl->thrusterPower[5],
                ctrl->thrusterPower[6], ctrl->thrusterPower[7]);
        out      += buffer;
        nl_count += 3;
    }
    if (flags.manipulator) {
        char buffer[64];
        sprintf(buffer, "Manipulator control: %03d O-C, %03d Rot\n\r",
                ctrl->manipulatorOpenClose, ctrl->manipulatorRotate);
        out += buffer;
        nl_count++;
    }
    if (flags.cameras) {
        char buffer[96];
        sprintf(buffer, "Cameras' rotation: F:%01d, R:%01d, camsel: %01d\n\r",
                ctrl->cameraRotationDelta[0], ctrl->cameraRotationDelta[1],
                ctrl->camsel);
        out += buffer;
        nl_count++;
    }
    if (flags.regulators) {
        out              += "Regulators debug not implemented!\n\r";
        flags.regulators  = 0;
    }

    String prefix;
    if (nl_count > 0) {
        for (; nl_count > 0; nl_count--) {
            prefix += "\033[F\033[K";
        }
        Logger::info(prefix + out, false);
    }
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
                   "    7. Regulators\n\r"
                   "    9. Invalidate IMU calibration data\n\r"));
    Logger::info(F("Input: [1,d/2,a/3,i/4,t/5,m/6,c/7,r/8,C/9,I/0,e]: "));

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
        if (pendingInput.indexOf('8') >= 0 || pendingInput.indexOf('C') >= 0) {
            Logger::debug(F("I'm Ayana and I manage this ROV! ^_^\n\r"));
            Logger::debug(ayana, false);
            pendingFlags.regulators = !flags.regulators;
        }
        if (pendingInput.indexOf('9') >= 0 || pendingInput.indexOf('I') >= 0) {
            Logger::debug(F("Invalidating IMU calibration data\n\r"));
            auxCtrl->auxFlags.imuInvCal = 1;
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