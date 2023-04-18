#ifndef IMU_CPP
#define IMU_CPP
#include "imu.h"
#include "USB/USBAPI.h"
#include "api/Common.h"
#include "logger.h"
#include "wiring_private.h"

using namespace IMUHelpers;

Uart SerialImu(&sercom1, 12, 11, SERCOM_RX_PAD_3, UART_TX_PAD_0);

void SERCOM1_Handler() { SerialImu.IrqHandler(); }

IMUSensor::IMUSensor(bool launch, bool test) {
    Logger::info("IMU init\n\r");
    SerialImu.begin(115200);
    delay(15);
    pinPeripheral(11, PIO_SERCOM);
    pinPeripheral(12, PIO_SERCOM);
}

void IMUSensor::imuCrc16Update(uint16_t *currentCrc, const uint8_t *src,
                               uint32_t lengthInBytes) {
    uint32_t crc = *currentCrc;
    uint32_t j;
    for (j = 0; j < lengthInBytes; ++j) {
        uint32_t i;
        uint32_t byte = src[j];
        crc ^= byte << 8;
        for (i = 0; i < 8; ++i) {
            uint32_t temp = crc << 1;
            if (crc & 0x8000) {
                temp ^= 0x1021;
            }
            crc = temp;
        }
    }
    *currentCrc = crc;
}

/**
 * @brief fuck...
 *
 *
 * @param c
 */
void IMUSensor::imuPacketDecode(uint8_t c) {
    static uint16_t CRCReceived = 0;      /* CRC value received from a frame */
    static uint16_t CRCCalculated = 0;    /* CRC value caluated from a frame */
    static uint8_t status = kStatus_Idle; /* state machine */
    static uint8_t crc_header[4] = {0x5A, 0xA5, 0x00, 0x00};
    // SerialUSB.println(String(c, 2));
    switch (status) {
    case kStatus_Idle:
        // SerialUSB.println("k_Idle");
        if (c == 0x5A)
            status = kStatus_Cmd;
        break;
    case kStatus_Cmd:
        // SerialUSB.println("k_Cmd");
        RxPkt.type = c;
        if (c == 0xA5) {
            status = kStatus_LenLow;
        } else {
            status = kStatus_Idle;
        }
        break;
    case kStatus_LenLow:
        // SerialUSB.println("k_LenLow");
        RxPkt.payload_len = c;
        crc_header[2] = c;
        status = kStatus_LenHigh;
        break;
    case kStatus_LenHigh:
        // SerialUSB.println("k_LenHigh");
        RxPkt.payload_len |= (c << 8);
        crc_header[3] = c;
        status = kStatus_CRCLow;
        break;
    case kStatus_CRCLow:
        // SerialUSB.println("k_crcLow");
        CRCReceived = c;
        status = kStatus_CRCHigh;
        break;
    case kStatus_CRCHigh:
        // SerialUSB.println("k_crcHigh");
        CRCReceived |= (c << 8);
        RxPkt.ofs = 0;
        CRCCalculated = 0;
        status = kStatus_Data;
        break;
    case kStatus_Data:
        // SerialUSB.println("k_Data");
        if (RxPkt.ofs >= IMU_MAX_PACKET_LEN) {
            status = kStatus_Idle;
            RxPkt.ofs = 0;
            CRCCalculated = 0;
            break;
        }

        RxPkt.buf[RxPkt.ofs++] = c;

        if (RxPkt.ofs >= RxPkt.payload_len && RxPkt.type == 0xA5) {
            imuCrc16Update(&CRCCalculated, crc_header, 4);
            imuCrc16Update(&CRCCalculated, RxPkt.buf, RxPkt.ofs);

            /* CRC match */
            if (CRCCalculated == CRCReceived) {
                imuUpdateEuler(&RxPkt);
            } else {
                Logger::error(F("IMU CRC mismatch: "));
                Logger::error(String(CRCReceived, HEX) + " vs " +
                              String(CRCCalculated, HEX) + "\n\r");
            }

            status = kStatus_Idle;
        }
        break;
    default:
        // SerialUSB.println("k_Default");
        status = kStatus_Idle;
        break;
    }
    // SerialUSB.println("end");
}

void IMUSensor::imuUpdateEuler(Packet_t *pkt) {
    if (pkt->buf[0] == kItemID) /* user ID */
    {
        ID = pkt->buf[1];
    }
    if (pkt->buf[2] == kItemAccRaw) /* Acc raw value */
    {
        memcpy(AccRaw, (uint8_t *)pkt->buf[3], 6);
    }

    if (pkt->buf[9] == kItemGyoRaw) /* gyro raw value */
    {
        memcpy(GyoRaw, (uint8_t *)pkt->buf[10], 6);
    }

    if (pkt->buf[16] == kItemMagRaw) /* mag raw value */
    {
        memcpy(MagRaw, (uint8_t *)pkt->buf[17], 6);
    }
    if (pkt->buf[23] == kItemAtdE) /* atd E */
    {
        float val = ((float)(int16_t)(pkt->buf[24] + (pkt->buf[25] << 8))) / 100;
        if (!isnan(val)) {
            Euler[0] = val;
        }
        val = ((float)(int16_t)(pkt->buf[26] + (pkt->buf[27] << 8))) / 100;
        if (!isnan(val)) {
            Euler[1] = val;
        }
        val = ((float)(int16_t)(pkt->buf[28] + (pkt->buf[29] << 8))) / 10;
        if (!isnan(val)) {
            Euler[2] = val;
        }
    }
}

void IMUSensor::update() {
    if (SerialImu.available()) {
        while (SerialImu.available()) {
            char ch = SerialImu.read();
            this->imuPacketDecode(ch);
        }
    }
}

void IMUSensor::end() { SerialImu.end(); }
//axes swap: pitch is roll
float IMUSensor::getPitch() { return Euler[1]; }
float IMUSensor::getRoll() { return Euler[0] + 180; }
float IMUSensor::getYaw() { return Euler[2] + 180; }

#endif