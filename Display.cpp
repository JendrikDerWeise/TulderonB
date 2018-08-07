#include "Display.h"

Adafruit_SSD1306 disp(-1);
const byte OLED_ADDR = 0x3C;

void Display::setup(){
    disp.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
	disp.clearDisplay();
	disp.display();
	disp.setTextSize(1);
    disp.setTextColor(WHITE);
    disp.setCursor(27,30);
    disp.print("Hello, world!");

    disp.display();
}

void Display::showText(String str){
    disp.clearDisplay();
    disp.setCursor(0,0);
    disp.print(str);

    disp.display();
}