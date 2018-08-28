#include "Touch.h"
#include "Eskalation.h"
#include "KristallReader.h"
#include "NeoPixel.h"

const int PURPLE = 1;
const int RED = 2;
const int GREEN = 3;
const int BLACK = 4;
const int WHITEBLUE = 5;
const int ENTSCHAERFKABEL = 2;
const unsigned long crystalTime = 20000; //20sek
const unsigned long sensitiveTime = 2000; //2sek

Touch touch;
KristallReader kristall;
NeoPixel light;
Eskalation eskalation;
HardwareSerial *bt = &Serial;

//ZUSTÄNDE
bool idle = true;
bool kristallFreigegeben = false;//nicht verwendet
bool entschaerfbar = false;
bool kristallEingesetzt = true;//nicht verwendet
bool timerIsRunning = false;
bool explodiert = false;
bool entschaerft = false;
bool kristallWarWeg = false;
bool defuseTimerIsRunning = false;
bool sensitiveTimerRunning = false;

//TIMER
unsigned long crystalTimer;
unsigned long lastCheck;
unsigned long momentOfFirstMissing;

//PARKPLÄTZE
String msg = "";

void setIdle(){
	light.setMPmultiplier(kristall.getMPInKristall());
	light.setEffectState();
	light.switchColor(PURPLE);
	idle = true;
	
}

void crystalMissing(){
  if(!sensitiveTimerRunning){
    sensitiveTimerRunning = true;
    momentOfFirstMissing = millis();
    //Serial.println("timer runs");
    return;
  }
  if(millis() - momentOfFirstMissing > sensitiveTime){
  	idle = false;
  	eskalation.raiseZstufe();
  	timerIsRunning = true;
  	//starte timer
  	crystalTimer = millis();
  	light.setEffectState();
  	light.switchColor(RED);
    sensitiveTimerRunning = false;
   // Serial.println("ausgelöst");
  }
}

void btNachricht(){
	msg = bt->readString();
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

	else if(msg == "alarm")
		eskalation.alarm();

	else if(msg == "nebel")
		eskalation.startFogger();

	else if(msg == "explodieren")
		eskalation.explosion();

	else if(msg == "eskalieren")
		eskalation.eskalate();

	else if(msg == "entschaerf")
		kristall.setMPInKristall(0);
	else
		Serial.println("Kommando nicht bekannt");
}

void setup(){
	Serial.begin(9600);	
	light.setup();
	kristall.setup();
	eskalation.setup(&light);
	touch.setup(&Serial, &eskalation, &light);
	pinMode(ENTSCHAERFKABEL, INPUT_PULLUP);
}

