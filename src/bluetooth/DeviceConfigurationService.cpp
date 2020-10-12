/*
 * DeviceConfigurationService.cpp
 *
 *  Created on: Jan 18, 2019
 *      Author: danielkaldheim
 */

#include "DeviceConfigurationService.h"
#if defined(CONFIG_BT_ENABLED)

DeviceConfigurationService::DeviceConfigurationService(BLEServer *pServer)
{
    m_pServer = pServer;

    m_serviceuuidMap = {
        std::make_pair("DCService", BLEUUID("24464c92-69ee-11e9-a923-1681be663d3e")),
        std::make_pair("WiFiService", BLEUUID("23464c92-69ee-11e9-a923-1681be663d3e")),
        std::make_pair("MQTTService", BLEUUID("22464c92-69ee-11e9-a923-1681be663d3e")),
        std::make_pair("CalibrationService", BLEUUID("21464c92-69ee-11e9-a923-1681be663d3e"))};

    m_charuuidMap = {
        std::make_pair(DeviceName, BLEUUID("5759f8cc-69ee-11e9-8a12-1681be663d3e")),
        std::make_pair(WiFiMac, BLEUUID("51ecb1ca-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(LocalIP, BLEUUID("51ec81c8-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(WiFiSSID, BLEUUID("51ecb440-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(WiFiPwd, BLEUUID("51ecb594-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(Device, BLEUUID("51ecc52a-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(Room, BLEUUID("51ecb6ca-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(Floor, BLEUUID("51ecb7f6-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(Tenant, BLEUUID("51ecb922-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(MQTTTopic, BLEUUID("51ecba4e-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(MQTTHost, BLEUUID("51ecbf26-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(MQTTPort, BLEUUID("51ecc156-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(MQTTUser, BLEUUID("51ecc2c8-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(MQTTPwd, BLEUUID("51ecc3fe-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(CrudusToken, BLEUUID("51ecc6d8-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(CaliTemp, BLEUUID("51ecca5c-6b85-11e9-a923-1681be663d3e")),
        std::make_pair(SoftReset, BLEUUID("51eccd18-6b85-11e9-a923-1681be663d3e"))};
}

DeviceConfigurationService::~DeviceConfigurationService()
{
}

void DeviceConfigurationService::setup(BLEAdvertising *pAdvertising)
{
    /**
     * Wifi Service
     **/
    BLEService *pWiFiService = m_pServer->createService(m_serviceuuidMap["WiFiService"]);
    createWiFiMacCharacteristic(pWiFiService);
    pWiFiService->start();
    pAdvertising->addServiceUUID(m_serviceuuidMap["WiFiService"]);

    /**
     * MQTT Service
     **/
    BLEService *pMQTTService = m_pServer->createService(m_serviceuuidMap["MQTTService"]);
    createMQTTCharacteristic(pMQTTService);
    pMQTTService->start();
    pAdvertising->addServiceUUID(m_serviceuuidMap["MQTTService"]);

    /**
     * Device Config Service
     **/
    BLEService *pService = m_pServer->createService(m_serviceuuidMap["DCService"]);
    createDeviceCharacteristic(pService);
    pService->start();
    pAdvertising->addServiceUUID(m_serviceuuidMap["DCService"]);

    /**
     * Calibration Service
     **/
    BLEService *pCaliService = m_pServer->createService(m_serviceuuidMap["CalibrationService"]);
    createCaliCharacteristic(pCaliService);
    pCaliService->start();
    pAdvertising->addServiceUUID(m_serviceuuidMap["CalibrationService"]);
}

void DeviceConfigurationService::onRead(BLECharacteristic *pCharacteristic)
{
    for (std::pair<CharacterOptions, BLEUUID> element : m_charuuidMap)
    {
        if (pCharacteristic->getUUID().equals(element.second))
        {
            switch (element.first)
            {
            case WiFiPwd:
            case MQTTPwd:
            case CrudusToken:
            case SoftReset:
                break;
            case DeviceName:
                pCharacteristic->setValue(Sense::getDeviceName().c_str());
                break;
            case WiFiMac:
                pCharacteristic->setValue(Sense::getWiFiMac().c_str());
                break;
            case LocalIP:
                pCharacteristic->setValue(Sense::getLocalIp().c_str());
                break;
            case WiFiSSID:
                pCharacteristic->setValue(Sense::getWiFiSSID().c_str());
                break;
            case Device:
                pCharacteristic->setValue(Sense::getDevice().c_str());
                break;
            case Room:
                pCharacteristic->setValue(Sense::getRoom().c_str());
                break;
            case Floor:
                pCharacteristic->setValue(Sense::getFloor().c_str());
                break;
            case Tenant:
                pCharacteristic->setValue(Sense::getTenant().c_str());
                break;
            case MQTTTopic:
                pCharacteristic->setValue(Sense::getMQTTTopic().c_str());
                break;
            case MQTTHost:
                pCharacteristic->setValue(Sense::getMQTTHost().c_str());
                break;
            case MQTTPort:
                pCharacteristic->setValue(String(Sense::getMQTTPort()).c_str());
                break;
            case MQTTUser:
                pCharacteristic->setValue(Sense::getMQTTUser().c_str());
                break;
            case CaliTemp:
                pCharacteristic->setValue(String(Sense::getCaliTemp()).c_str());
                break;
            }
            break;
        }
    }
}

void DeviceConfigurationService::onWrite(BLECharacteristic *pCharacteristic)
{
    if (pCharacteristic->getUUID().equals(m_charuuidMap[SoftReset]))
    {
        // Clear preferences!
        Sense::clearPreferences();
    }
    else
    {
        String value = String(pCharacteristic->getValue().c_str());
        if (value.length() > 0)
        {
            for (std::pair<CharacterOptions, BLEUUID> element : m_charuuidMap)
            {
                if (pCharacteristic->getUUID().equals(element.second))
                {
                    switch (element.first)
                    {
                    case WiFiMac:
                    case LocalIP:
                    case SoftReset:
                        break;
                    case DeviceName:
                        Sense::setDeviceName(value);
                        break;
                    case WiFiSSID:
                        Sense::setWiFiSSID(value);
                        break;
                    case WiFiPwd:
                        Sense::setWiFiPwd(value);
                        break;
                    case Device:
                        Sense::setDevice(value);
                        break;
                    case Room:
                        Sense::setRoom(value);
                        break;
                    case Floor:
                        Sense::setFloor(value);
                        break;
                    case Tenant:
                        Sense::setTenant(value);
                        break;
                    case MQTTTopic:
                        Sense::setMQTTTopic(value);
                        break;
                    case MQTTHost:
                        Sense::setMQTTHost(value);
                        break;
                    case MQTTPort:
                        Sense::setMQTTPort(value.toInt());
                        break;
                    case MQTTUser:
                        Sense::setMQTTUser(value);
                        break;
                    case MQTTPwd:
                        Sense::setMQTTPwd(value);
                        break;
                    case CrudusToken:
                        Sense::setCrudusToken(value);
                        break;
                    case CaliTemp:
                        Sense::setCaliTemp(value.toFloat());
                        break;
                    }
                    break;
                }
            }
        }
    }
}

void DeviceConfigurationService::createWiFiMacCharacteristic(BLEService *pService)
{
    BLECharacteristic *pCharacteristic1 = pService->createCharacteristic(
        m_charuuidMap[WiFiMac],
        BLECharacteristic::PROPERTY_READ);
    pCharacteristic1->setValue(Sense::getWiFiMac().c_str());

    BLECharacteristic *pCharacteristic2 = pService->createCharacteristic(
        m_charuuidMap[LocalIP],
        BLECharacteristic::PROPERTY_READ);
    pCharacteristic2->setValue(Sense::getWiFiMac().c_str());

    BLECharacteristic *pCharacteristic3 = pService->createCharacteristic(
        m_charuuidMap[WiFiSSID],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic3->setCallbacks(this);
    pCharacteristic3->setValue(Sense::getWiFiSSID().c_str());

    BLECharacteristic *pCharacteristic4 = pService->createCharacteristic(
        m_charuuidMap[WiFiPwd],
        BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic4->setCallbacks(this);
}

void DeviceConfigurationService::createDeviceCharacteristic(BLEService *pService)
{
    BLECharacteristic *pCharacteristic1 = pService->createCharacteristic(
        m_charuuidMap[DeviceName],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic1->setCallbacks(this);
    pCharacteristic1->setValue(Sense::getDeviceName().c_str());

    BLECharacteristic *pCharacteristic2 = pService->createCharacteristic(
        m_charuuidMap[Device],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic2->setCallbacks(this);
    pCharacteristic2->setValue(Sense::getDevice().c_str());

    BLECharacteristic *pCharacteristic3 = pService->createCharacteristic(
        m_charuuidMap[Room],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic3->setCallbacks(this);
    pCharacteristic3->setValue(Sense::getRoom().c_str());

    BLECharacteristic *pCharacteristic4 = pService->createCharacteristic(
        m_charuuidMap[Floor],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic4->setCallbacks(this);
    pCharacteristic4->setValue(Sense::getFloor().c_str());

    BLECharacteristic *pCharacteristic5 = pService->createCharacteristic(
        m_charuuidMap[Tenant],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic5->setCallbacks(this);
    pCharacteristic5->setValue(Sense::getTenant().c_str());

    BLECharacteristic *pCharacteristic6 = pService->createCharacteristic(
        m_charuuidMap[CrudusToken],
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic6->setCallbacks(this);

    BLECharacteristic *pCharacteristic7 = pService->createCharacteristic(
        m_charuuidMap[SoftReset],
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic7->setCallbacks(this);
}

void DeviceConfigurationService::createMQTTCharacteristic(BLEService *pService)
{
    BLECharacteristic *pCharacteristic1 = pService->createCharacteristic(
        m_charuuidMap[MQTTTopic],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic1->setCallbacks(this);
    pCharacteristic1->setValue(Sense::getMQTTTopic().c_str());

    BLECharacteristic *pCharacteristic2 = pService->createCharacteristic(
        m_charuuidMap[MQTTHost],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic2->setCallbacks(this);
    pCharacteristic2->setValue(Sense::getMQTTHost().c_str());

    BLECharacteristic *pCharacteristic3 = pService->createCharacteristic(
        m_charuuidMap[MQTTPort],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic3->setCallbacks(this);
    pCharacteristic3->setValue(String(Sense::getMQTTPort()).c_str());

    BLECharacteristic *pCharacteristic4 = pService->createCharacteristic(
        m_charuuidMap[MQTTUser],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic4->setCallbacks(this);
    pCharacteristic4->setValue(Sense::getMQTTUser().c_str());

    BLECharacteristic *pCharacteristic5 = pService->createCharacteristic(
        m_charuuidMap[MQTTPwd],
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic5->setCallbacks(this);
}

void DeviceConfigurationService::createCaliCharacteristic(BLEService *pService)
{
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        m_charuuidMap[CaliTemp],
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setCallbacks(this);
    pCharacteristic->setValue(String(Sense::getCaliTemp()).c_str());
}

#endif // CONFIG_BT_ENABLED
