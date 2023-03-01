#ifndef SENSORS_H
#define SENSORS_H
#define ANALOG_SAMPLES_COUNT 50
#include <Arduino.h>
#include <MS5837.h>
#include <Wire.h>
#include "config.h"
#include "wiring_private.h"
#include <math.h>
#include "logger.h"

class Sensors{
public:
    Sensors();
    ~Sensors();
    Sensors(const Sensors &) = default;
    Sensors(Sensors &&) = default;
    Sensors &operator=(const Sensors &) = default;
    Sensors &operator=(Sensors &&) = default;
    Sensors(bool launch, bool test, bool ds_init = true);
    float getDepth();
    float getTemperature();
    float getCurrent();
    float getVoltage();
    void update();
    void end();
private:
    //analog things
    float m_voltage = 0.0;
    float m_current = 0.0;

    // MS5837 depth sensor
    MS5837 m_depthSensor; 
    float m_depth = 0.0;
    float m_temp = 0.0;
    long long m_lastDepthUpdateMs = 0;
    bool m_depthSensorEnabled = false;
};

#endif