void loop(){
	if(entschaerft){
		light.setEffectState();
		light.switchColor(BLACK);
		light.makeLight();
		return;
	}

	if (Serial.available())      // Daten liegen an
    	btNachricht();		

	if(entschaerfbar){
			if(digitalRead(ENTSCHAERFKABEL) == HIGH){
				entschaerft = true;
				light.setEffectState();
				light.switchColor(BLACK);
				light.makeLight();
			}
	}

	if(idle){
		if(kristall.getMPInKristall() < 1){//MP = 0
			entschaerfbar = true;
			idle = false;
			light.setEffectState();
			light.switchColor(WHITEBLUE);
		}
		else
			if(digitalRead(ENTSCHAERFKABEL) == HIGH){//Kabel durchgeschnitten bevor MP = 0
				if(!explodiert)
					eskalation.explosion();
					explodiert = true;
			}
	}

	if(kristall.isKristallPresent()){
    if(sensitiveTimerRunning && millis() - momentOfFirstMissing > sensitiveTime){
      sensitiveTimerRunning = false;
    }
    touch.unhold();
		if(touch.isKristallFreigegeben()){
			if(kristallWarWeg && defuseTimerIsRunning){
				setIdle();
				defuseTimerIsRunning = false;
				kristallWarWeg = false;
				touch.kristallSperren();
			}

			if(!defuseTimerIsRunning && touch.isKristallFreigegeben()){
				defuseTimerIsRunning = true;
				light.setEffectState();
				light.switchColor(GREEN);
				light.makeLight();
				idle = false;
			}
		}
		if(!idle && !entschaerfbar && !defuseTimerIsRunning)
			setIdle();
		if(timerIsRunning)
			timerIsRunning = false;
			
	}
	else{//Kristall nicht da
    touch.hold();
		if(!touch.isKristallFreigegeben()){
			if(!timerIsRunning){//Kristall gerade erst weggenommen
				crystalMissing();
			}else{
				if(millis() - crystalTimer > crystalTime){//Kristall länger als crystalTime weg
					crystalMissing();
				}
			}
		}else{
			if(!kristallWarWeg){
			    kristallWarWeg = true;
			}
			if(defuseTimerIsRunning){
				light.fadeToRed(touch.getDefuseTime(), millis() - touch.getReleaseTime());
			}
		}
	}

	light.makeLight();

	if(eskalation.isFogging())
		eskalation.fogger();

	kristall.checkMPInKristall();
	
	if(!touch.isKristallFreigegeben()){
		defuseTimerIsRunning = false;
		kristallWarWeg = false;
		touch.checkTouch();
	}
}














/*
int count = 0;
unsigned long momentOfKristallRelease;
bool timerRunning = false;
bool isIdle = true;
bool colorChanged = false;
String msg;
int waitingTimeForKristall = 20000;

HardwareSerial *bt = &Serial;

void kristallCheck(){
	if(touch.isKristallFreigegeben()){
		if(timerRunning)
			if(millis() - momentOfKristallRelease  > kristallTime){ //Kristall zu lange entnommen
				eskalation.raiseZstufe();
				unsigned long now = millis();
				touch.kristallSperren();
				//while(millis() - now < waitingTimeForKristall){}
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
			//while(millis() - now < waitingTimeForKristall){light.makeLight();}
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

	else if(msg == "alarm")
		eskalation.alarm();

	else if(msg == "nebel")
		eskalation.fogger();

	else if(msg == "explodieren")
		eskalation.explosion();

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
	//pinMode(entschaerfKabel, INPUT);
}


void loop() {/*
	if(digitalRead(entschaerfKabel) == LOW){
		if(kristall.getMPInKristall() < 1){
			light.setEffectState();
			light.switchColor(BLACK);
			return;		
		}else
		eskalation.explosion();
	}*/
/*
	if(kristall.isKristallPresent() ){
		if(!isIdle && !touch.isKristallFreigegeben()){
			light.setEffectState();
			light.switchColor(PURPLE);
			timerRunning = false;
			isIdle = true;
			touch.kristallSperren();
			light.setMPmultiplier(kristall.getMPInKristall());
		}

		if(kristall.getMPInKristall() == 0){
			/*light.setEffectState();
			light.switchColor(WHITEBLUE);*/
			
/*		}
	}
	else{
		if(!touch.isKristallFreigegeben()){
			//eskalation++;
			eskalation.raiseZstufe();
			isIdle = false;
			light.switchColor(RED);
			light.setEffectState();
			unsigned long now = millis();
			while(millis() - now < waitingTimeForKristall){
				if(kristall.isKristallPresent()){
					
					light.setEffectState();
					light.switchColor(PURPLE);
					//kristall.checkMPInKristall();
					light.setMPmultiplier(kristall.getMPInKristall());
					break;
				}
				light.makeLight();
			}
		}
	}

	kristallCheck();
//kristall.readMPs();
	if (Serial.available()){      // Daten liegen an
    	btNachricht();		
	}
	
	kristall.checkMPInKristall();
	touch.checkTouch();
	light.makeLight();
	//Serial.print("ino: ");
	//Serial.println(kristall.getMPInKristall());
}*/
