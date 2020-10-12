/*
 * DeviceConfigurationService.h
 *
 *  Created on: Jan 18, 2019
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_DEVICE_CONFIGURATION_SERVICE_H_
#define COMPONENTS_SENSE_DEVICE_CONFIGURATION_SERVICE_H_

#include <Arduino.h>
#if defined(CONFIG_BT_ENABLED)
#include <BLEUUID.h>
#include <BLEDevice.h>
#include <Sense.h>
#include <WiFi.h>

enum CharacterOptions
{
	WiFiMac,
	LocalIP,
	WiFiSSID,
	WiFiPwd,
	DeviceName,
	Device,
	Room,
	Floor,
	Tenant,
	CrudusToken,
	MQTTTopic,
	MQTTHost,
	MQTTPort,
	MQTTUser,
	MQTTPwd,
	CaliTemp,
	SoftReset
};
class DeviceConfigurationService : public BLECharacteristicCallbacks
{
public:
	DeviceConfigurationService(BLEServer *pServer);
	virtual ~DeviceConfigurationService();
	void setup(BLEAdvertising *pAdvertising);

	void onRead(BLECharacteristic *pCharacteristic);
	void onWrite(BLECharacteristic *pCharacteristic);

private:
	BLECharacteristic *m_pCharacteristicLevel;
	BLEServer *m_pServer;
	std::map<std::string, BLEUUID> m_serviceuuidMap;
	std::map<CharacterOptions, BLEUUID> m_charuuidMap;
	void createWiFiMacCharacteristic(BLEService *pService);
	void createDeviceCharacteristic(BLEService *pService);
	void createMQTTCharacteristic(BLEService *pService);
	void createCaliCharacteristic(BLEService *pService);

}; // DeviceConfigurationService

#endif // CONFIG_BT_ENABLED
#endif // COMPONENTS_SENSE_DEVICE_CONFIGURATION_SERVICE_H_
