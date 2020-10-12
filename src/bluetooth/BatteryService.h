/*
 * BatteryService.h
 *
 *  Created on: Jan 18, 2019
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_BATTERY_SERVICE_H_
#define COMPONENTS_SENSE_BATTERY_SERVICE_H_
#include "SenseConfig.h"
#ifdef COMP_BATTERY
#ifdef CONFIG_BT_ENABLED
#include <Arduino.h>
#include <BLEUUID.h>
#include <BLEDevice.h>
#include <BLE2902.h>
#include <BatterySensor.h>

class BatteryService
{
public:
	BatteryService(BLEServer *pServer);
	virtual ~BatteryService();
	void setup(BLEAdvertising *pAdvertising);
	void notify();
	void setBatteryLevel(int level);
	BLEUUID getServiceUUID();
	BLEUUID getCharacteristicUUID();

private:
	BLECharacteristic *m_pCharacteristicLevel;
	BLEServer *m_pServer;
}; // BatteryService

#endif // CONFIG_BT_ENABLED
#endif
#endif // COMPONENTS_SENSE_BATTERY_SERVICE_H_