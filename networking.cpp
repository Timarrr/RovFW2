#ifndef NETWORKING_CPP
#define NETWORKING_CPP
#include "networking.h"
#include "Ethernet.h"
#include "USB/USBAPI.h"
#include "api/Stream.h"
#include "config.h"
#include "helpers.h"
#include "logger.h"
#include "rovdatatypes.h"
#include <cstddef>
#include <cstdint>

EthernetUDP udp = EthernetUDP();

Networking::Networking(bool launch, bool test) : m_linkStatus(Unknown) {
    if (!launch)
        return;
    Logger::info(F("Ethernet init... "));
    Ethernet.init(m_csPin);
    Logger::info(F(Ethernet.begin(m_macAddress, 10000, 5000) == 1
                       ? "Success!\n\r"
                       : "Failed!\n\r"),
                 false);
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Logger::warn(F("Ethernet controller was not found. If the next message "
                       "is \"Ethernet init... Success!\" then you can ignore "
                       "this message\n\r"));
    } else if (Ethernet.hardwareStatus() == EthernetW5100) {
        Logger::info(F("W5100 Ethernet controller detected.\n\r"));
    } else if (Ethernet.hardwareStatus() == EthernetW5200) {
        Logger::info(F("W5200 Ethernet controller detected.\n\r"));
    } else if (Ethernet.hardwareStatus() == EthernetW5500) {
        Logger::info(F("W5500 Ethernet controller detected.\n\r"));
    }
    udp.begin(m_selfPort);
}

bool Networking::getLinkStatus() { return Ethernet.linkStatus() == LinkON; }

void Networking::maintain() {
    EthernetLinkStatus lstat = Ethernet.linkStatus();
    if (lstat == LinkON && m_linkStatus != LinkON) {
        Logger::info("Link status: On");
        m_linkStatus = LinkON;
    } else if (lstat == LinkOFF && m_linkStatus != LinkOFF) {
        Logger::info(F("Link status: Off. This usually indicates problems with "
                       "cable\n\r"));
        m_linkStatus = LinkOFF;
    } else if (lstat == Unknown && m_linkStatus != Unknown) {
        Logger::info(F("Link status: Unknown\n\r"));
        m_linkStatus = Unknown;
    }
    if ((m_linkStatus == LinkON && (millis() >= m_lastMaintainTime + 500)) ||
        millis() >= m_lastMaintainTime +
                        5000) { // if link is up -> check every half a second,
                                // otherwise check once every 5 seconds
        byte status = Ethernet.maintain();

        switch (status) {
        case 0:
            break;
        case 1:
            Logger::warn(F("DHCP lease renew failed, check the configuration "
                           "of your DHCP server\n\r"));
            break;
        case 2:
            Logger::info(F("DHCP lease renew success\n\r"));
            break;
        case 3:
            Logger::warn(F("DHCP rebind failed, check the configuration of "
                           "your DHCP server\n\r"));
            break;
        case 4:
            if (m_linkStatus) {
                Logger::warn(F("DHCP rebind detected, this almost certainly "
                               "will break things\n\r"));
            } else {
                Logger::warn(
                    "Binding to DHCP address " + (String)Ethernet.localIP() +
                    " after downtime, check the IP and reboot the router "
                    "and/or the ROV if it is incorrect.\n\r" +
                    "If problem persists, check your router and PC's "
                    "settings\n\r");
            }
            break;
        default:
            break;
        }
        m_lastMaintainTime = millis();
    }
}

size_t Networking::readRovControl(RovControl &ctrl, RovAuxControl &auxCtrl) {
    uint8_t buffer[32];
    int     size         = 0;
    int     packets      = -1;
    size_t  data_counter = 0;
    int8_t  header       = 0;
    do {
        size = read(buffer, 32);

        helpers::read_bytes(buffer, data_counter, header);
        if (header == ctrl.header) { // yup that's a control message
            data_counter += 1;       // skip version
            helpers::read_bytes(buffer, data_counter, ctrl.thrusterPower);
            helpers::read_bytes(buffer, data_counter,
                                ctrl.manipulatorOpenClose);
            helpers::read_bytes(buffer, data_counter, ctrl.manipulatorRotate);
            helpers::read_bytes(buffer, data_counter,
                                ctrl.cameraRotationDelta[0]);
            helpers::read_bytes(buffer, data_counter,
                                ctrl.cameraRotationDelta[1]);
            helpers::read_bytes(buffer, data_counter, ctrl.camsel);
        } else if (header ==
                   auxCtrl.header) { // yup that's an auxControl message
            helpers::read_bytes(buffer, data_counter,
                                auxCtrl.auxFlags.rawFlags);
            helpers::read_bytes(buffer, data_counter, auxCtrl.dDepth);
            auxCtrl.dDepth = helpers::swapEndian(auxCtrl.dDepth);
            helpers::read_bytes(buffer, data_counter, auxCtrl.dYaw);
            auxCtrl.dYaw = helpers::swapEndian(auxCtrl.dYaw);
            helpers::read_bytes(buffer, data_counter, auxCtrl.dRoll);
            auxCtrl.dRoll = helpers::swapEndian(auxCtrl.dRoll);
            helpers::read_bytes(buffer, data_counter, auxCtrl.dPitch);
            auxCtrl.dPitch = helpers::swapEndian(auxCtrl.dPitch);
        }
        packets++;
    } while (size > 0);
    return packets;
}

void Networking::writeRovTelemetry(RovTelemetry &tele) {
    size_t i = 0;

    uint8_t buffer[128] = {};

    RovTelemetry rt = tele;
    helpers::swapEndianRovTelemetry(rt);
    helpers::write_bytes(buffer, i, rt.header);
    helpers::write_bytes(buffer, i, rt.version);
    helpers::write_bytes(buffer, i, rt.depth);
    helpers::write_bytes(buffer, i, rt.pitch);
    helpers::write_bytes(buffer, i, rt.yaw);
    helpers::write_bytes(buffer, i, rt.roll);
    helpers::write_bytes(buffer, i, rt.current);
    helpers::write_bytes(buffer, i, rt.voltage);
    helpers::write_bytes(buffer, i, rt.cameraIndex);
    helpers::write_bytes(buffer, i, rt.temperature);

    write(buffer, i);
}

int Networking::read(uint8_t *buffer, int size) {
    int packetSize = udp.parsePacket();
    udp.read(buffer, size);
    return packetSize;
}

void Networking::write(uint8_t *buffer, int size) {
    udp.beginPacket(m_remoteIp, m_remotePort);
    udp.write(buffer, size);
    udp.endPacket();
}

#endif