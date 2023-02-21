#ifndef HELPERS_CPP
#define HELPER_CPP

#include "helpers.h"

void helpers::swapEndianRovTelemetry(RovTelemetry &rt){
        rt.yaw = swapEndian(rt.yaw);
        rt.pitch = swapEndian(rt.pitch);
        rt.roll = swapEndian(rt.roll);

        rt.ammeter = swapEndian(rt.ammeter);
        rt.voltmeter = swapEndian(rt.voltmeter);

        // rt.manipulatorAngle = swapEndian(rt.manipulatorAngle);
        // rt.manipulatorState = swapEndian(rt.manipulatorState);
        // rt.regulatorsFeedback = swapEndian(rt.regulatorsFeedback);
        rt.depth = swapEndian(rt.depth);

        rt.cameraIndex = swapEndian(rt.cameraIndex);
        rt.temperature = swapEndian(rt.temperature);
}

void helpers::swapEndianRovControl(RovControl &rc){
    rc.cameraRotation[0] = swapEndian(rc.cameraRotation[0]);
    rc.cameraRotation[1] = swapEndian(rc.cameraRotation[1]);

    for (int8_t &tp : rc.thrusterPower) {
        tp = swapEndian(tp);
    }

    // rc.thrusterPower[0] = swapEndian(rc.thrusterPower[0]);
    // rc.thrusterPower[1] = swapEndian(rc.thrusterPower[1]);
    // rc.thrusterPower[2] = swapEndian(rc.thrusterPower[2]);
    // rc.thrusterPower[3] = swapEndian(rc.thrusterPower[3]);
    // rc.thrusterPower[4] = swapEndian(rc.thrusterPower[4]);
    // rc.thrusterPower[5] = swapEndian(rc.thrusterPower[5]);
    // rc.thrusterPower[6] = swapEndian(rc.thrusterPower[6]);
    // rc.thrusterPower[7] = swapEndian(rc.thrusterPower[7]);

    rc.auxFlags = swapEndian(rc.auxFlags);
    rc.manipulator[0] = swapEndian(rc.manipulator[0]);
    rc.manipulator[1] = swapEndian(rc.manipulator[1]);
    rc.camsel = swapEndian(rc.camsel);
}

#endif