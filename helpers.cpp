#ifndef HELPERS_CPP
#define HELPER_CPP

#include "helpers.h"

void helpers::swapEndianRovTelemetry(RovTelemetry &rt){
        rt.yaw = swapEndian(rt.yaw);
        rt.pitch = swapEndian(rt.pitch);
        rt.roll = swapEndian(rt.roll);

        rt.current = swapEndian(rt.current);
        rt.voltage = swapEndian(rt.voltage);

        // rt.manipulatorAngle = swapEndian(rt.manipulatorAngle);
        // rt.manipulatorState = swapEndian(rt.manipulatorState);
        // rt.regulatorsFeedback = swapEndian(rt.regulatorsFeedback);
        rt.depth = swapEndian(rt.depth);

        rt.cameraIndex = swapEndian(rt.cameraIndex);
        rt.temperature = swapEndian(rt.temperature);
}

void helpers::swapEndianRovControl(RovControl &rc){
    rc.cameraRotationDelta[0] = swapEndian(rc.cameraRotationDelta[0]);
    rc.cameraRotationDelta[1] = swapEndian(rc.cameraRotationDelta[1]);

    for (int8_t tp : rc.thrusterPower) {
        tp = swapEndian(tp);
    }

    rc.manipulatorOpenClose = swapEndian(rc.manipulatorOpenClose);
    rc.manipulatorRotate = swapEndian(rc.manipulatorRotate);
    rc.camsel = swapEndian(rc.camsel);
}

#endif