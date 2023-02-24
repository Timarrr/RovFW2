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

	pinMode(LED_BUILTIN, OUTPUT);
	analogWrite(LED_BUILTIN, 100);

    configType curConf = config::launchConfig::currentConfig;

    switch (curConf) {
        case config::launchConfig::fast: //launch everything, test nothing
            Logger::info(F("Fast startup selected"));
            break;
        case configType::full: //launch and test everything 
            Logger::info(F("Full startup selected"));
            break;
        case configType::standaloneWithDepth: //don't launch anything except for depth sensor
            Logger::info(F("Standalone startup with forced depth sensor init selected"));
            break;
        case config::launchConfig::standaloneWithEthernet: //don't launch anything except for ethernet
            Logger::info(F("Standalone startup with forced networking init selected"));
            break;
        case config::launchConfig::standalone: //don't launch anything  
            Logger::info(F("Standalone startup selected"));
            break;
        default:
            Logger::info(F("Unknown launch value"));
            while (true);        
    }

    Logger::warn(F("Overriding startup config to full"));
    curConf = config::launchConfig::full;

    Logger::info("Memory used: " + String(freeMemory()));
    networking = new Networking(true, true);
    Logger::info("Memory used: " + String(freeMemory()));
    thrusters = new Thrusters(true, true);
    Logger::info("Memory used: " + String(freeMemory()));
    sensors = new Sensors(true, true, true);
    Logger::info("Memory used: " + String(freeMemory()));
    imu = new IMUSensor(true, true);
    Logger::info("Memory used: " + String(freeMemory()));
    cameras = new Cameras(true, true);
    Logger::info("Memory used: " + String(freeMemory()));
    manipulator = new Manipulator(true, true);
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
				Logger::info(F("send \"reset\" for controller reset or \"debug\" for debug menu [DISABLED]"));
	}
}

void Rov::loop(){
    Logger::debug(F("sH"));
    serialHandler();
    Logger::debug(F("SU"));
    sensors->update();
    Logger::debug(F("gY"));
    tele->yaw = imu->getYaw();
    Logger::debug(F("gR"));
    tele->roll = imu->getRoll();
    Logger::debug(F("gP"));
    tele->pitch = imu->getPitch();
    Logger::debug(F("gD"));
    tele->depth = sensors->getDepth();
    Logger::debug(F("gT"));
    tele->temperature = sensors->getTemperature();
    Logger::debug(F("gA"));
    tele->ammeter = sensors->getCurrent();
    Logger::debug(F("gV"));
    tele->voltmeter = sensors->getVoltage();
    Logger::debug(F("cS"));
    tele->cameraIndex = control->camsel;
    
    if (config::launchConfig::currentConfig & (config::launchConfig::configType::fast | config::launchConfig::configType::full | config::launchConfig::configType::standaloneWithEthernet)) {
        Logger::debug(F("nM"));
        networking->maintain();
        Logger::debug(F("rRC"));
        networking->readRovControl(*control);
        Logger::debug(F("wRT"));   
        networking->writeRovTelemetry(*tele);
    }

    Logger::debug(F("uTH"));
    thrusters->update_thrusters(*control);

    Logger::debug(F("sCA0"));
    cameras->set_angle(config::cameras::servos::front, constrain(control->cameraRotation[0], -1, 1) * 3.0);
    Logger::debug(F("sCA1"));
    cameras->set_angle(config::cameras::servos::back,  constrain(control->cameraRotation[1], -1, 1) * 3.0);
    Logger::debug(F("seC"));
    cameras->select_cam(control->camsel == 1 ? true : false);

    Logger::debug(F("mSOC"));
    manipulator->setOpenClose(control->manipulator[0]);
    Logger::debug(F("mSR"));
    manipulator->setRotate(control->manipulator[1]);

    Logger::debug(F("LED"));
    analogWrite(LED_BUILTIN, sin(millis() * 0.01) * 127 + 127);
    // Debug::debugHandler(control);
}

#endif