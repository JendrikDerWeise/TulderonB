
#include "Touch.h"
#include "Eskalation.h"
#include "KristallReader.h"
#include "NeoPixel.h"

const int PURPLE = 1;
const int RED = 2;
const int GREEN = 3;
const unsigned long kristallTime = 60000; //2h = 7200000

Touch touch;
KristallReader kristall;
NeoPixel light;
Eskalation eskalation;
int count = 0;
unsigned long momentOfKristallRelease;
bool timerRunning = false;
bool isIdle = true;
String msg;
int waitingTimeForKristall = 20000;

HardwareSerial *bt = &Serial;

int estufe = 0;
int zstufe = 0;

void kristallCheck(){
	if(touch.isKristallFreigegeben()){
		if(timerRunning)
			if(millis() - momentOfKristallRelease  > kristallTime){ //Kristall zu lange entnommen
				eskalation.raiseZstufe();
				unsigned long now = millis();
				while(millis() - now < waitingTimeForKristall){}
			}
			else{
				light.fadeToRed(kristallTime, millis() - momentOfKristallRelease);
				return;
			}
		
		else{
			light.switchColor(GREEN);
			momentOfKristallRelease = millis();
			isIdle = false;
			timerRunning = true;
			light.setEffectState();
			unsigned long now = millis();
			while(millis() - now < waitingTimeForKristall){light.makeLight();}
		}
	}
}

void btNachricht(){
	msg = Serial.readString();
	if (msg == "aktualisieren") {
		String str = String(kristall.getMPInKristall());
		str += "?";
		str += eskalation.getEstufe();  
		str += "?";
		str += eskalation.getZstufe();
		char buffer[50];
		str.toCharArray(buffer,20);
		bt->print(buffer);
    }
	else if(msg == "eplus")
		eskalation.raiseEstufe();
	
	else if(msg == "eminus")
		eskalation.dezEstufe();
	
	else if(msg == "zplus")
		eskalation.raiseZstufe();
	
	else if(msg == "zminus")
		eskalation.dezZstufe();

	else if(msg == "freigeben")
		touch.releaseKristall();

	else if(msg == "sperren")
		touch.kristallSperren();

	else if(msg == "blinken")
		eskalation.redBlinking();

	else if(msg == "alarm"){}
		//alarm
	else if(msg == "nebel")
		eskalation.fogger();

	else if(msg == "eskalieren")
		eskalation.eskalate();

	else if(msg == "entschaerf"){}
		//entschärfmodus starten
	else
		Serial.println("Kommando nicht bekannt");
	

}

void setup() {
	Serial.begin(9600);
	touch.setup(&Serial, &eskalation);
	kristall.setup();
	light.setup();
	eskalation.setup(&light);
}


void loop() {
	if(kristall.isKristallPresent()){
		if(!isIdle){
			light.setEffectState();
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
		if(!touch.isKristallFreigegeben()){
			//eskalation++;
			eskalation.raiseZstufe();
			isIdle = false;
			light.switchColor(RED);
			light.setEffectState();
			unsigned long now = millis();
			while(millis() - now < waitingTimeForKristall){light.makeLight();}
		}
	}

	kristallCheck();

	if (Serial.available()){      // Daten liegen an
    	btNachricht();		
	}
	
	kristall.checkMPInKristall();
	touch.checkTouch();
	light.makeLight();
}