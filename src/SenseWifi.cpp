#include "SenseWifi.h"
#include <functional>
TimerHandle_t wifiReconnectTimer;
int WiFiReconnectAttempts = 0;
String hostname;

#if defined(ENABLE_OTA)
void checkOTA()
{
    String token = Sense::getCrudusToken();

    if (token.length() > 0 && WiFi.isConnected())
    {
        WiFiClient client;
        OTA *ota = new OTA(client);
        ota->setClock();
        ota->execOTA();
        // ota->execSecureOTA();
        free(ota);
    }
    else
    {
        Serial.println("No token or No WiFi, don't check OTA");
    }
}
#endif

void setClock()
{
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov"); // UTC
#if defined(DEBUG)
    Serial.print(F("Waiting for NTP time sync: "));
#endif
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
        yield();
        delay(500);
#if defined(DEBUG)
        Serial.print(F("."));
#endif
        now = time(nullptr);
    }

    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
#if defined(DEBUG)
    Serial.println(F(""));
    Serial.print(F("Current time: "));
    Serial.print(asctime(&timeinfo));
#endif
}

void connectToWifi()
{
    String wiFiSSID = Sense::getWiFiSSID();
    String wiFiSSIDPwd = Sense::getWiFiPwd();

    Serial.println(wiFiSSID);
    Serial.println(wiFiSSIDPwd);

    if (wiFiSSID.length() > 0 && wiFiSSIDPwd.length() > 0)
    {
        if (WiFiReconnectAttempts >= 5)
        {
#if defined(DEBUG)
            Serial.println("WiFi reconnect limit reached, device restart");
#endif
            ESP.restart();
        }
        else
        {
            WiFiReconnectAttempts++;
#if defined(DEBUG)
            Serial.println("Connecting to Wi-Fi...");
            Serial.print("Attemt: ");
#endif
            WiFi.begin(wiFiSSID.c_str(), wiFiSSIDPwd.c_str());
        }
    }
    else
    {
#if defined(DEBUG)
        Serial.print("No credentials, don't connect to WiFi");
#endif
    }
}

void WiFiEvent(WiFiEvent_t event)
{
#if defined(DEBUG)
    Serial.printf("[WiFi-event] event: %d\n", event);
#endif
    switch (event)
    {
    case SYSTEM_EVENT_STA_GOT_IP:
#if defined(DEBUG)
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
#endif
        WiFiReconnectAttempts = 0;
        setClock();
#if defined(ENABLE_OTA)
        checkOTA();
#endif
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
#if defined(DEBUG)
        Serial.println("WiFi lost connection");
#endif
        xTimerStart(wifiReconnectTimer, 0);
        break;
    case SYSTEM_EVENT_WIFI_READY:
#if defined(DEBUG)
        Serial.println("Wifi ready");
#endif
        break;
    // case SYSTEM_EVENT_SCAN_DONE:
    //     break;
    case SYSTEM_EVENT_STA_START:
#if defined(DEBUG)
        Serial.print("Set hostname: ");
        Serial.println(hostname);
#endif
        WiFi.setHostname(hostname.c_str());
        break;
    // case SYSTEM_EVENT_STA_STOP:
    //     break;
    // case SYSTEM_EVENT_STA_CONNECTED:
    //     break;
    // case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
    //     break;
    // case SYSTEM_EVENT_STA_LOST_IP:
    //     break;
    // case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
    //     break;
    // case SYSTEM_EVENT_STA_WPS_ER_FAILED:
    //     break;
    // case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
    //     break;
    // case SYSTEM_EVENT_STA_WPS_ER_PIN:
    //     break;
    // case SYSTEM_EVENT_AP_START:
    //     break;
    // case SYSTEM_EVENT_AP_STOP:
    //     break;
    // case SYSTEM_EVENT_AP_STACONNECTED:
    //     break;
    // case SYSTEM_EVENT_AP_STADISCONNECTED:
    //     break;
    // case SYSTEM_EVENT_AP_STAIPASSIGNED:
    //     break;
    // case SYSTEM_EVENT_AP_PROBEREQRECVED:
    //     break;
    // case SYSTEM_EVENT_GOT_IP6:
    //     break;
    // case SYSTEM_EVENT_ETH_START:
    //     break;
    // case SYSTEM_EVENT_ETH_STOP:
    //     break;
    // case SYSTEM_EVENT_ETH_CONNECTED:
    //     break;
    // case SYSTEM_EVENT_ETH_DISCONNECTED:
    //     break;
    // case SYSTEM_EVENT_ETH_GOT_IP:
    //     break;
    // case SYSTEM_EVENT_MAX:
    //     break;
    default:
        break;
    }
}

// SenseWifi::SenseWifi(std::function<void(WiFiEvent_t event)> wifiEventCallback)
// {
//     _wifiEventCallback = wifiEventCallback;
// }
SenseWifi::SenseWifi(WiFiEventCb wifiEventCallback)
{
    _wifiEventCallback = wifiEventCallback;
}

SenseWifi::~SenseWifi()
{
}

void SenseWifi::begin()
{
    hostname = Sense::getChipName();

    wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
    WiFi.onEvent(WiFiEvent);
    if (_wifiEventCallback)
    {
        WiFi.onEvent(_wifiEventCallback);
    }
    connectToWifi();
}

bool SenseWifi::connected()
{
    return WiFi.isConnected();
}
