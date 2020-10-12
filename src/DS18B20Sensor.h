/*
 * DS18B20Sensor.h
 *
 *  Created on: Apr 08, 2020
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_DS18B20_SENSOR_H_
#define COMPONENTS_SENSE_DS18B20_SENSOR_H_
#include "SenseConfig.h"
#ifdef COMP_DS18B20
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class DS18B20Sensor
{
public:
    DS18B20Sensor(uint8_t pin);
    DS18B20Sensor(uint8_t pin, PAYLOAD_CALLBACK_SIGNATURE);
    virtual ~DS18B20Sensor();
    void begin(void);
    void performReading();
    float getTemperature();
    void sendPayload();

    DS18B20Sensor &setCallback(PAYLOAD_CALLBACK_SIGNATURE);

    float temperature;

private:
    uint8_t _pin;
    DeviceAddress _thermometer;
    DallasTemperature _sensors;
    PAYLOAD_CALLBACK_SIGNATURE;
}; // DS18B20Sensor
#endif
#endif // COMPONENTS_SENSE_DS18B20_SENSOR_H_
