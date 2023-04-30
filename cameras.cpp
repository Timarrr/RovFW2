#ifndef CAMERAS_CPP
#define CAMERAS_CPP

#include "cameras.h"
#include "api/Common.h"
#include "config.h"
#include "logger.h"
#include <cctype>

Cameras::Cameras(bool launch, bool test) {
    Logger::info(F("Rotary cameras init\n\r"));
    pinMode(config::cameras::multiplexer_pin, OUTPUT);
    m_backServo.attach(config::cameras::back);
    m_frontServo.attach(config::cameras::front);
    if (test) {
        Logger::info(F("Testing cameras rotation\n\r"));
        for (byte pos = 0; pos <= 180; pos++) {
            m_backServo.write(pos);
            m_frontServo.write(pos);
            delay(15);
        }
    }
    m_backServo.write(angles[0]);
    m_frontServo.write(angles[1]);
}

void Cameras::set_angle_delta(int idx, int angle_delta) {
    int servo_idx = (idx == 0) ? config::cameras::servos::front
                               : config::cameras::servos::back;

    if ((millis() - m_lastUpdateTimes[idx]) > 17) {
        m_lastUpdateTimes[idx] = millis();

        angles[idx] = constrain(angles[idx] + angle_delta, 0, 180);
        if (idx == 0)
            m_frontServo.write(angles[idx]);
        else
            m_backServo.write(angles[idx]);
    }
}

void Cameras::select_cam(int index) {
    digitalWrite(config::cameras::multiplexer_pin, index);
}

#endif