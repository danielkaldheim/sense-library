#include "PhotoResistorSensor.h"
#ifdef COMP_PHOTO_RES
#define SAMPLERATE 5

PhotoResistorSensor::PhotoResistorSensor(adc1_channel_t pin)
{
    _pin = pin;
}

PhotoResistorSensor::PhotoResistorSensor(adc1_channel_t pin, PAYLOAD_CALLBACK_SIGNATURE)
{
    _pin = pin;
    setCallback(callback);
}

PhotoResistorSensor::~PhotoResistorSensor()
{
}

void PhotoResistorSensor::begin(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);              // configure the analogue to digital converter
    adc1_config_channel_atten(_pin, ADC_ATTEN_DB_11); // 0 - 3.3v
}
void PhotoResistorSensor::performReading()
{
    int ldrSamples[SAMPLERATE];
    int average;

    int i;
    // Get a new sample every 10 milliseconds, get SAMPLERATE samples (default 5)
    for (i = 0; i < SAMPLERATE; i++)
    {
        ldrSamples[i] = this->readRaw();
        delay(10);
    }

    // Calculate the average
    average = 0;
    for (i = 0; i < SAMPLERATE; i++)
    {
        average += ldrSamples[i];
    }
    average /= SAMPLERATE;

    // Map light to a value between 0-100
    this->percentage = map(average, 0, 4095, 0, 100);
}
int PhotoResistorSensor::getLight()
{
    return this->percentage;
}

int PhotoResistorSensor::readRaw()
{
    int raw = adc1_get_raw(_pin);
    // 12 bit @ 3.6v
    // Serial.print("Real raw: ");
    // Serial.println(raw);

    // Serial.print("Adjusted: ");
    // Serial.println((raw * 3.3) / 4095);
    // float adjusted = (3600.0f / 4096.0f) * (float)raw;
    // this->raw = (raw * 3.3) / 4095;
    this->raw = raw;

    return (int)raw;
}

String PhotoResistorSensor::getStringMeasurement()
{
    if (percentage < 1)
    {
        return "Dark";
    }
    else if (percentage < 20)
    {
        return "Dim";
    }
    else if (percentage < 50)
    {
        return "Light";
    }
    else if (percentage < 80)
    {
        return "Bright";
    }
    return "Very bright";
}

void PhotoResistorSensor::sendPayload()
{
    if (callback)
    {
        StaticJsonDocument<42> doc;

        doc["raw"] = this->raw;
        doc["light"] = this->percentage;

        String payload;
        serializeJson(doc, payload);
        doc.clear();
        callback("photoresistor", payload.c_str());
    }
}

PhotoResistorSensor &PhotoResistorSensor::setCallback(PAYLOAD_CALLBACK_SIGNATURE)
{
    this->callback = callback;
    return *this;
}
#endif