#ifndef ROV_H
#define ROV_H
#include "math.h"
// #include "wiring_private.h"
#include "main.h"
#include "config.h"
#include "networking.h"
#include "rovdatatypes.h"
#include "sensors.h"
#include "imu.h"
#include "thrusters.h"
#include "isrcontroller.h"
#include "cameras.h"
#include "manipulator.h"


class Rov{
    public:
        Rov();
        void loop();
    private:
        void serialHandler();
        Thrusters *thrusters;
        Cameras *cameras;
        Manipulator *manipulator;
        Networking *networking;
        Sensors *sensors;
        IMUSensor *imu;
        ISRController *isr;
        RovTelemetry *tele;
        RovControl *control;

};

#endif