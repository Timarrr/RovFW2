#ifndef IMU_H
#define IMU_H
#include "Uart.h"
#include <Arduino.h>
#define IMU_MAX_PACKET_LEN 256

namespace IMUHelpers {
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
}
class IMUSensor{
public:
    IMUSensor(bool launch, bool test);
    float getPitch();
    float getYaw();
    float getRoll();
    void update();
    void end();
private:
    IMUHelpers::Packet_t RxPkt;
    uint8_t ID;
    int16_t AccRaw[3] = {0,0,0};
    int16_t GyoRaw[3] = {0,0,0};
    int16_t MagRaw[3] = {0,0,0};
    float Euler[3] = {0.0,0.0,0.0};
    void imuCrc16Update(uint16_t *currentCrc, const uint8_t *src, uint32_t lengthInBytes);
    uint32_t imuPacketDecode(uint8_t c);
    void imuUpdateEuler(IMUHelpers::Packet_t * pkt);
};

#endif