/*
 * SenseBLE.h
 *
 *  Created on: Jan 18, 2019
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_BLE_H_
#define COMPONENTS_SENSE_BLE_H_

#include <Arduino.h>
#include <SenseConfig.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Sense.h>
#if defined(CONFIG_BT_ENABLED)
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertisedDevice.h>

#include <BLEUUID.h>
#include <BLEDevice.h>

#include <BLEScan.h>

#include <bluetooth/BatteryService.h>
#include <bluetooth/DeviceConfigurationService.h>

class SenseBLE
{
public:
    SenseBLE();
    virtual ~SenseBLE();
    void begin(void);
    void loop();
    void setMQTTClient(PubSubClient MQTTClient);

private:
    void deviceInformationService(BLEAdvertising *pAdvertising);
    BLEServer *m_pServer;
    // PubSubClient *m_senseMQTT;
}; // SenseBLE

#endif // CONFIG_BT_ENABLED
#endif // COMPONENTS_SENSE_BLE_H_
