#ifndef NETWORKING_CPP
#define NETWORKING_CPP
#include "networking.h"
#include "utility/w5100.h"
#include "helpers.h"
#include "rovdatatypes.h"
#include <cstddef>

Networking::Networking(bool launch, bool test){
    if (!launch) return;
    Ethernet.init(m_csPin);
	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
	  SerialUSB.println("Ethernet controller was not found. If the next message is \"Ethernet init... Success!\" then you can ignore this message");
      SerialUSB.println(W5100.getChip());
	}
	else if (Ethernet.hardwareStatus() == EthernetW5100) {
	  SerialUSB.println("W5100 Ethernet controller detected.");
	}
	else if (Ethernet.hardwareStatus() == EthernetW5200) {
	  SerialUSB.println("W5200 Ethernet controller detected.");
	}
	else if (Ethernet.hardwareStatus() == EthernetW5500) {
	  SerialUSB.println("W5500 Ethernet controller detected.");
	}
	SerialUSB.print("Ethernet init... ");
	SerialUSB.println(Ethernet.begin(mac, 10000, 5000)==1 ? "Success!" : "Failed!");
	m_udp.begin(m_selfPort);
}

String Networking::printStatus(){
    SerialUSB.print("Target self IP:     "); SerialUSB.println(m_selfIp);
	SerialUSB.print("Current self IP:    "); SerialUSB.println(Ethernet.localIP());
	SerialUSB.print("DNS server IP:      "); SerialUSB.println(Ethernet.dnsServerIP());
	SerialUSB.print("Gateway IP:         "); SerialUSB.println(Ethernet.gatewayIP());
	SerialUSB.print("Subnet mask:        "); SerialUSB.println(Ethernet.subnetMask());
	SerialUSB.print("Link status:        "); SerialUSB.println((Ethernet.linkStatus() ? "ON" : "OFF"));
}

void Networking::maintain(){
	if (Ethernet.linkStatus() == LinkON && !m_linkUp) {
		SerialUSB.println("Link status: On");
		m_linkUp = true;
	}
	else if (Ethernet.linkStatus() == LinkOFF && m_linkUp) {
		SerialUSB.println("Link status: Off. This usually indicates problems with cable");
		m_linkUp = false;
	}
	if ((m_linkUp && (millis() >= m_lastMaintainTime + 500)) || millis() >= m_lastMaintainTime + 5000){//if link is up -> check every half a second, otherwise check once every 5 seconds
		byte status = Ethernet.maintain();
		switch(status){
			case 0:
				break;
			case 1:
				SerialUSB.println("DHCP lease renew failed, check the configuration of your DHCP server");
				break;
			case 2:
				SerialUSB.println("DHCP lease renew success");
				break;
			case 3:
				SerialUSB.println("DHCP rebind failed, check the configuration of your DHCP server");
				break;
			case 4:
				if(m_linkUp)
				{
					SerialUSB.println("DHCP rebind detected, this almost certainly will break things");
				}
				else
				{
					String ip = (String) Ethernet.localIP();
					SerialUSB.println("Binding to DHCP address " + ip + " after downtime, check the IP and reboot the router and the ROV if it is incorrect\n\r" + \
										 "If problem persists, check your router and PC's settings");
				}
				break;
			default:
				break;
		}
		m_lastMaintainTime = millis();
	}	
}
void Networking::readRovControl(RovControl &ctrl) {
	uint8_t buffer[32];
	int size = read(buffer, 32);
	
	if (size > 0) {
        size_t i = 2; // skip header and version
        ctrl.version = 2;
        helpers::read_bytes(buffer, i, ctrl.auxFlags);
        helpers::read_bytes(buffer, i, ctrl.thrusterPower);
        helpers::read_bytes(buffer, i, ctrl.manipulator[1]);
        helpers::read_bytes(buffer, i, ctrl.manipulator[2]);
        helpers::read_bytes(buffer, i, ctrl.cameraRotation[0]);
        helpers::read_bytes(buffer, i, ctrl.cameraRotation[1]);
        helpers::read_bytes(buffer, i, ctrl.camsel);
        
	}
}  

void Networking::writeRovTelemetry(RovTelemetry &tel) {
    size_t i = 0;

    uint8_t buffer[64];

    RovTelemetry rt = RovTelemetry(tel); 
    helpers::swapEndianRovTelemetry(rt);
    helpers::write_bytes(buffer, i, rt.header_telemetry);
    helpers::write_bytes(buffer, i, rt.version);
    helpers::write_bytes(buffer, i, rt.depth);
    helpers::write_bytes(buffer, i, rt.pitch);
    helpers::write_bytes(buffer, i, rt.yaw);
    helpers::write_bytes(buffer, i, rt.roll);
    helpers::write_bytes(buffer, i, rt.ammeter);
    helpers::write_bytes(buffer, i, rt.voltmeter);
    helpers::write_bytes(buffer, i, rt.cameraIndex);
    helpers::write_bytes(buffer, i, rt.temperature);
	write(buffer, i);
}

int Networking::read(uint8_t * buffer, int size) {
	int packetSize = m_udp.parsePacket();

	if (packetSize) {
		m_udp.read(buffer, size);
		return packetSize;
	}
	return 0;
}

void Networking::write(uint8_t * buffer, int size) {
	m_udp.beginPacket(m_remoteIp, m_remotePort);
	m_udp.write(buffer, size);
	m_udp.endPacket();
}

#endif