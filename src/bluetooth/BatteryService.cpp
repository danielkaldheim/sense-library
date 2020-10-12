/*
 * BatteryService.cpp
 *
 *  Created on: Jan 18, 2019
 *      Author: danielkaldheim
 */

#include "BatteryService.h"
#ifdef COMP_BATTERY
#if defined(CONFIG_BT_ENABLED)

BatterySensor bs(ADC1_CHANNEL_7);

BatteryService::BatteryService(BLEServer *pServer)
{
    m_pServer = pServer;
}

BatteryService::~BatteryService()
{
}

void BatteryService::setup(BLEAdvertising *pAdvertising)
{
    BLEService *pService = m_pServer->createService(getServiceUUID());

    m_pCharacteristicLevel = pService->createCharacteristic(
        getCharacteristicUUID(),
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE);

    m_pCharacteristicLevel->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Add to advertising
    pAdvertising->addServiceUUID(getServiceUUID());
}

void BatteryService::notify()
{
    bs.performReading();
    m_pCharacteristicLevel->setValue(bs.percentage);
    m_pCharacteristicLevel->notify();
}

BLEUUID BatteryService::getServiceUUID()
{
    return BLEUUID::fromString("0x180F");
}
BLEUUID BatteryService::getCharacteristicUUID()
{
    return BLEUUID::fromString("0x2A19");
}

void BatteryService::setBatteryLevel(int level)
{
    m_pCharacteristicLevel->setValue(level);
    m_pCharacteristicLevel->notify();
}

#endif // CONFIG_BT_ENABLED
#endif