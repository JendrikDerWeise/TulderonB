#ifndef _Display_h_included_
#define _Display_h_included_

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



class Display{

    public:
        void setup();
        void showText(String);
};
#endif