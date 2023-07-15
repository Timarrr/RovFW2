#ifndef ISRCONTROLLER_CPP
#define ISRCONTROLLER_CPP
#include "isrcontroller.h"
#include "logger.h"
#define ISR_SERVO_DEBUG 2

ISRController::ISRController() {
    using namespace config::thrusters;

    Logger::debug(F("ISR init\n\r"));

    SAMD_ISR_Servos.useTimer(TIMER_TCC);

    m_pins[0] = thrusters::fr_lo_le;
    m_pins[1] = thrusters::fr_lo_ri;
    m_pins[2] = thrusters::fr_hi_le;
    m_pins[3] = thrusters::fr_hi_ri;
    m_pins[4] = thrusters::ba_lo_le;
    m_pins[5] = thrusters::ba_lo_ri;
    m_pins[6] = thrusters::ba_hi_le;
    m_pins[7] = thrusters::ba_hi_ri;

    for (int i = 0; i < 8; i++) {
        m_isrServos[i] =
            SAMD_ISR_Servos.setupServo(m_pins[i], pulse_min, pulse_max);
    }
    SAMD_ISR_Servos.setReadyToRun();
    for (int i = 0; i < 8; i++) {
        SAMD_ISR_Servos.setPulseWidth(m_isrServos[i], pulse_med);
    }
}

void ISRController::setThruster(int idx, int power) {
    int pulse;
    if (power != 0)
        pulse = map(power, -100, 100, 1000, 2000);
    else
        pulse = 1488;
    pulse = constrain(pulse, 1000, 2000);
    SAMD_ISR_Servos.setPulseWidth(m_isrServos[idx], pulse);
}

#endif