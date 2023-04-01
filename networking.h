/**
 * @file networking.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Communication with the RC
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef NETWORKING_H
#define NETWORKING_H
#include "config.h"
#include "helpers.h"
#include "logger.h"
#include "rovdatatypes.h"
#include <Arduino.h>
#include <Ethernet.h>

/**
 * @brief Class responsible for communication with RC
 *
 */
class Networking {
  public:
    /**
     * @brief Construct a new Networking object
     *
     * @param launch Should this subsystem actually launch?
     * @param test Do we need to test the subsystem?
     */
    Networking(bool launch, bool test);

    /**
     * @brief Maintain connection and DHCP lease
     *
     */
    void maintain();

    /**
     * @brief Reads (if available) RovControl and RovAuxControl from the UDP
     * socket
     *
     * @param ctrl Reference to RovControl object
     * @param auxCtrl Reference to RovAuxControl object
     */
    void readRovControl(RovControl &ctrl, RovAuxControl &auxCtrl);

    /**
     * @brief Writes RovTelemetry to the UDP socket
     *
     * @param tele Telemetry
     */
    void writeRovTelemetry(RovTelemetry tele);

    /**
     * @brief Get physical link status
     *
     * @return true Link is up
     * @return false Link is down
     */
    bool getLinkStatus();

  private:
    /**
     * @brief Reads up to size bytes from the socket into the buffer
     *
     * @param buffer Output buffer
     * @param size Maximum size to read
     * @return int Size of read data
     */
    int read(uint8_t *buffer, int size);

    /**
     * @brief Writes size bytes from the buffer to the socket
     *
     * @param buffer Input buffer
     * @param size Size of the data to write
     */
    void write(uint8_t *buffer, int size);
    /**
     * @brief MAC address of the controller
     *
     */
    byte m_macAddress[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
    /**
     * @see config::net::rovIP
     */
    IPAddress m_selfIp = config::net::rovIP;

    /**
     * @see config::net::rovPort
     */
    uint16_t m_selfPort = config::net::rovPort;

    /**
     * @see config::net::rcIP
     */
    IPAddress m_remoteIp = config::net::rcIP;

    /**
     * @see config::net::rcPort
     */
    uint16_t m_remotePort = config::net::rcPort;

    /**
     * @see config::net::netControllerCsPin
     */
    short m_csPin = config::net::netControllerCsPin;

    /**
     * @brief last maintain() call time
     */
    long long m_lastMaintainTime;

    /**
     * @brief Physical link status
     *
     */
    EthernetLinkStatus m_linkStatus;
};

#endif