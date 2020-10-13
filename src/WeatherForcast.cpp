#include "WeatherForcast.h"
#ifdef COMP_WEATHER_FORCAST

WeatherForcast::WeatherForcast()
{
}

WeatherForcast::~WeatherForcast()
{
}

void WeatherForcast::begin(void)
{
}

void WeatherForcast::loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis += interval;
#if defined(DEBUG)
        Serial.println("Fetch weather forcast...");
#endif

        getForcast();
        delay(100);
        getNext24();

#if defined(DEBUG)
        Serial.print("Air temperature: ");
        Serial.println(airTemperature);
#endif
    }
}

void WeatherForcast::getForcast()
{
    String lat = STR(LATITUDE);
    String lon = STR(LONGITUDE);
    String serverPath = "https://met.crudus.no/locationforecast?lat=" + lat + "&lon=" + lon;
    String result = httpGETRequest(serverPath.c_str());

    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, result);

    // Test if parsing succeeds.
    if (error)
    {
#if defined(DEBUG)
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
#endif
        return;
    }
    airTemperature = doc["air_temperature"];
    cloudFraction = doc["cloud_area_fraction"];
    pressureAtSeaLevel = doc["air_pressure_at_sea_level"];
    relativeHumidity = doc["relative_humidity"];
    windDirection = doc["wind_from_direction"];
    windSpeed = doc["wind_speed"];
}

void WeatherForcast::getNext24()
{
    String lat = STR(LATITUDE);
    String lon = STR(LONGITUDE);
    String serverPath = "https://met.crudus.no/next24?lat=" + lat + "&lon=" + lon;
    String result = httpGETRequest(serverPath.c_str());

    StaticJsonDocument<350> doc;
    DeserializationError error = deserializeJson(doc, result);

    // Test if parsing succeeds.
    if (error)
    {
#if defined(DEBUG)
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
#endif
        return;
    }
    tempLow = doc["temp"]["low"];
    tempHigh = doc["temp"]["high"];
    tempMean = doc["temp"]["mean"];

    windLow = doc["wind"]["low"];
    windHigh = doc["wind"]["high"];
    windMean = doc["wind"]["mean"];

    cloudLow = doc["cloud"]["low"];
    cloudHigh = doc["cloud"]["high"];
    cloudMean = doc["cloud"]["mean"];
}

String WeatherForcast::httpGETRequest(const char *serverPath)
{
    HTTPClient http;

    // Your IP address with path or Domain name with URL path
    http.begin(serverPath, STR(OTA_ROOT_CA));

    // Send HTTP POST request
    int httpResponseCode = http.GET();

    String payload = "{}";

    if (httpResponseCode == 200)
    {
        payload = http.getString();
    }
    else
    {
#if defined(DEBUG)
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
#endif
    }
    // Free resources
    http.end();

    return payload;
}

#endif
