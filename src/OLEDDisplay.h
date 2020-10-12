/*
 * OLEDDisplay.h
 *
 *  Created on: Apr 08, 2020
 *      Author: danielkaldheim
 */

#ifndef COMPONENTS_SENSE_OLED_DISPLAY_H_
#define COMPONENTS_SENSE_OLED_DISPLAY_H_
#include "SenseConfig.h"
#ifdef COMP_OLED
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

class OLEDDisplay
{
public:
    OLEDDisplay();
    virtual ~OLEDDisplay();
    void begin(void);

private:
}; // OLEDDisplay
#endif
#endif // COMPONENTS_SENSE_OLED_DISPLAY_H_
