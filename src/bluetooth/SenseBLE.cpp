/*
 * SenseBLE.cpp
 *
 *  Created on: Jan 18, 2019
 *      Author: danielkaldheim
 */

#include "SenseBLE.h"
#ifdef CONFIG_BT_ENABLED

#ifdef SCAN_BLE_DEVICES
uint32_t scanTime = 10;
unsigned long scanDelay = 2 * 45 * 1000;
unsigned long lastScanTime = 0;
BLEScan *pBLEScan;
#endif

#ifdef COMP_BATTERY
BatteryService *bService = NULL;
#endif
DeviceConfigurationService *dcService = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

PubSubClient p_senseMQTT;

class SenseBLEServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

#ifdef SCAN_BLE_DEVICES
class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        // Serial.printf("Advertised Device: %s RSSI: %d \n", advertisedDevice.toString().c_str(), advertisedDevice.getRSSI());
    }
};
#endif

SenseBLE::SenseBLE()
{
}

void SenseBLE::begin(void)
{
    #if defined(DEBUG)
    Serial.println("BLE begin");
    Serial.printf("\n****Before BLEDevice::deinit ESP.getFreeHeap() %u\n", ESP.getFreeHeap());
    #endif
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    BLEDevice::deinit(false);
    #if defined(DEBUG)
    Serial.printf("\n****After BLEDevice::deinit ESP.getFreeHeap() %u\n", ESP.getFreeHeap());
    #endif

    BLEDevice::init(Sense::getDeviceName().c_str());

    // uint8_t mac[6];
    // esp_err_t err = esp_read_mac(mac, ESP_MAC_BT);

    // BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
    // BLEDevice::setSecurityCallbacks(new MySecurity());

    m_pServer = BLEDevice::createServer();
    m_pServer->setCallbacks(new SenseBLEServerCallbacks());

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

    // Device Information Service
    this->deviceInformationService(pAdvertising);

#ifdef COMP_BATTERY
    // Battery Service
    bService = new BatteryService(m_pServer);
    bService->setup(pAdvertising);
#endif
    // Device configuration service
    dcService = new DeviceConfigurationService(m_pServer);
    dcService->setup(pAdvertising);

    pAdvertising->setScanResponse(false);
    // pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

    // BLESecurity *pSecurity = new BLESecurity();
    // pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);
    // pSecurity->setCapability(ESP_IO_CAP_OUT);
    // pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    BLEDevice::startAdvertising();
    #if defined(DEBUG)
    Serial.println("Waiting a client connection to notify...");
    #endif

#ifdef SCAN_BLE_DEVICES
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(200);
    pBLEScan->setWindow(199); // less or equal setInterval value
#endif

    // xTaskCreate(
    //     this->loop, /* Task function. */
    //     "BLEloop",  /* String with name of task. */
    //     10000,      /* Stack size in bytes. */
    //     NULL,       /* Parameter passed as input of the task */
    //     1,          /* Priority of the task. */
    //     NULL);      /* Task handle. */

    // xTaskCreatePinnedToCore(
    //                 this->loop,   /* Function to implement the task */
    //                 "coreTask", /* Name of the task */
    //                 10000,      /* Stack size in words */
    //                 NULL,       /* Task input parameter */
    //                 0,          /* Priority of the task */
    //                 NULL,       /* Task handle. */
    //                 1);  /* Core where the task should run */
}

void SenseBLE::setMQTTClient(PubSubClient MQTTClient)
{
    // this->m_senseMQTT = MQTTClient;
    p_senseMQTT = MQTTClient;
}

SenseBLE::~SenseBLE()
{
    BLEDevice::deinit(false);
}

