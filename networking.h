#ifndef NETWORKING_H
#define NETWORKING_H
#include <Arduino.h>
#include <Ethernet.h>
#include "config.h"
#include "helpers.h"
#include "rovdatatypes.h"

class Networking{
public:
    Networking();
    Networking(bool launch, bool test);
    void maintain();
    void init();
    void readRovControl(RovControl &ctrl);
    void writeRovTelemetry(RovTelemetry &tel);
    String printStatus();
private:
    int read(uint8_t *buffer, int size);
    void write(uint8_t *buffer, int size);
    String ip_decoder(uint32_t addr);

    byte mac[6] = {
        0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
    };
    EthernetUDP m_udp;


    IPAddress m_selfIp = config::net::rovIP;
    uint16_t m_selfPort = config::net::rovPort; 
    IPAddress m_remoteIp = config::net::rcIP;
    uint16_t m_remotePort = config::net::rcPort;
    short m_csPin = config::net::netControllerCsPin;
    long long m_lastMaintainTime;
    bool m_linkUp;
};

#endif