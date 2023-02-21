#ifndef CAMERAS_H
#define CAMERAS_H

#include <Arduino.h>
#include <Servo.h>
#include "config.h"

class Cameras{
public:
    Cameras(bool launch, bool test);
    void set_angle(int idx, int angle);
    void select_cam(bool state);
    int get_cam_index();
private:
    int angles[2] = { 90, 90 };
    Servo m_camServos[2];
    long long m_lastUpdateTimes[2];
    bool cam_switch = false;
};

#endif