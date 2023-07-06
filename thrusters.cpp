#ifndef THRUSTERS_CPP
#define THRUSTERS_CPP
#include "thrusters.h"
#include "SAMD_ISR_Servo/SAMD_ISR_Servo.h"
#include "USB/USBAPI.h"
#include "api/Common.h"
#include "config.h"
#include "isrcontroller.h"
#include "logger.h"
#include "rovdatatypes.h"
#define THRUSTER_TEST_WAIT_TIME 100
#define THRUSTER_POWER_COEFF    0.3f

Thrusters::Thrusters(bool launch, bool test, long &thrusters_init_begin_time) {
    if (!launch) {
        Logger::info(F("Thrusters init cancelled\n\r"));
        return;
    }

    if (test) {
        Logger::info(F("Waiting for thrusters init..."));
        delay(8000);
        Logger::info(F("  Done!\n\r"));
        Logger::info(F("Testing thrusters:\n\r"));
        for (int i = 0; i < 4; i++) {
            Logger::info("Set thruster pair " + String(i) + " to -100\r");
            m_controller.setThruster(i, -100 * THRUSTER_POWER_COEFF);
            m_controller.setThruster(i + 1, -100 * THRUSTER_POWER_COEFF);
            delay(THRUSTER_TEST_WAIT_TIME);
            Logger::info("Set thruster pair " + String(i) + " to 0\r");
            m_controller.setThruster(i, 0);
            m_controller.setThruster(i + 1, 0);
            delay(THRUSTER_TEST_WAIT_TIME);
            Logger::info("Set thruster pair " + String(i) + " to 100\r");
            m_controller.setThruster(i, 100 * THRUSTER_POWER_COEFF);
            m_controller.setThruster(i + 1, 100 * THRUSTER_POWER_COEFF);
            delay(THRUSTER_TEST_WAIT_TIME);
            Logger::info("Set thruster pair " + String(i) + " to 0\r");
            m_controller.setThruster(i, 0);
            m_controller.setThruster(i + 1, 0);
            delay(THRUSTER_TEST_WAIT_TIME);
        }
        thrusters_init_begin_time = millis();
    } else {
        thrusters_init_begin_time = millis();
        return;
    }
}

void Thrusters::update(RovControl ctrl) {
    using namespace config::thrusters;

    m_controller.setThruster(thrusters::horizontal_front_left,
                             ctrl.thrusterPower[0] * thrusterDirections[0]);
    m_controller.setThruster(thrusters::horizontal_front_right,
                             ctrl.thrusterPower[1] * thrusterDirections[1]);
    m_controller.setThruster(thrusters::horizontal_back_left,
                             ctrl.thrusterPower[2] * thrusterDirections[2]);
    m_controller.setThruster(thrusters::horizontal_back_right,
                             ctrl.thrusterPower[3] * thrusterDirections[3]);
    m_controller.setThruster(thrusters::vertical_front_left,
                             ctrl.thrusterPower[4] * thrusterDirections[4]);
    m_controller.setThruster(thrusters::vertical_front_right,
                             ctrl.thrusterPower[5] * thrusterDirections[5]);
    m_controller.setThruster(thrusters::vertical_back_left,
                             ctrl.thrusterPower[6] * thrusterDirections[6]);
    m_controller.setThruster(thrusters::vertical_back_right,
                             ctrl.thrusterPower[7] * thrusterDirections[7]);
}

#endif