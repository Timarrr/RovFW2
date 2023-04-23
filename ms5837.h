#ifndef MS5837_H
#define MS5837_H
#include <Arduino.h>
#include <Wire.h>
#include <cstdint>
class MS5837Atomic{
    public:
    MS5837Atomic(){};

    void setFluidDensity(float density){fluidDensity=density;};

    bool init();

    void loop();


    private:
    uint16_t C[8];
	uint32_t D1, D2;
	int32_t TEMP;
	int32_t P;
	uint8_t _model;
    
    float fluidDensity;

	enum stateMachine {
		requestD1,
		requestD2,
		acquireD1,
		acquireD2,
		convertDD,
	} state;

	uint16_t time;

	/** Performs calculations per the sensor data sheet for conversion and
	 *  second order compensation.
	 */
	void calculate();

	uint8_t crc4(uint16_t n_prom[]);

};

#endif