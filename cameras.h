#ifndef CAMERAS_H
#define CAMERAS_H

#include <Arduino.h>
#include <Servo.h>
#include "config.h"
#include "logger.h"


class Cameras{
public:
    Cameras(bool launch, bool test);
    void set_angle_delta(int idx, int angle);
    void select_cam(bool state);
    int get_cam_index();
private:
    int angles[2] = { 90, 90 };
    Servo m_frontServo;
    Servo m_backServo;
    long long m_lastUpdateTimes[2] = {0, 0};
    bool cam_switch = false;
};

#endif