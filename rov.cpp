#ifndef ROV_CPP
#define ROV_CPP
#include "rov.h"
#include "config.h"
#include "sensors.h"



Rov::Rov(){
    using namespace config::launchConfig;
    SerialUSB.println("HighROV init!");

	pinMode(LED_BUILTIN, OUTPUT);
	analogWrite(LED_BUILTIN, 100);

    switch (currentConfig) {
        case config::launchConfig::fast: //launch everything, test nothing
            SerialUSB.println("Fast startup selected");
            networking = new Networking(true, false);
            sensors = new Sensors(true, false, true);
            thrusters = new Thrusters(true, false);
            cameras = new Cameras(true, false);
            manipulator = new Manipulator(true, false);
            break;
        default:
        case configType::full: //launch and test everything [default]
            SerialUSB.println("Full startup selected");
            networking = new Networking(true, true);
            sensors = new Sensors(true, true, true);
            thrusters = new Thrusters(true, true);
            cameras = new Cameras(true, true);
            manipulator = new Manipulator(true, true);
            break;
        case configType::standaloneWithDepth: //don't launch anything except for depth sensor
            SerialUSB.println("Standalone startup with forced depth sensor init selected");
            sensors = new Sensors(true, true, true);
            break;
        case config::launchConfig::standaloneWithEthernet: //don't launch anything except for ethernet
            SerialUSB.println("Standalone startup with forced networking init selected");
            networking = new Networking(true, true);
            break;
        case config::launchConfig::standalone: //don't launch anything  

            break;


        }

        if (currentConfig == configType::full) {
        
    }
}

void Rov::serialHandler(){
	if(SerialUSB.available()){
		String msg = SerialUSB.readString();
		msg.trim();
		if(msg=="reset"){
			SerialUSB.println("Resetting the controller, please reconnect the debug cable or reactivate serial monitor if you want to continue debugging");
            SerialUSB.end();
            Wire.end();
            sensors->end();
            NVIC_SystemReset();
		}
		else
			if(msg=="debug")
                asm("nop");
                // Debug::debugMenu();
			else
				SerialUSB.println("send \"reset\" for controller reset or \"debug\" for debug menu [DISABLED]");
	}
}

void Rov::loop(){
    serialHandler();
    sensors->update();
    tele->yaw = sensors->getYaw();
    tele->roll = sensors->getRoll();
    tele->pitch = sensors->getPitch();
    tele->depth = sensors->getDepth();
    tele->temperature = sensors->getTemp();
    tele->ammeter = sensors->getAmperage();
    tele->voltmeter = sensors->getVoltage();
    tele->cameraIndex = control->camsel;
    
    if (config::launchConfig::currentConfig & (config::launchConfig::configType::fast | config::launchConfig::configType::full | config::launchConfig::configType::standaloneWithEthernet)) {
        networking->maintain();
        networking->readRovControl(*control);
        networking->writeRovTelemetry(*tele);
    }

    thrusters->update_thrusters(*control);

    cameras->set_angle(config::cameras::servos::front, constrain(control->cameraRotation[0], -1, 1) * 3.0);
    cameras->set_angle(config::cameras::servos::back,  constrain(control->cameraRotation[1], -1, 1) * 3.0);
    cameras->select_cam(control->camsel == 1 ? true : false);

    manipulator->setOpenClose(control->manipulator[0]);
    manipulator->setRotate(control->manipulator[1]);

    analogWrite(LED_BUILTIN, sin(millis() * 0.01) * 127 + 127);
    // Debug::debugHandler(control);
}

#endif