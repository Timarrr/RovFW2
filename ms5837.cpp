#ifndef MS5837_CPP
#define MS5837_CPP
#include "ms5837.h"
#include "api/Common.h"
#include "samd21/include/component/tc.h"

#define MS5837_ADDR 0x76
#define MS5837_RESET 0x1E
#define MS5837_ADC_READ 0x00
#define MS5837_PROM_READ 0xA0
#define MS5837_CONVERT_D1_8192 0x4A
#define MS5837_CONVERT_D1_4096 0x48
#define MS5837_CONVERT_D2_8192 0x5A
#define MS5837_CONVERT_D2_4096 0x58

bool MS5837Atomic::init() {
    // Reset the MS5837, per datasheet
    Wire.beginTransmission(MS5837_ADDR);
    Wire.write(MS5837_RESET);
    Wire.endTransmission();

    // Wait for reset to complete
    delay(10);

    // Read calibration values and CRC
    for (uint8_t i = 0; i < 7; i++) {
        Wire.beginTransmission(MS5837_ADDR);
        Wire.write(MS5837_PROM_READ + i * 2);
        Wire.endTransmission();

        Wire.requestFrom(MS5837_ADDR, 2);
        C[i] = (Wire.read() << 8) | Wire.read();
    }

    // Verify that data is correct with CRC
    uint8_t crcRead = C[0] >> 12;
    uint8_t crcCalculated = crc4(C);

    if (crcCalculated == crcRead) {
        return true; // Initialization success
    }

    return false; // CRC fail
}

void MS5837Atomic::loop() {
    int32_t dT = 0;
    int32_t Ti = 0;
    int64_t OFF = 0;
    int32_t OFFi = 0;
    int64_t SENS = 0;
    int32_t SENSi = 0;
    int64_t OFF2 = 0;
    int64_t SENS2 = 0;

    switch (state) {

    case requestD1:
        // Request D1 conversion
        Wire.beginTransmission(MS5837_ADDR);
        Wire.write(0x4A);
        Wire.endTransmission();
        state = acquireD1;
        time = micros();
        break;
    case acquireD1:
        if (time + 10000 < micros())
            break;

        Wire.beginTransmission(MS5837_ADDR);
        Wire.write(MS5837_ADC_READ);
        Wire.endTransmission();

        Wire.requestFrom(MS5837_ADDR, 3);
        D1 = 0;
        D1 = Wire.read();
        D1 = (D1 << 8) | Wire.read();
        D1 = (D1 << 8) | Wire.read();

        state = requestD2;
        break;
    case requestD2:
        // Request D2 conversion
        Wire.beginTransmission(MS5837_ADDR);
        Wire.write(0x5A);
        Wire.endTransmission();
        state = acquireD2;
        break;
    case acquireD2:
        if (time + 10000 < micros())
            break;

        Wire.beginTransmission(MS5837_ADDR);
        Wire.write(MS5837_ADC_READ);
        Wire.endTransmission();

        Wire.requestFrom(MS5837_ADDR, 3);
        D2 = 0;
        D2 = Wire.read();
        D2 = (D2 << 8) | Wire.read();
        D2 = (D2 << 8) | Wire.read();
        state = requestD1;
        break;
    case convertDD:
        // Terms called
        dT = D2 - uint32_t(C[5]) * 256l;
        SENS = int64_t(C[1]) * 32768l + (int64_t(C[3]) * dT) / 256l;
        OFF = int64_t(C[2]) * 65536l + (int64_t(C[4]) * dT) / 128l;
        P = (D1 * SENS / (2097152l) - OFF) / (8192l);
        // Temp conversion
        TEMP = 2000l + int64_t(dT) * C[6] / 8388608LL;

        // Second order compensation
        if ((TEMP / 100) < 20) { // Low temp
            Ti = (3 * int64_t(dT) * int64_t(dT)) / (8589934592LL);
            OFFi = (3 * (TEMP - 2000) * (TEMP - 2000)) / 2;
            SENSi = (5 * (TEMP - 2000) * (TEMP - 2000)) / 8;
            if ((TEMP / 100) < -15) { // Very low temp
                OFFi = OFFi + 7 * (TEMP + 1500l) * (TEMP + 1500l);
                SENSi = SENSi + 4 * (TEMP + 1500l) * (TEMP + 1500l);
            }
        } else if ((TEMP / 100) >= 20) { // High temp
            Ti = 2 * (dT * dT) / (137438953472LL);
            OFFi = (1 * (TEMP - 2000) * (TEMP - 2000)) / 16;
            SENSi = 0;
        }

        OFF2 = OFF - OFFi; // Calculate pressure and temp second order
        SENS2 = SENS - SENSi;

        TEMP = (TEMP - Ti);
        P = (((D1 * SENS2) / 2097152l - OFF2) / 8192l);
    }
}
#endif