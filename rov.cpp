#ifndef ROV_CPP
#define ROV_CPP
#include "rov.h"
#include "config.h"
#include "logger.h"
#include "rovdatatypes.h"
#include "variant.h"
#include <cstdint>

#define PROFILE 0

#define PROFILE_OSCILLOGRAPH     0
#define PROFILE_OSCILLOGRAPH_PIN -1

#define LIGHT_PIN A2
#define PUMP_PIN  A3

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
    pinMode(PROFILE_OSCILLOGRAPH_PIN, OUTPUT);
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(LIGHT_PIN, OUTPUT);
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
    Logger::info(F("HighROV init!\n\r"));

    launchConfig curConf = currentConfig;

    if (curConf & fast) { // launch everything, test nothing
        Logger::info(F("Fast startup selected"));
    }
    if (curConf & full) { // launch and test everything
        Logger::info(F("Full startup selected"));
    }
    if (curConf & forceDepth) { // force depth sensor init no matter what
        Logger::info(F("Forced depth sensor init selected"));
        if (curConf & (fast | full)) {
            Logger::info(
                F("Unnescessary forceDepth flag, remove it in config.h"));
        }
    }
    if (curConf & forceEthernet) { // force ethernet init no matter what
        Logger::info(F("Forced networking init selected"));
        if (curConf & (fast | full)) {
            Logger::info(
                F("Unnescessary forceEthernet flag, remove it in config.h"));
        }
    }
    if (curConf & standalone) { // don't launch anything
        if (curConf & full) {
            Logger::info(F("Standalone setup cancelled because of fast or full "
                           "flags in config.h"));
        }
        Logger::info(F("Standalone startup selected"));
    }

    long timr   = 0x80000000;
    thrusters   = new Thrusters(curConf & (full | fast), curConf & full, timr);
    cameras     = new Cameras(curConf & (full | fast), curConf & full);
    imu         = new IMUSensor(curConf & (full | fast), curConf & full);
    manipulator = new Manipulator(curConf & (full | fast), curConf & full);
    networking =
        new Networking(curConf & initEthernet && !(curConf & forceNoEthernet),
                       curConf & (full | forceEthernet));
    regulators = new RovRegulators();
    sensors    = new Sensors(curConf & (full | fast), curConf & full,
                             (curConf & initDepth) && !(curConf & forceNoDepth));
    debug      = new Debug(tele, control, auxControl);

    long long wait_while = millis() + 9000 - (millis() - timr);

    while (wait_while > millis()) {
        analogWrite(LED_BUILTIN, sin(millis() * 0.03) * 127 + 127);
        delay(3);
    }
    // Logger::debug("Waiting for " + String(time) + "ms\n\r");
    // delay(time > 0 ? time : 1);
    Logger::debug(F("Launching main loop...\n"));
}

void Rov::serialHandler() {
    if (SerialUSB.available()) {
        String msg = SerialUSB.readString();
        msg.trim();
        if (msg == "reset") {
            Logger::info(
                F("Resetting the controller, please reconnect the debug "
                  "cable or "
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
    int       i        = 0;
#if PROFILE_OSCILLOGRAPH
    digitalWrite(PROFILE_OSCILLOGRAPH_PIN, 0);
#endif
    serialHandler();
    sensors->update();
    imu->update();

#if PROFILE
    long long micros_s = micros();
#endif
    tele->yaw         = imu->getYaw();
    tele->roll        = imu->getRoll();
    tele->pitch       = imu->getPitch();
    tele->depth       = sensors->getDepth();
    tele->temp        = sensors->getTemperature();
    tele->current     = sensors->getCurrent();
    tele->voltage     = sensors->getVoltage();
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

    // digitalWrite(LIGHT_PIN, auxControl->auxFlags.eLight); // enable light
    // digitalWrite(PUMP_PIN, auxControl->auxFlags.ePump);   // enable pump

    int val = abs((int16_t(millis() % 512)) - 256);
    Logger::debug(String(val, 10));
    pinMode(LED_BUILTIN, OUTPUT);
    analogWrite(LED_BUILTIN, 1024);
    Logger::debug(String(analogRead(PIN_LED)));
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
    digitalWrite(PROFILE_OSCILLOGRAPH_PIN, 1);
#endif
    delayMicroseconds(del > 0 ? del : 1);
}

#endif