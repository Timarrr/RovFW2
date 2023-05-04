#ifndef SENSORS_CPP
#define SENSORS_CPP
#include "sensors.h"
#include "USB/USBAPI.h"
#include "api/Common.h"
#include "config.h"
#include "logger.h"
#include "SERCOM.h"
#include <cmath>
#include <cstdint>

Sensors::Sensors(bool launch, bool test, bool ds_init) {
    if (!ds_init)
        return;

    Logger::info(F("Depth sensor init\n\r"));
    m_depthSensorEnabled = ds_init;
    m_depthSensor.setFluidDensity(
        997); // kg/m^3 (997 for freshwater, 1029 for seawater)
    if (!m_depthSensor.init()) {
        Logger::warn(
            F("Depth sensor init failed! Will retry init on next read"));
        return;
    }
}

void Sensors::update() {
    using namespace config::sensors;

    int rawVoltage  = analogRead(voltmeter_pin);
    int rawAmperage = analogRead(ammeter_pin);

    m_voltage += rawVoltage * voltage_multiplier;
    m_voltage /= 2;
    m_current += (rawAmperage - amperage_deflection) * amperage_multiplier;
    m_current /= 2;

    if (m_depthSensorEnabled) {
        m_depthSensor.loop();
        if ((abs(m_depthSensor.temperature()) < 70.0f && abs(m_depthSensor.depth())) < 4.0f || micros() < 20000000) {
            m_depth = m_depthSensor.depth();
            m_temp  = m_depthSensor.temperature();
            Logger::warn("Depth value: " + String(m_depthSensor.depth()));
            Logger::warn("Temprerature value: " + String(m_depthSensor.temperature()));
        } else { // don't accept value if value is too big (probably
                 // corrupted data)
            Logger::warn(F("Depth sensor read error. Retrying init\n\r"));
            Logger::warn("Depth value: " + String(m_depthSensor.depth()));
            Logger::warn("Temprerature value: " + String(m_depthSensor.temperature()));

            // trying to reset I2C and init sensor again.
            Wire.end();
            Wire.begin();
            Wire.setTimeout(1000);
            Wire.setClock(10000);
            m_depthSensor.init();
        }

    } else {
        m_depth = MAXFLOAT;
    }
}

void Sensors::end() { Wire.end(); }

float Sensors::getDepth() { return m_depth; }
float Sensors::getTemperature() { return m_temp; }
float Sensors::getCurrent() { return m_current; }
float Sensors::getVoltage() { return m_voltage; }

#endif