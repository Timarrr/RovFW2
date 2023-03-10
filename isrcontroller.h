#ifndef ISRCONTROLLER_H
#define ISRCONTROLLER_H
#include <Wire.h>
#include <Servo.h>
#include "SAMD_ISR_Servo/SAMD_ISR_Servo.h"
#include "config.h"
#include "logger.h"

class ISRController{
public:
    ISRController();
    void setThruster(int idx, int power);
private:
    static const int pulse_min = 1000;
    static const int pulse_med = 1500;
    static const int pulse_max = 2000;

    //ISR pins
    int m_pins[8] = {0,0,0,0,0,0,0,0};
    //ISR servos
    int m_isrServos[8] = {0,0,0,0,0,0,0,0};
};


#endif