void SenseBLE::deviceInformationService(BLEAdvertising *pAdvertising)
{
    // Device Information service
    BLEUUID DEVICE_INFO_SERVICE_UUID = BLEUUID::fromString("0x180A");
    BLEService *deviceInfoService = m_pServer->createService(DEVICE_INFO_SERVICE_UUID);

    BLECharacteristic *manufacturerName = deviceInfoService->createCharacteristic(
        BLEUUID::fromString("0x2A29"),
        BLECharacteristic::PROPERTY_READ);
    manufacturerName->setValue("Crudus Media");

    BLECharacteristic *modelNumber = deviceInfoService->createCharacteristic(
        BLEUUID::fromString("0x2A24"),
        BLECharacteristic::PROPERTY_READ);
    modelNumber->setValue(STR(VERSION));

    pAdvertising->addServiceUUID(DEVICE_INFO_SERVICE_UUID);

    // Start the service
    deviceInfoService->start();
}
#ifdef SCAN_BLE_DEVICES
void sendDevice(BLEAdvertisedDevice advertisedDevice)
{
    if (p_senseMQTT.connected())
    {
        // Serial.printf("Advertised Device: %s RSSI: %d \n", advertisedDevice.toString().c_str(), advertisedDevice.getRSSI());
        // String name = advertisedDevice.getName().c_str();
        // String addr = advertisedDevice.getAddress().toString().c_str();
        // String serviceUUID;
        // if (advertisedDevice.haveServiceUUID())
        // {
        //     serviceUUID = advertisedDevice.getServiceUUID().toString().c_str();
        // }

        // StaticJsonBuffer<300U> jsonBuffer;
        // JsonObject &root = jsonBuffer.createObject();

        // root["name"] = name;
        // root["address"] = addr;
        // // if (advertisedDevice.haveAppearance())
        // // {
        // //     root["appearance"] = advertisedDevice.getAppearance();
        // // }
        // // if (advertisedDevice.haveManufacturerData())
        // // {
        // //     char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t *)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
        // //     String appearance = pHex;
        // //     free(pHex);
        // //     root["appearance"] = appearance;
        // // }
        // // if (!serviceUUID.isEmpty())
        // // {
        // //     root["serviceUUID"] = serviceUUID;
        // // }

        // if (advertisedDevice.haveTXPower())
        // {
        //     root["txPower"] = advertisedDevice.getTXPower();
        // }
        // if (advertisedDevice.haveRSSI())
        // {
        //     root["RSSI"] = advertisedDevice.getRSSI();
        // }

        // String output;
        // root.printTo(output);
        // Serial.println(jsonBuffer.size());
        // jsonBuffer.clear();

        // char *payload = new char[output.length() + 1];
        // strcpy(payload, output.c_str());
        // Serial.println(payload);

        // String mqTopic = Sense::getMQTTTopic();
        // mqTopic += "ble-scan/";

        // addr.replace(":", "");
        // mqTopic += addr;
        // Serial.println(mqTopic);
        // p_senseMQTT.publish(mqTopic.c_str(), payload);
        // free(payload);
    }
}
#endif

void SenseBLE::loop()
{
#ifdef COMP_BATTERY
    if (deviceConnected)
    {
        bService->notify();
        delay(500); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
#endif

    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                    // give the bluetooth stack the chance to get things ready
        m_pServer->startAdvertising(); // restart advertising
        #if defined(DEBUG)
        Serial.println("start advertising");
        #endif
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }

#ifdef SCAN_BLE_DEVICES
    if ((millis() - lastScanTime) > scanDelay)
    {
        Serial.println("Scanning...");
        BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
        Serial.print("Devices found: ");
        Serial.println(foundDevices.getCount());
        for (size_t i = 0; i < foundDevices.getCount(); i++)
        {
            digitalWrite(GREEN_LED, LOW);
            sendDevice(foundDevices.getDevice(i));
            delay(100);
            digitalWrite(GREEN_LED, HIGH);
            delay(200);
        }
        Serial.println("Scan done!");
        pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
        lastScanTime = millis();
    }
#endif
}

#endif // CONFIG_BT_ENABLED
