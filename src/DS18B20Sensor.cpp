#include "DS18B20Sensor.h"
#ifdef COMP_DS18B20
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(32);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

DS18B20Sensor::DS18B20Sensor(uint8_t pin)
{
    _pin = pin;
}

DS18B20Sensor::DS18B20Sensor(uint8_t pin, PAYLOAD_CALLBACK_SIGNATURE)
{
    _pin = pin;
    setCallback(callback);
}

DS18B20Sensor::~DS18B20Sensor()
{
}

void DS18B20Sensor::begin(void)
{
    sensors.begin();

    if (!sensors.getAddress(_thermometer, 0))
    {
        Serial.println("Unable to find address for Device 0");
    }
}

void DS18B20Sensor::performReading()
{
    sensors.requestTemperatures();
    temperature = sensors.getTempC(_thermometer);
}

float DS18B20Sensor::getTemperature()
{
    return temperature;
}

void DS18B20Sensor::sendPayload()
{
    if (callback)
    {
        if (this->temperature < 85)
        {
            StaticJsonDocument<28> doc;
            doc["temperature"] = this->temperature;

            String payload;
            serializeJson(doc, payload);
            doc.clear();
            callback("ds18b20/temperature", payload.c_str());
        }
    }
}

DS18B20Sensor &DS18B20Sensor::setCallback(PAYLOAD_CALLBACK_SIGNATURE)
{
    this->callback = callback;
    return *this;
}
#endif