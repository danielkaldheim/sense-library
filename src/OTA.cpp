#include "OTA.h"
#if defined(ENABLE_OTA)
// OTA::OTA(WiFiClientSecure client)
OTA::OTA(WiFiClient client)
{
    m_client = client;
    _isSecure = false;
}

OTA::OTA(WiFiClientSecure client)
{
    m_secure_client = client;
    _isSecure = true;
}

OTA::~OTA()
{
}

// OTA Logic
void OTA::execOTA()
{

    Serial.print("Current version: ");
    Serial.println(STR(VERSION));

    // Reading data over SSL may be slow, use an adequate timeout
    m_client.setTimeout(12000);

    t_httpUpdate_return ret = otahttpUpdate.update(m_client, STR(OTA_UPDATE_URL), STR(VERSION));

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", otahttpUpdate.getLastError(), otahttpUpdate.getLastErrorString().c_str());
        break;

    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

    case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
}

void OTA::execSecureOTA()
{

    Serial.print("Current version: ");
    Serial.println(STR(VERSION));

    m_secure_client.setCACert(OTA_ROOT_CA);

    // Reading data over SSL may be slow, use an adequate timeout
    m_secure_client.setTimeout(12000);

    t_httpUpdate_return ret = otahttpUpdate.update(m_secure_client, "sense.crudus.no", 443, "devices/ota", STR(VERSION));

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", otahttpUpdate.getLastError(), otahttpUpdate.getLastErrorString().c_str());
        break;

    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

    case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
}

// Set time via NTP, as required for x.509 validation
void OTA::setClock()
{
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov"); // UTC

    Serial.print(F("Waiting for NTP time sync: "));
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
        yield();
        delay(500);
        Serial.print(F("."));
        now = time(nullptr);
    }

    Serial.println(F(""));
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print(F("Current time: "));
    Serial.print(asctime(&timeinfo));
}

HTTPUpdateResult OTAHTTPUpdate::update(WiFiClient &client, const String &url, const String &currentVersion)
{
    HTTPClient http;
    if (!http.begin(client, url))
    {
        return HTTP_UPDATE_FAILED;
    }

    String bearerToken = "Bearer ";
    bearerToken += Sense::getCrudusToken();

    // Add custom headers for devices.crudus.no
    http.addHeader("authorization", bearerToken);
    http.addHeader("x-esp32-framework", STR(FRAMEWORK));
    http.addHeader("x-esp32-board", STR(BOARD));
    return handleUpdate(http, currentVersion, false);
}

HTTPUpdateResult OTAHTTPUpdate::update(WiFiClient &client, const String &host, uint16_t port, const String &uri,
                                       const String &currentVersion)
{
    HTTPClient http;
    if (!http.begin(client, host, port, uri))
    {
        return HTTP_UPDATE_FAILED;
    }

    String bearerToken = "Bearer ";
    bearerToken += Sense::getCrudusToken();

    // Add custom headers for devices.crudus.no
    http.addHeader("authorization", bearerToken);
    http.addHeader("x-esp32-framework", STR(FRAMEWORK));
    http.addHeader("x-esp32-board", STR(BOARD));
    return handleUpdate(http, currentVersion, false);
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_OTAHTTPUPDATE)
OTAHTTPUpdate otahttpUpdate;
#endif
#endif
