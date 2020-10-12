/*
 * BME680Sensor.h
 *
 *  Created on: Jan 29, 2019
 *      Author: danielkaldheim
 *
 * - https://learn.adafruit.com/calibrating-sensors/why-calibrate
 * - https://wiki.dfrobot.com/Gravity__I2C_BME680_Environmental_Sensor__VOC,_Temperature,_Humidity,_Barometer__SKU__SEN0248
 * - https://community.bosch-sensortec.com/t5/MEMS-sensors-forum/BME680-BSEC-eCO2/m-p/5919?advanced=false&collapse_discussion=true&filter=location&location=forum-board:bst_community-mems-forum&q=BME680&search_type=thread
 * Todo:
 * Drop adafruit library and use BoschSensortec Arduino library
 * - https://github.com/BoschSensortec/BSEC-Arduino-library/blob/master/examples/basic/basic.ino
 */

#ifndef COMPONENTS_SENSE_BME680_SENSOR_H_
#define COMPONENTS_SENSE_BME680_SENSOR_H_
#include "SenseConfig.h"
#ifdef COMP_BME680
#include <Wire.h>
#include <Sense.h>
#include "bsec.h"

#include <ArduinoJson.h>

class BME680Sensor
{
public:
    BME680Sensor(void);
    BME680Sensor(PAYLOAD_CALLBACK_SIGNATURE);
    virtual ~BME680Sensor();
    void begin(void);
    bool performReading();

    void sendTemperaturePayload();
    void sendGasPayload();
    void sendPressurePayload();
    void sendIAQPayload();

    float readAltitude(float seaLevel);

    float getTemperature();
    float getHumidity();
    float getIaq();
    float getIaqAccuracy();

    BME680Sensor &setCallback(PAYLOAD_CALLBACK_SIGNATURE);
    // float air_quality_score;

private:
    void CalculateAirQualityScore();
    // void GetGasReference();
    String CalculateIAQ(float score);
    void checkIaqSensorStatus(void);
    void loadState(void);
    void updateState(void);
    PAYLOAD_CALLBACK_SIGNATURE;

}; // BME680Sensor
#endif
#endif // COMPONENTS_SENSE_BME680_SENSOR_H_
