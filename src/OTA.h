/*
 * OTA.h
 *
 *  Created on: Jan 18, 2019
 *      Author: danielkaldheim
 *  https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPUpdate/examples/httpUpdateSecure/httpUpdateSecure.ino
 */

#ifndef COMPONENTS_SENSE_OTA_H_
#define COMPONENTS_SENSE_OTA_H_
#if defined(ENABLE_OTA)

#include <Arduino.h>
#include <SenseConfig.h>
#include <Sense.h>
#include <WiFi.h>

#include <HTTPClient.h>
#include <HTTPUpdate.h>

#include <time.h>

class OTA
{
public:
  OTA(WiFiClient client);
  OTA(WiFiClientSecure client);
  virtual ~OTA();
  void execOTA();
  void execSecureOTA();
  void setClock();

private:
  WiFiClientSecure m_secure_client;
  WiFiClient m_client;
  bool _isSecure;
}; // OTA

class OTAHTTPUpdate : public HTTPUpdate
{
public:
  t_httpUpdate_return update(WiFiClient &client, const String &url, const String &currentVersion = "");
  t_httpUpdate_return update(WiFiClient &client, const String &host, uint16_t port, const String &uri = "/",
                             const String &currentVersion = "");
}; // OTAHTTPUpdate

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_OTAHTTPUPDATE)
extern OTAHTTPUpdate otahttpUpdate;
#endif

#endif
#endif // COMPONENTS_SENSE_OTA_H_
