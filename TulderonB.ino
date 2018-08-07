
#include "Touch.h"
#include "Display.h"
#include "KristallReader.h"
#include "NeoPixel.h"

const int PURPLE = 1;
const int RED = 2;
const int GREEN = 3;
const unsigned long kristallTime = 60000; //2h = 7200000

Display display;
Touch touch;
KristallReader kristall;
NeoPixel light;
int count = 0;
unsigned long momentOfKristallRelease;
bool timerRunning = false;
bool isIdle = true;

void kristallCheck(){
	if(timerRunning)
		if(millis() - momentOfKristallRelease  > kristallTime) //Kristall zu lange entnommen
		{Serial.println("bla");}
			//ja --> eskalation (zeitliche abstände setzen um mehrfach eskalation zu vermeiden)
		else{
			light.fadeToRed(kristallTime, millis() - momentOfKristallRelease);
			return;
		}
	
	else{
		light.switchColor(GREEN);
		momentOfKristallRelease = millis();
		isIdle = false;
		timerRunning = true;
	}
}

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
	//digitalWrite(7, HIGH);
	

	light.makeLight();

	if(kristall.isKristallPresent()){
		if(!isIdle){
			light.switchColor(PURPLE);
			timerRunning = false;
			isIdle = true;
			touch.kristallSperren();
		}

		if(kristall.getMPInKristall() == 0){
			//Entschärfbar
		}
	}
	else{
		if(touch.isKristallFreigegeben()){
			kristallCheck();
		}
		else{
			//eskalation++;
			isIdle = false;
			light.switchColor(RED);
		}
	}
	
	kristall.checkMPInKristall();
	touch.checkTouch();
}