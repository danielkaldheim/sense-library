#include "BME680Sensor.h"
#ifdef COMP_BME680
const uint8_t bsec_config_iaq[] = {4, 7, 4, 1, 61, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 0, 48, 0, 1, 0, 0, 192, 168, 71, 64, 49, 119, 76, 0, 0, 225, 68, 137, 65, 0, 63, 205, 204, 204, 62, 0, 0, 64, 63, 205, 204, 204, 62, 0, 0, 0, 0, 216, 85, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 28, 0, 2, 0, 0, 244, 1, 225, 0, 25, 0, 0, 128, 64, 0, 0, 32, 65, 144, 1, 0, 0, 112, 65, 0, 0, 0, 63, 16, 0, 3, 0, 10, 215, 163, 60, 10, 215, 35, 59, 10, 215, 35, 59, 9, 0, 5, 0, 0, 0, 0, 0, 1, 88, 0, 9, 0, 229, 208, 34, 62, 0, 0, 0, 0, 0, 0, 0, 0, 218, 27, 156, 62, 225, 11, 67, 64, 0, 0, 160, 64, 0, 0, 0, 0, 0, 0, 0, 0, 94, 75, 72, 189, 93, 254, 159, 64, 66, 62, 160, 191, 0, 0, 0, 0, 0, 0, 0, 0, 33, 31, 180, 190, 138, 176, 97, 64, 65, 241, 99, 190, 0, 0, 0, 0, 0, 0, 0, 0, 167, 121, 71, 61, 165, 189, 41, 192, 184, 30, 189, 64, 12, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 229, 0, 254, 0, 2, 1, 5, 48, 117, 100, 0, 44, 1, 112, 23, 151, 7, 132, 3, 197, 0, 92, 4, 144, 1, 64, 1, 64, 1, 144, 1, 48, 117, 48, 117, 48, 117, 48, 117, 100, 0, 100, 0, 100, 0, 48, 117, 48, 117, 48, 117, 100, 0, 100, 0, 48, 117, 48, 117, 100, 0, 100, 0, 100, 0, 100, 0, 48, 117, 48, 117, 48, 117, 100, 0, 100, 0, 100, 0, 48, 117, 48, 117, 100, 0, 100, 0, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 44, 1, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 8, 7, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 112, 23, 255, 255, 255, 255, 255, 255, 255, 255, 220, 5, 220, 5, 220, 5, 255, 255, 255, 255, 255, 255, 220, 5, 220, 5, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 44, 1, 0, 0, 0, 0, 138, 80, 0, 0};

Bsec iaqSensor;
String output;

#define STATE_SAVE_PERIOD UINT32_C(360 * 60 * 1000) // 360 minutes - 4 times a day
// #define STATE_SAVE_PERIOD UINT32_C(10 * 60 * 1000) // 5 minutes

// Adafruit_BME680 bme;
// Fetch this from yr.no
#define SEALEVELPRESSURE_HPA (1010.6)

// float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
// float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score

// float hum_score, gas_score;
// float gas_reference = 250000;
// float hum_reference = 40;
// int getgasreference_count = 0;

uint8_t bsecState[BSEC_MAX_STATE_BLOB_SIZE] = {0};
uint16_t stateUpdateCounter = 0;
int latest_accuracy = 1;

void errLeds(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    ESP.restart();
}

BME680Sensor::BME680Sensor(void)
{
}

BME680Sensor::BME680Sensor(PAYLOAD_CALLBACK_SIGNATURE)
{
    setCallback(callback);
}

BME680Sensor::~BME680Sensor()
{
}

void BME680Sensor::begin(void)
{
    Wire.begin();
    iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
    output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
    Serial.println(output);
    checkIaqSensorStatus();

    iaqSensor.setConfig(bsec_config_iaq);
    checkIaqSensorStatus();
    iaqSensor.setTemperatureOffset(Sense::getCaliTemp());

    loadState();

    bsec_virtual_sensor_t sensorList[10] = {
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
    };

    iaqSensor.updateSubscription(sensorList, sizeof(sensorList) / sizeof(sensorList[0]), BSEC_SAMPLE_RATE_LP);
    Serial.println("After updateSubscription");
    checkIaqSensorStatus();
}

