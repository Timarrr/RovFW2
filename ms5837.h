#ifndef MS5837_H
#define MS5837_H
#include "logger.h"
#include <Arduino.h>
#include <Wire.h>
#include <cstdint>
class MS5837Atomic {
  public:
    MS5837Atomic(){};

    void setFluidDensity(float density) { fluidDensity = density; };

    bool init();

    void loop();

    float depth() { return (P * 10 - 101300) / (fluidDensity * 9.80665); };

    int32_t temperature() { return TEMP; };

  private:
    int32_t  dT    = 0;
    int32_t  Ti    = 0;
    int64_t  OFF   = 0;
    int32_t  OFFi  = 0;
    int64_t  SENS  = 0;
    int32_t  SENSi = 0;
    int64_t  OFF2  = 0;
    int64_t  SENS2 = 0;
    uint16_t C[8]  = {0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t D1    = 0;
    uint32_t D2    = 0;
    int32_t  TEMP  = 0;
    int32_t  P     = 0;

    float fluidDensity;

    enum stateMachine {
        requestD1,
        requestD2,
        acquireD1,
        acquireD2,
        convertDD,
    } state = requestD1;

    uint32_t time;

    /** Performs calculations per the sensor data sheet for conversion and
     *  second order compensation.
     */
    void calculate();

    uint8_t crc4(uint16_t n_prom[]);
};

#endif