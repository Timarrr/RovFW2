#ifndef ISRCONTROLLER_CPP
#define ISRCONTROLLER_CPP
#include "isrcontroller.h"
#include "logger.h"
#define ISR_SERVO_DEBUG 2


ISRController::ISRController(){
    using namespace config::thrusters;

    Logger::debug(F("ISR init"));

    SAMD_ISR_Servos.useTimer(TIMER_TCC);

    m_pins[thrusters::horizontal_front_left]  = pins[0];
    m_pins[thrusters::horizontal_front_right] = pins[1];
    m_pins[thrusters::horizontal_back_left]   = pins[2];
    m_pins[thrusters::horizontal_back_right]  = pins[3];
    m_pins[thrusters::vertical_front_left]    = pins[4];
    m_pins[thrusters::vertical_front_right]   = pins[5];
    m_pins[thrusters::vertical_back_left]     = pins[6];
    m_pins[thrusters::vertical_back_right]    = pins[7];

    for (int i = 0; i < 8; i++) {
        m_isrServos[i] = SAMD_ISR_Servos.setupServo(m_pins[i], pulse_min, pulse_max);
    }
    SAMD_ISR_Servos.setReadyToRun();
    for (int i = 0; i < 8; i++) {
        SAMD_ISR_Servos.setPulseWidth(m_isrServos[i], pulse_med);
    }
}

void ISRController::setThruster(int idx, int power) {
    int pulse = map(power, -100, 100, 1010, 1990);
    pulse = constrain(pulse, 1010, 1990);
    SAMD_ISR_Servos.setPulseWidth(m_isrServos[idx], pulse);
}

#endif