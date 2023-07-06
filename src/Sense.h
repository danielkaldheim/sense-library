/*
 * Sense.h
 *
 *  Created on: Jun 28, 2020
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_H_
#define COMPONENTS_SENSE_H_

#include "SenseConfig.h"
#include <Preferences.h>
#include <WiFi.h>

class Sense
{
public:
    Sense();
    virtual ~Sense();
    bool begin(void);

    static String getChipName();
    static String getChipId();
    static String getDeviceName();
    static void setDeviceName(String value);
    static String getWiFiMac();
    static String getLocalIp();
    static String getWiFiSSID();
    static void setWiFiSSID(String value);
    static String getWiFiPwd();
    static void setWiFiPwd(String value);
    static String getDevice();
    static void setDevice(String value);
    static String getRoom();
    static void setRoom(String value);
    static String getFloor();
    static void setFloor(String value);
    static String getTenant();
    static void setTenant(String value);
    static String getMQTTTopic();
    static void setMQTTTopic(String value);
    static String getMQTTHost();
    static void setMQTTHost(String value);
    static uint16_t getMQTTPort();
    static void setMQTTPort(uint16_t value);
    static String getMQTTUser();
    static void setMQTTUser(String value);
    static String getMQTTPwd();
    static void setMQTTPwd(String value);
    static String getCrudusToken();
    static void setCrudusToken(String value);
    static float getCaliTemp();
    static void setCaliTemp(float value);
    static bool checkIAQState(size_t maxLen);
    static void getIAQState(uint8_t *state, size_t maxLen);
    static void setIAQState(const void *bsecState, size_t len);
    static void clearIAQState();

    static Preferences *getPreferences();

    static String getString(const char *key, String defaultValue = String());
    static uint32_t getUInt(const char *key, uint32_t defaultValue = 0);
    static float_t getFloat(const char *key, float_t defaultValue = NAN);

    static size_t putString(const char *key, String value);
    static size_t putUInt(const char *key, uint32_t value);
    static size_t putFloat(const char *key, float_t value);

    static bool clearPreferences();
}; // Sense

#endif // COMPONENTS_SENSE_H_