void BME680Sensor::checkIaqSensorStatus(void)
{
    if (iaqSensor.status != BSEC_OK)
    {
        if (iaqSensor.status < BSEC_OK)
        {
            output = "BSEC error code : " + String(iaqSensor.status);
            Serial.println(output);
            for (;;)
                errLeds(); /* Halt in case of failure */
        }
        else
        {
            output = "BSEC warning code : " + String(iaqSensor.status);
            Serial.println(output);
        }
    }

    if (iaqSensor.bme680Status != BME680_OK)
    {
        if (iaqSensor.bme680Status < BME680_OK)
        {
            output = "BME680 error code : " + String(iaqSensor.bme680Status);
            Serial.println(output);
            for (;;)
                errLeds(); /* Halt in case of failure */
        }
        else
        {
            output = "BME680 warning code : " + String(iaqSensor.bme680Status);
            Serial.println(output);
        }
    }
    iaqSensor.status = BSEC_OK;
}

bool BME680Sensor::performReading()
{
    if (iaqSensor.run())
    {
        updateState();
        return true;
    }
    else
    {
        checkIaqSensorStatus();
    }
    return false;
}

float BME680Sensor::getTemperature()
{
    return iaqSensor.temperature;
}

float BME680Sensor::getHumidity()
{
    return iaqSensor.humidity;
}

float BME680Sensor::getIaq()
{
    return iaqSensor.iaq;
}

float BME680Sensor::getIaqAccuracy()
{
    return iaqSensor.iaqAccuracy;
}

void BME680Sensor::sendTemperaturePayload()
{
    if (callback)
    {
        StaticJsonDocument<112> doc;
        doc["rawTemperature"] = iaqSensor.rawTemperature;
        doc["rawHumidity"] = iaqSensor.rawHumidity;
        doc["temperature"] = iaqSensor.temperature;
        doc["humidity"] = iaqSensor.humidity;

        String payload;
        serializeJson(doc, payload);
        doc.clear();
        callback("bme/temperature", payload.c_str());
    }
}

void BME680Sensor::sendIAQPayload()
{
    if (callback)
    {
#ifndef DEBUG
        if (iaqSensor.iaqAccuracy < 3)
        {
            return;
        }
#endif
        StaticJsonDocument<200> doc;
        doc["iaq"] = iaqSensor.iaq;
        doc["score"] = CalculateIAQ(iaqSensor.iaq);
        doc["iaqAccuracy"] = iaqSensor.iaqAccuracy;
        doc["staticIaq"] = iaqSensor.staticIaq;
        String payload;
        serializeJson(doc, payload);
        doc.clear();
        callback("bme/iaq", payload.c_str());
    }
}

void BME680Sensor::sendPressurePayload()
{
    if (callback)
    {
        StaticJsonDocument<50> doc;
        doc["pressure"] = iaqSensor.pressure / 100.0f;
        doc["altitude"] = readAltitude(SEALEVELPRESSURE_HPA);
        String payload;
        serializeJson(doc, payload);
        doc.clear();
        callback("bme/pressure", payload.c_str());
    }
}
void BME680Sensor::sendGasPayload()
{
    if (callback)
    {
        if (iaqSensor.breathVocEquivalent >= 1000.f)
        {
            return;
        }
        StaticJsonDocument<96> doc;
        doc["gasResistance"] = iaqSensor.gasResistance / 1000.0f;
        doc["co2Equivalent"] = iaqSensor.co2Equivalent;
        doc["breathVocEquivalent"] = iaqSensor.breathVocEquivalent;
        String payload;
        serializeJson(doc, payload);
        doc.clear();
        callback("bme/gas", payload.c_str());
    }
}

float BME680Sensor::readAltitude(float seaLevel)
{
    float atmospheric = iaqSensor.pressure / 100.0F;
    return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}

void BME680Sensor::loadState(void)
{
    uint8_t readState[BSEC_MAX_STATE_BLOB_SIZE] = {0};
    if (Sense::checkIAQState(BSEC_MAX_STATE_BLOB_SIZE))
    {
        Serial.println("Reading state from preferences");
        Sense::getIAQState(readState, BSEC_MAX_STATE_BLOB_SIZE);
        iaqSensor.setState(readState);
        checkIaqSensorStatus();
    }
    else
    {
        Serial.println("Erasing preferences");
        Sense::clearIAQState();
    }
}

