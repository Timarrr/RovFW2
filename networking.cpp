#ifndef NETWORKING_CPP
#define NETWORKING_CPP
#include "networking.h"
#include "Ethernet.h"
#include "logger.h"

EthernetUDP udp = EthernetUDP();

Networking::Networking(bool launch, bool test){
    if (!launch) return;
	Logger::info(F("Ethernet init... "));
    Ethernet.init(m_csPin);
	Logger::info(F(Ethernet.begin(mac, 10000, 5000)==1 ? "Success!\n\r" : "Failed!\n\r"));
	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
	  Logger::warn(F("Ethernet controller was not found. If the next message is \"Ethernet init... Success!\" then you can ignore this message\n\r"));
	}
	else if (Ethernet.hardwareStatus() == EthernetW5100) {
	  Logger::info(F("W5100 Ethernet controller detected.\n\r"));
	}
	else if (Ethernet.hardwareStatus() == EthernetW5200) {
	  Logger::info(F("W5200 Ethernet controller detected.\n\r"));
	}
	else if (Ethernet.hardwareStatus() == EthernetW5500) {
	  Logger::info(F("W5500 Ethernet controller detected.\n\r"));
	}
	udp.begin(m_selfPort);
}

String Networking::getConfig(){
    return "Target self IP:      " + String(m_selfIp) +
	   "\n\rCurrent self IP: " + String(Ethernet.localIP()) +
	   "\n\rDNS server IP:       " + String(Ethernet.dnsServerIP()) +
	   "\n\rGateway IP:          " + String(Ethernet.gatewayIP()) +
	   "\n\rSubnet mask:         " + String(Ethernet.subnetMask()) +
	   "\n\rLink status:         " + String((Ethernet.linkStatus() ? "ON" : "OFF"));
}
//returs true if status == LinkON
bool Networking::getLinkStatus(){
	return Ethernet.linkStatus() == LinkON;
}

void Networking::maintain(){
	if (Ethernet.linkStatus() == LinkON && m_linkStatus != LinkON) {
		Logger::info(F("Link status: On"));
		m_linkStatus = LinkON;
	}
	else if (Ethernet.linkStatus() == LinkOFF && m_linkStatus != LinkOFF) {
		Logger::info(F("Link status: Off. This usually indicates problems with cable"));
		m_linkStatus = LinkOFF;
	}
	else if (Ethernet.linkStatus() == Unknown && m_linkStatus != Unknown) {
		Logger::info(F("Link status: Unknown."));
		m_linkStatus = Unknown;
	}
	if ((m_linkStatus == LinkON && (millis() >= m_lastMaintainTime + 500)) || millis() >= m_lastMaintainTime + 5000){//if link is up -> check every half a second, otherwise check once every 5 seconds
		byte status = Ethernet.maintain();
		switch(status){
			case 0:
				break;
			case 1:
				Logger::warn(F("DHCP lease renew failed, check the configuration of your DHCP server"));
				break;
			case 2:
				Logger::info(F("DHCP lease renew success"));
				break;
			case 3:
				Logger::warn(F("DHCP rebind failed, check the configuration of your DHCP server"));
				break;
			case 4:
				if(m_linkStatus)
				{
					Logger::warn(F("DHCP rebind detected, this almost certainly will break things"));
				}
				else
				{
					Logger::warn("Binding to DHCP address " + (String) Ethernet.localIP() + " after downtime, check the IP and reboot the router and/or the ROV if it is incorrect.\n\r" + \
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

    uint8_t buffer[64] = {};

    RovTelemetry rt = tel; 
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
	int packetSize = udp.parsePacket();

	if (packetSize) {
		udp.read(buffer, size);
		return packetSize;
	}
	return 0;
}

void Networking::write(uint8_t * buffer, int size) {
	udp.beginPacket(m_remoteIp, m_remotePort);
	udp.write(buffer, size);
	udp.endPacket();
}

#endif