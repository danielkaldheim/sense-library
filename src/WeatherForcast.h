/*
 * WeatherForcast.h
 *
 *  Created on: Oct 13, 2020
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_WEATHER_FORCAST_H_
#define COMPONENTS_SENSE_WEATHER_FORCAST_H_
#include "SenseConfig.h"
#ifdef COMP_WEATHER_FORCAST
#include <HTTPClient.h>
#include <ArduinoJson.h>

class WeatherForcast
{
public:
    WeatherForcast();
    virtual ~WeatherForcast();
    void begin(void);
    void loop();
    void getForcast();
    void getNext24();

private:
    HTTPClient http;
    String httpGETRequest(const char *serverName);

    unsigned long previousMillis = 0;
    const unsigned long interval = 60*15*1000; // 15 minutes

    double pressureAtSeaLevel;
    double airTemperature;
    double relativeHumidity;
    double cloudFraction;
    double windSpeed;
    double windDirection;

    double tempLow;
    double tempHigh;
    double tempMean;

    double windLow;
    double windHigh;
    double windMean;

    double cloudLow;
    double cloudHigh;
    double cloudMean;
}; // WeatherForcast

#endif
#endif // COMPONENTS_SENSE_WEATHER_FORCAST_H_
