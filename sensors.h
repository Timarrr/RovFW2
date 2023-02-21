#ifndef SENSORS_H
#define SENSORS_H
#define ANALOG_SAMPLES_COUNT 50
#define IMU_MAX_PACKET_LEN 256
#include <Arduino.h>
#include <MS5837.h>
#include <Wire.h>
#include "config.h"
#include "wiring_private.h"
#include <math.h>

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
    float getTemp();
    float getAmperage();
    float getVoltage();
    float getPitch();
    float getYaw();
    float getRoll();
    void update();
    void end(); 

private:
    //IMU
    enum status {
        kStatus_Idle,
        kStatus_Cmd,
        kStatus_LenLow,
        kStatus_LenHigh,
        kStatus_CRCLow,
        kStatus_CRCHigh,
        kStatus_Data,
    };

    typedef enum {
        kItemKeyStatus = 0x80,   	/* key status           size: 4 */
        kItemID = 0x90,   			/* user programed ID    size: 1 */
        kItemUID = 0x91,   			/* Unique ID            size: 4 */
        kItemIPAdress = 0x92,   	/* ip address           size: 4 */
        kItemAccRaw = 0xA0,   		/* raw acc              size: 3x2 */
        kItemAccRawFiltered = 0xA1,
        kItemAccLinear = 0xA2,
        kItemAccCalibrated = 0xA3,
        kItemGyoRaw = 0xB0,   		/* raw gyro             size: 3x2 */
        kItemGyoRawFiltered = 0xB1,
        kItemGyoCalibrated = 0xB3,
        kItemMagRaw = 0xC0,   		/* raw mag              size: 3x2 */
        kItemMagRawFiltered = 0xC1,
        kItemMagCalibrated = 0xC3,
        kItemAtdE = 0xD0,   		/* eular angle          size:3x2 */
        kItemAtdQ = 0xD1,   		/* att q,               size:4x4 */
        kItemTemp = 0xE0,
        kItemPressure = 0xF0,   	/* pressure             size:1x4 */
        kItemEnd = 0xFF,
    } ItemID_t;

    typedef struct Packet_t {
        uint32_t ofs;
        uint8_t buf[IMU_MAX_PACKET_LEN] = {0};
        uint16_t payload_len;
        uint16_t len;
        uint8_t type;
    } Packet_t;

    Packet_t RxPkt;
    uint8_t ID;
    int16_t AccRaw[3] = {0};
    int16_t GyoRaw[3] = {0};
    int16_t MagRaw[3] = {0};
    float Eular[3] = {0.0};
    int32_t m_imuPressure;
    void imuCrc16Update(uint16_t *currentCrc, const uint8_t *src, uint32_t lengthInBytes);
    uint32_t imuPacketDecode(uint8_t c);
    void imuUpdateEuler(Packet_t * pkt);

    MS5837 m_depthSensor;
    float m_depth = 0.0;
    float m_temp = 0.0;
    float m_voltage = 0.0;
    float m_amperage = 0.0;

    long long m_lastDepthUpdateMs = 0;
    short m_analog_samples_counter;
    double m_voltage_samples[ANALOG_SAMPLES_COUNT] = { 0.0 };
    double m_amperage_samples[ANALOG_SAMPLES_COUNT] = { 0.0 };
    Uart SerialImu = Uart(&sercom1, 12, 11, SERCOM_RX_PAD_3, UART_TX_PAD_0);
    void SERCOM1_Handler() {
        SerialImu.IrqHandler();
        this->update();
    }

};

#endif