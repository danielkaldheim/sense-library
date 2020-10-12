#include "OLEDDisplay.h"
#ifdef COMP_OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

OLEDDisplay::OLEDDisplay()
{
}

OLEDDisplay::~OLEDDisplay()
{
}

void OLEDDisplay::begin(void)
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    display.setRotation(2);
    display.display();
    delay(100); // Pause for 2 seconds
    display.clearDisplay();
}
#endif