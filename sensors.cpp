#ifndef SENSORS_CPP
#define SENSORS_CPP
#include "sensors.h"


Sensors::Sensors(bool launch, bool test, bool ds_init){
    if(!ds_init) return;

    SerialUSB.println("Depth sensor init");
    bool ok = m_depthSensor.init();
    if (!ok) {
        SerialUSB.println("Depth sensor init failed! Will retry init on next read");
        return;
    }

    m_depthSensor.setModel(MS5837::MS5837_30BA);
    m_depthSensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)

    SerialUSB.println("IMU init");
    SerialImu.begin(115200);
    pinPeripheral(11, PIO_SERCOM);
    pinPeripheral(12, PIO_SERCOM);
    // SERIAL_BUFFER_SIZE
}

void Sensors::update() {
    using namespace config::sensors;

    int rawVoltage = analogRead(voltmeter_pin);
    int rawAmperage = analogRead(ammeter_pin);

    float curVoltage = round(rawVoltage * (voltage_multiplier * 100.0)) / 100.0;
    float curAmperage = round((rawAmperage - amperage_deflection) * (amperage_multiplier * 100.0)) / 100.0;

    m_voltage_samples[m_analog_samples_counter] = curVoltage;
    m_amperage_samples[m_analog_samples_counter] = curAmperage;

    float resultVoltage = 0.0;
    float resultAmperage = 0.0;

    for (int i = 0; i < ANALOG_SAMPLES_COUNT; i++) {
        resultVoltage += m_voltage_samples[i];
        resultAmperage += m_amperage_samples[i];
    }

    resultVoltage /= ANALOG_SAMPLES_COUNT;
    resultAmperage /= ANALOG_SAMPLES_COUNT;

    m_voltage = resultVoltage;
    m_amperage = resultAmperage;

    m_analog_samples_counter++;
    if (m_analog_samples_counter >= ANALOG_SAMPLES_COUNT) {
        m_analog_samples_counter = 0;
    }
    bool status = false; // false = error, true = ok

    if ((millis() - m_lastDepthUpdateMs) > 200) {
        m_depthSensor.read();
        m_lastDepthUpdateMs = millis();

        if (abs(m_depthSensor.depth()) < 150) {
            status = true;
            m_depth = m_depthSensor.depth();
            m_temp = m_depthSensor.temperature();
        } else { // don't accept value if value is too big (probably corrupted data)
            SerialUSB.println("Depth sensor read error. Retrying init");
            status = false;

            // trying to reset I2C and init sensor again.
            Wire.end();
            Wire.begin();
            Wire.setTimeout(1000);
            Wire.setClock(10000);
            init();
        }
    }
    if (SerialImu.available()) {
        while (SerialImu.available())
        {
            char ch = SerialImu.read();
            this->imuPacketDecode(ch);
        }
    }
}

void Sensors::imuCrc16Update(uint16_t *currentCrc, const uint8_t *src, uint32_t lengthInBytes)
{
    uint32_t crc = *currentCrc;
    uint32_t j;
    for (j = 0; j < lengthInBytes; ++j)
    {
        uint32_t i;
        uint32_t byte = src[j];
        crc ^= byte << 8;
        for (i = 0; i < 8; ++i)
        {
            uint32_t temp = crc << 1;
            if (crc & 0x8000)
            {
                temp ^= 0x1021;
            }
            crc = temp;
        }
    }
    *currentCrc = crc;
}