void BME680Sensor::updateState(void)
{
    bool update = false;
    /* Set a trigger to save the state. Here, the state is saved every STATE_SAVE_PERIOD with the first state being saved once the algorithm achieves full calibration, i.e. iaqAccuracy = 3 */
    if (stateUpdateCounter == 0)
    {
        if (iaqSensor.iaqAccuracy >= 3)
        {
            update = true;
            stateUpdateCounter++;
        }
    }
    else
    {
        /* Update every STATE_SAVE_PERIOD milliseconds */
        if ((stateUpdateCounter * STATE_SAVE_PERIOD) < millis())
        {
            update = true;
            stateUpdateCounter++;
        }
    }

    if (update)
    {
        iaqSensor.getState(bsecState);
        checkIaqSensorStatus();

        Serial.println("Writing state to preferences");
        Sense::setIAQState(bsecState, BSEC_MAX_STATE_BLOB_SIZE);
    }
}

void BME680Sensor::CalculateAirQualityScore()
{
    // Calculate humidity contribution to IAQ index
    /*float current_humidity = bme.readHumidity();
    if (current_humidity >= 38 && current_humidity <= 42)
    {
        hum_score = 0.25 * 100; // Humidity +/-5% around optimum
    }
    else
    { // sub-optimal
        if (current_humidity < 38)
            hum_score = 0.25 / hum_reference * current_humidity * 100;
        else
        {
            hum_score = ((-0.25 / (100 - hum_reference) * current_humidity) + 0.416666) * 100;
        }
    }

    // Calculate gas contribution to IAQ index
    float gas_lower_limit = 5000;  // Bad air quality limit
    float gas_upper_limit = 50000; // Good air quality limit
    if (gas_reference > gas_upper_limit)
    {
        gas_reference = gas_upper_limit;
    }
    if (gas_reference < gas_lower_limit)
    {
        gas_reference = gas_lower_limit;
    }
    gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100;

    // Combine results for the final IAQ index value (0-100% where 100% is good quality air)
    air_quality_score = hum_score + gas_score;

    // Serial.println("Air Quality = " + String(air_quality_score, 1) + "% derived from 25% of Humidity reading and 75% of Gas reading - 100% is good quality air");
    // Serial.println("Humidity element was : " + String(hum_score / 100) + " of 0.25");
    // Serial.println("     Gas element was : " + String(gas_score / 100) + " of 0.75");
    // if (bme.readGas() < 120000)
    // {
    //     Serial.println("***** Poor air quality *****");
    // }
    // Serial.println();
    if ((getgasreference_count++) % 10 == 0)
    {
        GetGasReference();
    }

    // Serial.println(CalculateIAQ(air_quality_score));
    // Serial.println("------------------------------------------------");

    // Serial.println();*/
}

// void BME680Sensor::GetGasReference()
// {
// Now run the sensor for a burn-in period, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
// Serial.println("Getting a new gas reference value");
/* int readings = 10;
    for (int i = 1; i <= readings; i++)
    { // read gas for 10 x 0.150mS = 1.5secs
        gas_reference += bme.readGas();
    }
    gas_reference = gas_reference / readings; */
// }

String BME680Sensor::CalculateIAQ(float score)
{
    String IAQ_text = "";
    // score = (100 - score) * 5;
    if (score >= 301)
        IAQ_text += "Hazardous";
    else if (score >= 201 && score <= 300)
        IAQ_text += "Very Unhealthy";
    else if (score >= 176 && score <= 200)
        IAQ_text += "Unhealthy";
    else if (score >= 151 && score <= 175)
        IAQ_text += "Unhealthy for Sensitive Groups";
    else if (score >= 51 && score <= 150)
        IAQ_text += "Moderate";
    else if (score >= 00 && score <= 50)
        IAQ_text += "Good";
    return IAQ_text;
}

BME680Sensor &BME680Sensor::setCallback(PAYLOAD_CALLBACK_SIGNATURE)
{
    this->callback = callback;
    return *this;
}
#endif