/*
 * SenseWifi.h
 *
 *  Created on: Jan 30, 2019
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_WIFI_H_
#define COMPONENTS_SENSE_WIFI_H_

#include <Arduino.h>
#include <SenseConfig.h>
#include <Sense.h>
#include <WiFi.h>
#if defined(ENABLE_OTA)
#include <OTA.h>
#endif
extern "C"
{
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "esp_system.h"
}

class SenseWifi
{
public:
  SenseWifi(WiFiEventCb wifiEventCallback);
  virtual ~SenseWifi();
  void begin(void);
  bool connected();

private:
  WiFiEventCb _wifiEventCallback;
}; // SenseWifi

#endif // COMPONENTS_SENSE_WIFI_H_
