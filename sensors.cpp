#ifndef SENSORS_CPP
#define SENSORS_CPP
#include "sensors.h"
#include "USB/USBAPI.h"
#include "config.h"
#include "logger.h"


Sensors::Sensors(bool launch, bool test, bool ds_init){
    if(!ds_init) return;

    Logger::info(F("Depth sensor init"));
    m_depthSensorEnabled = ds_init;
    m_depthSensor.setModel(MS5837::MS5837_30BA);
    m_depthSensor.setFluidDensity(997); // kg/m^3 (997 for freshwater, 1029 for seawater)
    if (ds_init){
        if (!m_depthSensor.init()) {
            Logger::warn(F("Depth sensor init failed! Will retry init on next read"));
            return;
        }
    }
}

void Sensors::update() {
    using namespace config::sensors;

    int rawVoltage = analogRead(voltmeter_pin);
    int rawAmperage = analogRead(ammeter_pin);

    float curVoltage = round(rawVoltage * (voltage_multiplier * 100.0)) / 100.0;
    float curAmperage = round((rawAmperage - amperage_deflection) * (amperage_multiplier * 100.0)) / 100.0;

    m_voltage += curVoltage;
    m_voltage /= 2;
    m_current += curAmperage;
    m_current /= 2;

    if (m_depthSensorEnabled && (millis() - m_lastDepthUpdateMs) > 50) {
        m_depthSensor.read();
        m_lastDepthUpdateMs = millis();

        if (abs(m_depthSensor.depth()) < 150) {
            m_depth = m_depthSensor.depth();
            m_temp = m_depthSensor.temperature();
        } else { // don't accept value if value is too big (probably corrupted data)
            Logger::warn(F("Depth sensor read error. Retrying init"));

            // trying to reset I2C and init sensor again.
            Wire.end();
            Wire.begin();
            Wire.setTimeout(1000);
            Wire.setClock(10000);
            m_depthSensor.init();
        }
    }
}

void Sensors::end(){
    Wire.end();
}

float Sensors::getDepth()
{
    return m_depth;
}
float Sensors::getTemperature()
{
    return m_temp;
}
float Sensors::getCurrent()
{
    return m_current;
}
float Sensors::getVoltage()
{
    return m_voltage;
}

#endif