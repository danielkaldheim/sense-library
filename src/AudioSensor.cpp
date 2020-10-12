#include "AudioSensor.h"
#ifdef COMP_AUDIO
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
const int numReadings = 50;
int readings[numReadings]; // the readings from the analog input

AudioSensor::AudioSensor(adc1_channel_t pin)
{
    _pin = pin;
}

AudioSensor::AudioSensor(adc1_channel_t pin, PAYLOAD_CALLBACK_SIGNATURE)
{
    _pin = pin;
    setCallback(callback);
}

AudioSensor::~AudioSensor()
{
}

void AudioSensor::begin(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);              // configure the analogue to digital converter
    adc1_config_channel_atten(_pin, ADC_ATTEN_DB_11); // 0 - 3.3v

    for (int thisReading = 0; thisReading < numReadings; thisReading++)
    {
        readings[thisReading] = 0;
    }
}

double AudioSensor::getPeakToPeak()
{
    unsigned long startMillis = millis(); // Start of sample window
    // unsigned int peakToPeak = 0;          // peak-to-peak level

    unsigned int signalMax = 0;
    unsigned int signalMin = 4095;
    unsigned int sample;

    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
        sample = this->readRaw();
        if (sample < 4095) // toss out spurious readings
        {
            if (sample > signalMax)
            {
                signalMax = sample; // save just the max levels
            }
            else if (sample < signalMin)
            {
                signalMin = sample; // save just the min levels
            }
        }
    }
    this->peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
    return this->peakToPeak;
}

void AudioSensor::performReading()
{
    getPeakToPeak();
}

double AudioSensor::sample()
{
    return getPeakToPeak();
}

int AudioSensor::readRaw()
{
    int raw = adc1_get_raw(_pin);
    this->raw = raw;

    // subtract the last reading:
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = raw;
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings)
    {
        // ...wrap around to the beginning:
        readIndex = 0;
    }

    this->average = total / numReadings;

    return (int)raw;
}

float AudioSensor::getdB()
{
    return (this->average + 80) / 20;
}

void AudioSensor::sendPayload()
{
    if (callback)
    {
        StaticJsonDocument<66> doc;
        doc["raw"] = this->raw;
        doc["db"] = getdB();
        doc["peakToPeak"] = (this->peakToPeak * 3.3) / 4095;

        String payload;
        serializeJson(doc, payload);
        doc.clear();
        callback("audio", payload.c_str());
    }
}

AudioSensor &AudioSensor::setCallback(PAYLOAD_CALLBACK_SIGNATURE)
{
    this->callback = callback;
    return *this;
}
#endif