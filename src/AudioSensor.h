/*
 * AudioSensor.h
 *
 *  Created on: Jan 29, 2019
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_AUDIO_SENSOR_H_
#define COMPONENTS_SENSE_AUDIO_SENSOR_H_
#include "SenseConfig.h"
#ifdef COMP_AUDIO
#include <ArduinoJson.h>
#include <driver/adc.h>

class AudioSensor
{
public:
  AudioSensor(adc1_channel_t pin);
  AudioSensor(adc1_channel_t pin, PAYLOAD_CALLBACK_SIGNATURE);
  virtual ~AudioSensor();
  void begin(void);
  void performReading();
  double getPeakToPeak();
  double sample();
  int readRaw();
  // Get current decibel value
  float getdB();
  void sendPayload();
  AudioSensor &setCallback(PAYLOAD_CALLBACK_SIGNATURE);

  // Peak to peak value
  double peakToPeak;
  // Raw value
  int raw;
  // the average
  int average = 0;

private:
  adc1_channel_t _pin;
  PAYLOAD_CALLBACK_SIGNATURE;
  // the index of the current reading
  int readIndex = 0;
  // the running total
  int total = 0;

}; // AudioSensor

#endif // COMPONENTS_SENSE_AUDIO_SENSOR_H_
#endif