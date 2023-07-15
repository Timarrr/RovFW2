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
      auxControl(new RovAuxControl) {
    Wire.begin();
    Wire.setTimeout(1000);
    Wire.setClock(10000);

    SerialUSB.begin(2000000);
    SerialUSB.setTimeout(50);
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
            }
        }
        delay(200);
    }
    Logger::info(F("HighROV init!\n\r"));

    launchConfig curConf = currentConfig;

    if (curConf & fast) { // launch everything, test nothing
        Logger::info(F("Fast startup selected\n\r"));
    }
    if (curConf & full) { // launch and test everything
        Logger::info(F("Full startup selected\n\r"));
    }
    if (curConf & forceDepth) { // force depth sensor init no matter what
        Logger::info(F("Forced depth sensor init selected\n\r"));
        if (curConf & (fast | full)) {
            Logger::info(
                F("Unnescessary forceDepth flag, remove it in config.h\n\r"));
        }
    }
    if (curConf & forceThrusterTest) {
        Logger::info(F("Forced thruster testing selected"));
        if (curConf & full) {
            Logger::info(
                F("Unnescessary forceThrusterTest flag, remove it in config"));
        }
    }
    if (curConf & forceEthernet) { // force ethernet init no matter what
        Logger::info(F("Forced networking init selected\n\r"));
        if (curConf & (fast | full)) {
            Logger::info(F(
                "Unnescessary forceEthernet flag, remove it in config.h\n\r"));
        }
    }
    if (curConf & standalone) { // don't launch anything
        if (curConf & (fast | full)) {
            Logger::info(F("Standalone setup cancelled because of fast or full "
                           "flags in config.h\n\r"));
        }
        Logger::info(F("Standalone startup selected\n\r"));
    }

    long timr   = 0x80000000;
    thrusters   = new Thrusters(curConf & (full | fast | forceThrusterTest),
                                curConf & (full | forceThrusterTest), timr);
    cameras     = new Cameras(curConf & (full | fast), curConf & full);
    imu         = new IMUSensor(curConf & (full | fast), curConf & full);
    manipulator = new Manipulator(curConf & (full | fast), curConf & full);
    networking =
        new Networking(curConf & initEthernet && !(curConf & forceNoEthernet),
                       curConf & (full | initEthernet));
    sensors    = new Sensors(curConf & (full | fast), curConf & full,
                             (curConf & initDepth) && !(curConf & forceNoDepth));
    debug      = new Debug(tele, control, auxControl);

    int32_t wait_until = 9000 + timr;

    Logger::debug("Waiting until millis() >= " + String(wait_until, 10) +
                  "\n\r");
    bool inc       = true;
    int  update_on = millis();
    analogWrite(PIN_LED, 0);
    while ((int32_t)wait_until > (int32_t)millis()) {
        if (millis() >= update_on) {
            if (inc) {
                digitalWrite(PIN_LED_RXL, 0);
                digitalWrite(PIN_LED_TXL, 1);
                inc = false;
            } else {
                digitalWrite(PIN_LED_RXL, 1);
                digitalWrite(PIN_LED_TXL, 0);
                inc = true;
            }
            update_on = millis() + 60;
        }
    }
    digitalWrite(PIN_LED_RXL, 0);
    digitalWrite(PIN_LED_TXL, 0);

    Logger::debug(F("Launching main loop...\n\r"));
}
void Rov::serialHandler() {
    if (SerialUSB.available()) {
        Logger::info(F("Detected user input, suspending execution... "));
        String msg;
        while (!msg.endsWith("\n") && !msg.endsWith("\r")) {
            if (SerialUSB.available()) {
                char pending = SerialUSB.read();
                Logger::info(String(pending), false);
                switch (pending) {
                case 27: // ESC
                    Logger::info(
                        F("\n\rDetected char 27 - ESC, dropping input and "
                          "returning to main loop"));
                    return;
                    break;
                case 127:
                    if (msg.length() <= 0)
                        continue;
                    msg.remove(msg.length() - 1);
                    Logger::info("\b \b", false);
                    continue;
                    break;
                default:
                    break;
                }
                msg += pending;
            }
        }
        Logger::info("\n\r");
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
            debug->menu(); // TODO: implement debug menu
        else {
            Logger::info(
                F("send \"reset\" for controller reset or \"debug\" for "
                  "debug menu, continuing execution\n\r"));
        }
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
    tele->yawCal      = imu->getYawCal();
    tele->rollCal     = imu->getRollCal();
    tele->pitchCal    = imu->getPitchCal();
    tele->accel0      = imu->getAccel0();
    tele->accel1      = imu->getAccel1();
    tele->accel2      = imu->getAccel2();
    tele->depth       = sensors->getDepth();
    tele->temp        = sensors->getTemperature();
    tele->current     = sensors->getCurrent();
    tele->voltage     = sensors->getVoltage();
    tele->cameraIndex = control->camsel;

    if (config::launchConfig::currentConfig &
        config::launchConfig::initEthernet) {
        networking->maintain();
        if (networking->getLinkStatus()) {
            networking->readRovControl(*control, *auxControl);
            networking->writeRovTelemetry(*tele);
        }
    }
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

    if (auxControl->auxFlags.imuInvCal) {
        imu->invalidateCalibration();
        auxControl->auxFlags.imuInvCal = 0;
    }

    // digitalWrite(LIGHT_PIN, auxControl->auxFlags.eLight); // enable light
    // digitalWrite(PUMP_PIN, auxControl->auxFlags.ePump);   // enable pump
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
    digitalWrite(PROFILE_OSCILLOGRAPH_PIN, 1);
#endif
    delayMicroseconds(del > 0 ? del : 1);
}

#endif