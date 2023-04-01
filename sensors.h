/**
 * @file sensors.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Analog and depth sensor subsystem
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SENSORS_H
#define SENSORS_H
#include "config.h"
#include "logger.h"
#include "wiring_private.h"
#include <Arduino.h>
#include <MS5837.h>
#include <Wire.h>
#include <math.h>

/**
 * @brief Class responsible for sensors subsystem
 *
 */
class Sensors {
  public:
    /**
     * @brief Construct a new Sensors object
     *
     * @param launch Should this subsystem actually launch?
     * @param test Do we need to test the subsystem?
     * @param ds_init Do we need to launch the MS5837 depth sensor?
     */
    Sensors(bool launch, bool test, bool ds_init = true);

    /**
     * @brief Get depth data
     *
     * @return float Depth
     */
    float getDepth();

    /**
     * @brief Get temperature data
     *
     * @return float Temperature
     */
    float getTemperature();

    /**
     * @brief Get electric current data
     *
     * @return float Current
     */
    float getCurrent();

    /**
     * @brief Get voltage data
     *
     * @return float Voltage
     */
    float getVoltage();

    /**
     * @brief Update sensors data
     *
     */
    void update();

    /**
     * @brief Turn off the sensor subsystem
     *
     */
    void end();

  private:
    /**
     * @brief Voltage onboard the ROV provided by the analog sensor
     *
     */
    float m_voltage = 0.0;

    /**
     * @brief Current consumed by the ROV provided by the analog sensor
     *
     */
    float m_current = 0.0;

    /**
     * @brief MS5837 depth sensor
     *
     */
    MS5837 m_depthSensor;

    /**
     * @brief Depth provided by the MS5837
     *
     */
    float m_depth = 0.0;

    /**
     * @brief Temperature provided by the MS5837
     *
     */
    float m_temp = 0.0;

    /**
     * @brief Last depth sensor update time in millis
     *
     */
    long long m_lastDepthUpdateMs = 0;

    /**
     * @brief Do we need to work with the depth sensor?
     *
     */
    bool m_depthSensorEnabled = false;
};

#endif