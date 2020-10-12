#include "BatterySensor.h"
#ifdef COMP_BATTERY
#define SAMPLERATE 5

BatterySensor::BatterySensor(adc1_channel_t pin)
{
    _pin = pin;
}

BatterySensor::BatterySensor(adc1_channel_t pin, PAYLOAD_CALLBACK_SIGNATURE)
{
    _pin = pin;
    setCallback(callback);
}

BatterySensor::~BatterySensor()
{
}

void BatterySensor::begin(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12); // configure the analogue to digital converter
    adc1_config_channel_atten(_pin, ADC_ATTEN_DB_11);
}

void BatterySensor::performReading()
{
    int sample[SAMPLERATE];
    int average;

    int i;
    // Get a new sample every 10 milliseconds, get SAMPLERATE samples (default 5)
    for (i = 0; i < SAMPLERATE; i++)
    {
        sample[i] = this->readRaw();
        delay(10);
    }

    // Calculate the average
    average = 0;
    for (i = 0; i < SAMPLERATE; i++)
    {
        average += sample[i];
    }
    average /= SAMPLERATE;

    // Get percentage
    uint8_t level = map(average, 1550, 2100, 0, 100);
    this->percentage = level < 101 ? level : 100;

    // Get voltage
    this->voltage = ((float)average / 0.5f) / 1000.0f;

    // Is charging
    // TODO: Check if value is increasing over time
    this->isCharging = (average >= 2100);
}

int BatterySensor::readRaw()
{
    int raw = adc1_get_raw(_pin);
    // 12 bit @ 3.6v
    // Serial.print("Real raw: ");
    // Serial.println(raw);
    float adjusted = (3600.0f / 4096.0f) * (float)raw;
    this->raw = adjusted;
    return (int)adjusted;
}

float BatterySensor::readVoltage()
{
    // performReading();
    return voltage;
}

int BatterySensor::readPercentage()
{
    // performReading();
    return percentage;
}

bool BatterySensor::readIsCharging()
{
    // performReading();
    return isCharging;
}

void BatterySensor::sendPayload()
{
    if (callback)
    {
        callback("battery", getPayload().c_str());
    }
}

String BatterySensor::getPayload()
{
    StaticJsonDocument<92> doc;

    doc["percent"] = this->percentage;
    doc["volt"] = this->voltage;
    doc["raw"] = this->raw;
    doc["isCharging"] = this->isCharging;

    String payload;
    serializeJson(doc, payload);
    doc.clear();
    return payload;
}

BatterySensor &BatterySensor::setCallback(PAYLOAD_CALLBACK_SIGNATURE)
{
    this->callback = callback;
    return *this;
}
#endif