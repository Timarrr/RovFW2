#ifndef ROV_CPP
#define ROV_CPP
#include "rov.h"
#include "config.h"
#include "logger.h"

extern "C" char* sbrk(int incr);
int freeMemory() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}

Rov::Rov() : control(new RovControl), tele(new RovTelemetry){
    using namespace config::launchConfig;
    Wire.begin();
    Wire.setTimeout(1000);
    Wire.setClock(10000);

    SerialUSB.begin(115200);
    unsigned int t_on = millis() + config::serial::waitForSerialTime;
    
    pinMode(LED_BUILTIN, OUTPUT);
	analogWrite(LED_BUILTIN, 100);

    if(config::serial::waitForSerial){
        while(!SerialUSB){
            if(millis() >= t_on){  
                break;
            }else{
                analogWrite(LED_BUILTIN, sin(millis() * 0.03) * 127 + 127);
                delay(1);
            }
        }
        delay(200);
    }

    Logger::info(F("HighROV init!"));

    configType curConf = currentConfig;

    switch (curConf) {
        case fast: //launch everything, test nothing
            Logger::info(F("Fast startup selected"));
            break;
        case full: //launch and test everything 
            Logger::info(F("Full startup selected"));
            break;
        case standaloneWithDepth: //don't launch anything except for depth sensor
            Logger::info(F("Standalone startup with forced depth sensor init selected"));
            break;
        case standaloneWithEthernet: //don't launch anything except for ethernet
            Logger::info(F("Standalone startup with forced networking init selected"));
            break;
        case standalone: //don't launch anything  
            Logger::info(F("Standalone startup selected"));
            break;
        default:
            Logger::info(F("Unknown launch value"));
            while (true);        
    }

    Logger::info("Memory used: " + String(freeMemory()));
    networking = new Networking(curConf & initEthernet, curConf & (full | standaloneWithEthernet));
    Logger::info("Memory used: " + String(freeMemory()));
    thrusters = new Thrusters(curConf & (full | fast), curConf & full);
    Logger::info("Memory used: " + String(freeMemory()));
    sensors = new Sensors(curConf & (full | fast), curConf & full, curConf & initDepth);
    Logger::info("Memory used: " + String(freeMemory()));
    imu = new IMUSensor(curConf & (full | fast), curConf & full);
    Logger::info("Memory used: " + String(freeMemory()));
    cameras = new Cameras(curConf & (full | fast), curConf & full);
    Logger::info("Memory used: " + String(freeMemory()));
    manipulator = new Manipulator(curConf & (full | fast), curConf & full);
    Logger::info("Memory used: " + String(freeMemory()));
}

void Rov::serialHandler(){
	if(SerialUSB.available()){
		String msg = SerialUSB.readString();
		msg.trim();
		if(msg=="reset"){
			Logger::info(F("Resetting the controller, please reconnect the debug cable or reactivate serial monitor if you want to continue debugging"));
            SerialUSB.end();
            sensors->end();
            imu->end();
            NVIC_SystemReset();
		}
		else
			if(msg=="debug")
                asm("nop");
                // Debug::debugMenu();
			else
				Logger::info(F("send \"reset\" for controller reset or \"debug\" for debug menu [NIY]"));
	}
}

void Rov::loop(){
    serialHandler();
    sensors->update();
    imu->update();
    tele->yaw = imu->getYaw();
    tele->roll = imu->getRoll();
    tele->pitch = imu->getPitch();
    tele->depth = sensors->getDepth();
    tele->temperature = sensors->getTemperature();
    tele->ammeter = sensors->getCurrent();
    tele->voltmeter = sensors->getVoltage();
    tele->cameraIndex = control->camsel;
    
    if (config::launchConfig::currentConfig & (config::launchConfig::configType::fast | config::launchConfig::configType::full | config::launchConfig::configType::standaloneWithEthernet)) {
        networking->maintain();
        if (networking->getLinkStatus()) {
            networking->readRovControl(*control, *auxControl);
            networking->writeRovTelemetry(*tele);        
        }
    }
    thrusters->update_thrusters(*control);

    cameras->set_angle_delta(0, constrain(control->cameraRotation[0], -1, 1) * 3.0);
    cameras->set_angle_delta(1,  constrain(control->cameraRotation[1], -1, 1) * 3.0);
    cameras->select_cam(control->camsel == 1);
    manipulator->setOpenClose(control->manipulator[0]);
    manipulator->setRotate(control->manipulator[1]);

    analogWrite(LED_BUILTIN, sin(millis() * 0.01) * 127 + 127);
    // Debug::debugHandler(control);
}

#endif