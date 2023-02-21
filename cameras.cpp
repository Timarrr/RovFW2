#ifndef CAMERAS_CPP
#define CAMERAS_CPP

#include "cameras.h"
#include "config.h"

Cameras::Cameras(bool launch, bool test) : m_camServos(){
    SerialUSB.println("Rotary cameras init");
    m_camServos[0].attach(config::cameras::servos::front);
    m_camServos[1].attach(config::cameras::servos::back);
    m_camServos[0].writeMicroseconds(angles[0]);
    m_camServos[1].writeMicroseconds(angles[1]);
}

void Cameras::set_angle(int idx, int angle) {
    int servo_idx = (idx == 0) ? config::cameras::servos::front : config::cameras::servos::back;

    if ((millis() - m_lastUpdateTimes[idx]) > 17) {
        m_lastUpdateTimes[idx] = millis();
        // data.angles[idx] += angle;
        angles[idx] = constrain(angles[idx] + angle, 0, 180);
        m_camServos[servo_idx].writeMicroseconds(angles[servo_idx]);
    }
}

void Cameras::select_cam(bool state){
    digitalWrite(config::cameras::multiplexer_pin, state);
}

#endif