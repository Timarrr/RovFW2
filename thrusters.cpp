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
#define THRUSTER_TEST_WAIT_TIME 1000
#define THRUSTER_POWER_COEFF    0.1f

Thrusters::Thrusters(bool launch, bool test, long &thrusters_init_begin_time)
    : m_controller() {
    if (!launch) {
        Logger::info(F("Thrusters init cancelled\n\r"));
        return;
    }

    if (test) {
        Logger::info(F("Waiting for thrusters init..."));
        delay(9000);
        Logger::info(F("  Done!\n\r"));
        Logger::info(F("Testing thrusters:\n\r"));
        for (int i = 0; i < 8; i++) {
            Logger::info("\t\t\t\t\t\rSet thruster " + String(i) +
                         " to -100\r");
            m_controller.setThruster(i, -100 * THRUSTER_POWER_COEFF);
            delay(THRUSTER_TEST_WAIT_TIME);
            Logger::info("\t\t\t\t\t\rSet thruster " + String(i) + " to 0\r");
            m_controller.setThruster(i, 0);
            delay(THRUSTER_TEST_WAIT_TIME);
            Logger::info("\t\t\t\t\t\rSet thruster " + String(i) + " to 100\r");
            m_controller.setThruster(i, 100 * THRUSTER_POWER_COEFF);
            delay(THRUSTER_TEST_WAIT_TIME);
            Logger::info("\t\t\t\t\t\rSet thruster " + String(i) + " to 0\r");
            m_controller.setThruster(i, 0);
            delay(THRUSTER_TEST_WAIT_TIME);
        }
        thrusters_init_begin_time = 0;
    } else {
        thrusters_init_begin_time = millis();
        return;
    }
}

void Thrusters::update(RovControl ctrl) {
    using namespace config::thrusters;
    for (int i = 0; i < 8; i++) {
        m_controller.setThruster(i,
                                 ctrl.thrusterPower[i] * thrusterDirections[i]);
    }

    // m_controller.setThruster(1,
    //                          ctrl.thrusterPower[1] * thrusterDirections[1]);
    // m_controller.setThruster(thrusters::hi_fr_le,
    //                          ctrl.thrusterPower[2] * thrusterDirections[2]);
    // m_controller.setThruster(thrusters::hi_fr_ri,
    //                          ctrl.thrusterPower[3] * thrusterDirections[3]);
    // m_controller.setThruster(thrusters::lo_ba_le,
    //                          ctrl.thrusterPower[4] * thrusterDirections[4]);
    // m_controller.setThruster(thrusters::lo_ba_ri,
    //                          ctrl.thrusterPower[5] * thrusterDirections[5]);
    // m_controller.setThruster(thrusters::hi_ba_le,
    //                          ctrl.thrusterPower[6] * thrusterDirections[6]);
    // m_controller.setThruster(thrusters::hi_ba_ri,
    //                          ctrl.thrusterPower[7] * thrusterDirections[7]);
}

#endif