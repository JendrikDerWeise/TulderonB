
#include "Touch.h"
#include "Display.h"
#include "KristallReader.h"
#include "NeoPixel.h"

Display display;
Touch touch;
KristallReader kristall;
NeoPixel light;
int count = 0;

void setup() {
	Serial.begin(9600);
	display.setup();
	touch.setup(&Serial);
	kristall.setup();
	light.setup();
	pinMode(7, OUTPUT);
	
}


void loop() {
	display.showText("blabla");
	//Serial.print("blabla");
	//Serial.println(count);
	//digitalWrite(7, HIGH);
	

	light.makeLight();

	
	count++;
	
	if(kristall.isMPChanged()){
		int mp = kristall.getMPInKristall();
		String mpStr = String(mp);
		display.showText(mpStr);
		Serial.println(mp);
		light.switchColor(3);
	}
	
	kristall.checkMPInKristall();
	touch.checkTouch();
}