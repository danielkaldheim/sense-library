/*
 * BatterySensor.h
 *
 *  Created on: Jan 29, 2019
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_BATTERY_SENSOR_H_
#define COMPONENTS_SENSE_BATTERY_SENSOR_H_
#include "SenseConfig.h"
#ifdef COMP_BATTERY
#include <ArduinoJson.h>
#include <driver/adc.h>
#include "esp_adc_cal.h"

class BatterySensor
{
public:
  BatterySensor(adc1_channel_t pin);
  BatterySensor(adc1_channel_t pin, PAYLOAD_CALLBACK_SIGNATURE);
  virtual ~BatterySensor();
  void begin(void);
  void performReading();
  float readVoltage();
  int readPercentage();
  bool readIsCharging();
  int readRaw();
  void sendPayload();
  String getPayload();

  BatterySensor &setCallback(PAYLOAD_CALLBACK_SIGNATURE);

  float voltage;
  int percentage;
  bool isCharging;
  int raw;

private:
  adc1_channel_t _pin;
  PAYLOAD_CALLBACK_SIGNATURE;

}; // BatterySensor

#endif
#endif // COMPONENTS_SENSE_BATTERY_SENSOR_H_