uint32_t Sensors::imuPacketDecode(uint8_t c)
{
    static uint16_t CRCReceived = 0; /* CRC value received from a frame */
    static uint16_t CRCCalculated = 0;  /* CRC value caluated from a frame */
    static uint8_t status = kStatus_Idle; /* state machine */
    static uint8_t crc_header[4] = { 0x5A, 0xA5, 0x00, 0x00 };

    switch (status)
    {
    case kStatus_Idle:
        if (c == 0x5A)
            status = kStatus_Cmd;
        break;
    case kStatus_Cmd:
        RxPkt.type = c;
        if (c == 0xA5) {
            status = kStatus_LenLow;
        } else {
            status = kStatus_Idle;
        }
        break;
    case kStatus_LenLow:
        RxPkt.payload_len = c;
        crc_header[2] = c;
        status = kStatus_LenHigh;
        break;
    case kStatus_LenHigh:
        RxPkt.payload_len |= (c << 8);
        crc_header[3] = c;
        status = kStatus_CRCLow;
        break;
    case kStatus_CRCLow:
        CRCReceived = c;
        status = kStatus_CRCHigh;
        break;
    case kStatus_CRCHigh:
        CRCReceived |= (c << 8);
        RxPkt.ofs = 0;
        CRCCalculated = 0;
        status = kStatus_Data;
        break;
    case kStatus_Data:
        if (RxPkt.ofs >= IMU_MAX_PACKET_LEN) {
            status = kStatus_Idle;
            RxPkt.ofs = 0;
            CRCCalculated = 0;
            break;
        }

        RxPkt.buf[RxPkt.ofs++] = c;

        if (RxPkt.ofs >= RxPkt.payload_len && RxPkt.type == 0xA5)
        {
            imuCrc16Update(&CRCCalculated, crc_header, 4);
            imuCrc16Update(&CRCCalculated, RxPkt.buf, RxPkt.ofs);

            /* CRC match */
            if (CRCCalculated == CRCReceived)
            {
                imuUpdateEuler(&RxPkt);
            } else {
                // SerialUSB.print("[IMU CRC FAIL] ");
                // SerialUSB.print("excepted: ");
                // SerialUSB.print(CRCReceived, HEX);
                // SerialUSB.print(", got: ");
                // SerialUSB.print(CRCCalculated, HEX);
                // SerialUSB.print("\n\r\t");
            }

            status = kStatus_Idle;
        }
        break;
    default:
        status = kStatus_Idle;
        break;
    }
}

void Sensors::imuUpdateEuler(Packet_t * pkt)
{
    if (pkt->buf[0] == kItemID) /* user ID */
    {
        ID = pkt->buf[1];
    }
    if (pkt->buf[2] == kItemAccRaw)  /* Acc raw value */
    {
        memcpy(AccRaw, (uint8_t *)pkt->buf[3], 6);
    }

    if (pkt->buf[9] == kItemGyoRaw)  /* gyro raw value */
    {
        memcpy(GyoRaw, (uint8_t *)pkt->buf[10], 6);
    }

    if (pkt->buf[16] == kItemMagRaw)  /* mag raw value */
    {
        memcpy(MagRaw, (uint8_t *)pkt->buf[17], 6);
    }
    if (pkt->buf[23] == kItemAtdE)  /* atd E */
    {
        float val = ((float)(int16_t)(pkt->buf[24] + (pkt->buf[25] << 8))) / 100;
        if (!isnan(val)) {
            Eular[0] = ((float)(int16_t)(pkt->buf[24] + (pkt->buf[25] << 8))) / 100;
        }
        val = ((float)(int16_t)(pkt->buf[26] + (pkt->buf[27] << 8))) / 100;
        if (!isnan(val)) {
            Eular[1] = ((float)(int16_t)(pkt->buf[26] + (pkt->buf[27] << 8))) / 100;
        }
        val = ((float)(int16_t)(pkt->buf[28] + (pkt->buf[29] << 8))) / 10;
        if (!isnan(val)) {
            Eular[2] = ((float)(int16_t)(pkt->buf[28] + (pkt->buf[29] << 8))) / 10;
        }
    }
}

float Sensors::getPitch()
{
    return Eular[0];
}
float Sensors::getRoll()
{
    return Eular[1];
}
float Sensors::getYaw()
{
    return Eular[2];
}

float Sensors::getDepth()
{
    return m_depth;
}
float Sensors::getTemp()
{
    return m_temp;
}
float Sensors::getAmperage()
{
    return m_amperage;
}
float Sensors::getVoltage()
{
    return m_voltage;
}

void Sensors::end(){
  SerialImu.end();
}

#endif