/*
 * PhotoResistorSensor.h
 *
 *  Created on: Jan 29, 2019
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_PHOTO_RESISTOR_SENSOR_H_
#define COMPONENTS_SENSE_PHOTO_RESISTOR_SENSOR_H_
#include "SenseConfig.h"
#ifdef COMP_PHOTO_RES
#include <ArduinoJson.h>
#include <driver/adc.h>

class PhotoResistorSensor
{
public:
  PhotoResistorSensor(adc1_channel_t pin);
  PhotoResistorSensor(adc1_channel_t pin, PAYLOAD_CALLBACK_SIGNATURE);
  virtual ~PhotoResistorSensor();
  void begin(void);
  void performReading();
  int getLight();
  int readRaw();
  void sendPayload();
  String getStringMeasurement();

  PhotoResistorSensor &setCallback(PAYLOAD_CALLBACK_SIGNATURE);

  int percentage;
  int raw;

private:
  adc1_channel_t _pin;
  PAYLOAD_CALLBACK_SIGNATURE;

}; // PhotoResistorSensor

#endif
#endif // COMPONENTS_SENSE_PHOTO_RESISTOR_SENSOR_H_
