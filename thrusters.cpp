#ifndef THRUSTERS_CPP
#define THRUSTERS_CPP
#include "thrusters.h"
#include "SAMD_ISR_Servo/SAMD_ISR_Servo.h"
#include "USB/USBAPI.h"
#include "api/Common.h"
#include "config.h"
#include "isrcontroller.h"

Thrusters::Thrusters(bool launch, bool test) : m_controller(config::thrusters::pins){
    if (!launch)return;
    if (!test)return;
    this->test();
}

void Thrusters::test(){
    SerialUSB.println("Testing thrusters:");
    for (int i = 0; i < 8; i++) {
        SerialUSB.println("Set thruster " + String(i) + " to -100");
        m_controller.setThruster(i, -100);
        delay(500);
        SerialUSB.println("Set thruster " + String(i) + " to 0");
        m_controller.setThruster(i, 0);
        delay(500);
        SerialUSB.println("Set thruster " + String(i) + " to 100");
        m_controller.setThruster(i, 100);        
        delay(500);
        SerialUSB.println("Set thruster " + String(i) + " to 0");
        m_controller.setThruster(i, 0);
    }
}

void Thrusters::update_thrusters(RovControl &ctrl) {
	using namespace config::thrusters;

	m_controller.setThruster(thrusters::horizontal_front_left,  ctrl.thrusterPower[0]);
	m_controller.setThruster(thrusters::horizontal_front_right, ctrl.thrusterPower[1]);
	m_controller.setThruster(thrusters::horizontal_back_left,   ctrl.thrusterPower[2]);
	m_controller.setThruster(thrusters::horizontal_back_right,  ctrl.thrusterPower[3]);
	m_controller.setThruster(thrusters::vertical_front_left,    ctrl.thrusterPower[4]);
	m_controller.setThruster(thrusters::vertical_front_right,   ctrl.thrusterPower[5]);
	m_controller.setThruster(thrusters::vertical_back_left,     ctrl.thrusterPower[6]);
	m_controller.setThruster(thrusters::vertical_back_right,    ctrl.thrusterPower[7]);
	
	SerialUSB.println(
		"0-HFL:	" + (String) ctrl.thrusterPower[0] + "	1-HFR:	" + (String) ctrl.thrusterPower[1] + "\n\r"
		"2-HRL:	" + (String) ctrl.thrusterPower[2] + "	3-HRR:	" + (String) ctrl.thrusterPower[3] + "\n\r"
		"4-VFL:	" + (String) ctrl.thrusterPower[4] + "	5-VFR:	" + (String) ctrl.thrusterPower[5] + "\n\r"
		"6-VRL:	" + (String) ctrl.thrusterPower[6] + "	7-VRR:	" + (String) ctrl.thrusterPower[7] + "\n\r"
	);

}

#endif