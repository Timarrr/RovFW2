#ifndef ROV_CPP
#define ROV_CPP
#include "rov.h"
#include "USB/USBAPI.h"
#include "api/Common.h"
#include "config.h"
#include "logger.h"
#include "rovdatatypes.h"
#include <cstdint>

#define PROFILE 0

#define PROFILE_OSCILLOGRAPH 1
#define PROFILE_PIN A2

using namespace config::launchConfig;

extern "C" char *sbrk(int incr);
int freeMemory() {
    char top;
    return &top - reinterpret_cast<char *>(sbrk(0));
}

Rov::Rov()
    : tele(new RovTelemetry), control(new RovControl),
      rawControl(new RovControl), auxControl(new RovAuxControl) {
    Wire.begin();
    Wire.setTimeout(1000);
    Wire.setClock(10000);

    SerialUSB.begin(115200);
    unsigned int t_on = millis() + config::serial::waitForSerialTime;

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PROFILE_PIN, OUTPUT);
    analogWrite(LED_BUILTIN, 100);

    if (config::serial::waitForSerial) {
        while (!SerialUSB) {
            if (millis() >= t_on) {
                break;
            } else {
                analogWrite(LED_BUILTIN, sin(millis() * 0.03) * 127 + 127);
                delay(1);
            }
        }
        delay(200);
    }
    digitalWrite(LED_BUILTIN, 0);
    Logger::info(F("HighROV init!\n\r"));

    launchConfig curConf = currentConfig;

    // switch (curConf) {
    //     case fast: //launch everything, test nothing
    //         Logger::info(F("Fast startup selected"));
    //         break;
    //     case full: //launch and test everything
    //         Logger::info(F("Full startup selected"));
    //         break;
    //     case forceDepth: //don't launch anything except for depth sensor
    //         Logger::info(F("Standalone startup with forced depth sensor init
    //         selected")); break;
    //     case forceEthernet: //don't launch anything except for ethernet
    //         Logger::info(F("Standalone startup with forced networking init
    //         selected")); break;
    //     case standalone: //don't launch anything
    //         Logger::info(F("Standalone startup selected"));
    //         break;
    //     default:
    //         Logger::info(F("Unknown launch value"));
    //         // while (true);
    // }
    long init_ms_begin = 0;
    thrusters =
        new Thrusters(curConf & (full | fast), curConf & full, init_ms_begin);
    cameras = new Cameras(curConf & (full | fast), curConf & full);
    imu = new IMUSensor(curConf & (full | fast), curConf & full);
    manipulator = new Manipulator(curConf & (full | fast), curConf & full);
    networking = new Networking(curConf & initEthernet,
                                curConf & (full | forceEthernet));
    regulators = new RovRegulators();
    sensors = new Sensors(curConf & (full | fast), curConf & full,
                          (curConf & initDepth) && !(curConf & forceNoDepth));
    debug = new Debug(tele, control, auxControl);
    Logger::debug("Waiting for " + String(9000 - (millis() - init_ms_begin)) +
                  "ms\n\r");
    delay(9000 - (millis() - init_ms_begin));
}

void Rov::serialHandler() {
    if (SerialUSB.available()) {
        String msg = SerialUSB.readString();
        msg.trim();
        if (msg == "reset") {
            Logger::info(F(
                "Resetting the controller, please reconnect the debug cable or "
                "reactivate serial monitor if you want to continue "
                "debugging\n\r"));
            SerialUSB.end();
            sensors->end();
            imu->end();
            NVIC_SystemReset();
        } else if (msg == "debug")
            asm("nop"); // TODO: implement debug menu
        else
            Logger::info(
                F("send \"reset\" for controller reset or \"debug\" for "
                  "debug menu [NIY]"));
    }
}
void Rov::loop() {
    long long micros_p = micros();
#if PROFILE_OSCILLOGRAPH
    digitalWrite(PROFILE_PIN, 0);
#endif
    serialHandler();
    sensors->update();
    imu->update();

#if PROFILE
    long long micros_s = micros();
#endif
    tele->yaw = imu->getYaw();
    tele->roll = imu->getRoll();
    tele->pitch = imu->getPitch();
    tele->depth = sensors->getDepth();
    tele->temperature = sensors->getTemperature();
    tele->current = sensors->getCurrent();
    tele->voltage = sensors->getVoltage();
    tele->cameraIndex = control->camsel;
    if (config::launchConfig::currentConfig &
        config::launchConfig::initEthernet) {
        networking->maintain();
        if (networking->getLinkStatus()) {
            networking->readRovControl(*rawControl, *auxControl);
            networking->writeRovTelemetry(*tele);
        }
    }
    memcpy(control, rawControl, sizeof(RovControl));
    *control = regulators->evaluate(*control, *auxControl, *tele);
    thrusters->update(*control);
#if PROFILE > 0
    long long micros_nt = micros();
#endif
    cameras->set_angle_delta(
        0, constrain(control->cameraRotationDelta[0], -1, 1) * 3.0);
    cameras->set_angle_delta(
        1, constrain(control->cameraRotationDelta[1], -1, 1) * 3.0);
    cameras->select_cam(control->camsel == 1);
    manipulator->setOpenClose(control->manipulatorOpenClose);
    manipulator->setRotate(control->manipulatorRotate);

    analogWrite(LED_BUILTIN, abs((int16_t(millis() % 512)) - 256));
#if PROFILE > 0
    // Logger::trace("s: " + String(uint16_t(micros_s - micros_p)) + ";\t\ts to
    // nt: " + String(uint16_t(micros_nt - micros_s)) + ";\t\tnt to end: " +
    // String(uint16_t(micros() - micros_nt)) + ";\t\tfull: " +
    // String(uint16_t(micros() - micros_p)));
#endif
    // Logger::trace(String(10000 - (micros() - micros_p)));
    debug->debugHandler();
    int del = 7500 - (micros() - micros_p);
#if PROFILE_OSCILLOGRAPH
    digitalWrite(PROFILE_PIN, 1);
#endif
    delayMicroseconds(del > 0 ? del : 1);
}

#endif