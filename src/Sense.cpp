#include "Sense.h"

Preferences preferences;

Sense::~Sense()
{
    // preferences.end();
}

Sense::Sense()
{
    preferences.begin(PREFERENCES_KEY, false);
}

bool Sense::begin(void)
{
    preferences.begin(PREFERENCES_KEY, false);
    return true;
}

String Sense::getChipName()
{
    uint64_t chipid = ESP.getEfuseMac();

    char chipName[32];
    sprintf(chipName, "-%04X",
            (uint16_t)(chipid >> 32));
    return String(STR(PROJECT)) + chipName;
}

String Sense::getDeviceName()
{
    return getString("deviceName", getChipName());
}

void Sense::setDeviceName(String value)
{
    putString("deviceName", value);
}

String Sense::getWiFiMac()
{
    return WiFi.macAddress();
}

String Sense::getLocalIp()
{
    return WiFi.localIP().toString();
}

String Sense::getWiFiSSID()
{
    return getString("wifi-ssid", STR(WIFI_SSID));
}

void Sense::setWiFiSSID(String value)
{
    putString("wifi-ssid", value);
}

String Sense::getWiFiPwd()
{
    return getString("wifi-pwd", STR(WIFI_SSID_PWD));
}

void Sense::setWiFiPwd(String value)
{
    putString("wifi-pwd", value);
}

String Sense::getDevice()
{
    return getString("deviceId");
}

void Sense::setDevice(String value)
{
    putString("deviceId", value);
}

String Sense::getRoom()
{
    return getString("roomId");
}

void Sense::setRoom(String value)
{
    putString("roomId", value);
}

String Sense::getFloor()
{
    return getString("floor");
}

void Sense::setFloor(String value)
{
    putString("floor", value);
}

String Sense::getTenant()
{
    return getString("tenantId");
}

void Sense::setTenant(String value)
{
    putString("tenantId", value);
}

String Sense::getMQTTTopic()
{
    String tenantID = getTenant();
    String deviceID = getDevice();
    if (!tenantID.isEmpty() && !deviceID.isEmpty())
    {
        return String(STR(PROJECT)) + "/" + deviceID + "/";
    }

    uint64_t chipid = ESP.getEfuseMac();
    char chipName[32];
    sprintf(chipName, "/%04X/",
            (uint16_t)(chipid >> 32));

    return getString("mqtt-topic", String(STR(PROJECT)) + chipName);
}

void Sense::setMQTTTopic(String value)
{
    putString("mqtt-topic", value);
}

String Sense::getMQTTHost()
{
    return getString("mqtt-host", STR(MQTT_HOST));
}

void Sense::setMQTTHost(String value)
{
    putString("mqtt-host", value);
}

uint16_t Sense::getMQTTPort()
{
    preferences.begin(PREFERENCES_KEY, false);
    return preferences.getUShort("mqtt-port", 1883);
}

void Sense::setMQTTPort(uint16_t value)
{
    preferences.begin(PREFERENCES_KEY, false);
    preferences.putUShort("mqtt-port", value);
}

String Sense::getMQTTUser()
{
    return getString("mqtt-user", STR(MQTT_USER));
}

void Sense::setMQTTUser(String value)
{
    putString("mqtt-user", value);
}

String Sense::getMQTTPwd()
{
    return getString("mqtt-pwd", STR(MQTT_PWD));
}

void Sense::setMQTTPwd(String value)
{
    putString("mqtt-pwd", value);
}

String Sense::getCrudusToken()
{
    return getString("token");
}

void Sense::setCrudusToken(String value)
{
    putString("token", value);
}

float Sense::getCaliTemp()
{
    return getFloat("cali-temp", 3.5f);
}

void Sense::setCaliTemp(float value)
{
    putFloat("cali-temp", value);
}

bool Sense::checkIAQState(size_t maxLen)
{
    preferences.begin(PREFERENCES_KEY, false);
    return preferences.getBytesLength("bsecState") == maxLen;
}

void Sense::getIAQState(uint8_t *state, size_t maxLen)
{
    preferences.begin(PREFERENCES_KEY, false);
    preferences.getBytes("bsecState", state, maxLen);
}

void Sense::setIAQState(const void *bsecState, size_t maxLen)
{
    preferences.begin(PREFERENCES_KEY, false);
    preferences.putBytes("bsecState", bsecState, maxLen);
}

void Sense::clearIAQState()
{
    preferences.begin(PREFERENCES_KEY, false);
    preferences.remove("bsecState");
}

Preferences *Sense::getPreferences()
{
    preferences.begin(PREFERENCES_KEY, false);
    return &preferences;
}

String Sense::getString(const char *key, const String defaultValue)
{
    preferences.begin(PREFERENCES_KEY, false);
    return preferences.getString(key, defaultValue);
}

uint32_t Sense::getUInt(const char *key, const uint32_t defaultValue)
{
    preferences.begin(PREFERENCES_KEY, false);
    return preferences.getUInt(key, defaultValue);
}

float_t Sense::getFloat(const char *key, const float_t defaultValue)
{
    preferences.begin(PREFERENCES_KEY, false);
    return preferences.getFloat(key, defaultValue);
}

size_t Sense::putString(const char *key, String value)
{
    preferences.begin(PREFERENCES_KEY, false);
    return preferences.putString(key, value);
}

size_t Sense::putUInt(const char *key, uint32_t value)
{
    preferences.begin(PREFERENCES_KEY, false);
    return preferences.putUInt(key, value);
}

size_t Sense::putFloat(const char *key, float_t value)
{
    preferences.begin(PREFERENCES_KEY, false);
    return preferences.putFloat(key, value);
}

bool Sense::clearPreferences()
{
    return preferences.clear();
}
