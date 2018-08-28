#include "Touch.h"

const int CAP1RST = 4;
const int CAP2RST = 5;

const int code[] = { 1,2,4,5,6,4,3,4,7,2,5 };

int letzteEingabe = 0;

bool failure = false;
bool inputTimerRunning = false;
long startTime = millis();
int counter = 0;


Touch::Touch(){
    cap = Adafruit_CAP1188();
    cap2 = Adafruit_CAP1188();
  	kristallFreigegeben = false;
  	letzteFehleingabe = 0;
  	releaseTime = 0;
    isReset = false;
    
}

void Touch::setup(Stream *_serialRef, Eskalation *e, NeoPixel *s){
	//display = disp;
  strip = s;
	_serial = _serialRef;
   // _serial->println("CAP1188 test!");

	if (!cap.begin(0x28)) {
		//_serial->println("CAP1 not found");
		//display->showText("CAP1 not found");
		while (1);
	}
	//_serial->println("CAP1 found!");
	//display->showText("CAP1 found!");
	delay(1000); 

	if (!cap2.begin(0x2C)) {
		//_serial->println("CAP2 not found");
		//display->showText("CAP2 not found");
		while (1);
	}
	//_serial->println("CAP2 found!");
	//display->showText("CAP2 found!");
	delay(1000);

	eskalation = e;
 pinMode(CAP1RST, OUTPUT);
 pinMode(CAP2RST, OUTPUT);
}

void Touch::hold(){
  if(!isReset){
    digitalWrite(CAP1RST, HIGH);
    digitalWrite(CAP2RST, HIGH);
    isReset = true;
  }
}

void Touch::unhold(){
  if(isReset){
    digitalWrite(CAP1RST, LOW);
    digitalWrite(CAP2RST, LOW);
    isReset = false;
  }
}

void Touch::reset(){
	cap.writeRegister(0x2A, 0x80); // 0x2A default 0x80 use 0x41  — Set multiple touches back to off
	cap.writeRegister(0x44, 0x41);  // 0x44 default 0x40 use 0x41  — Set interrupt on press but not release
	cap.writeRegister(0x72, 0x00);  // 0x72 default 0x00  — Sets LED links back to off (default)

	cap2.writeRegister(0x2A, 0x80); // 0x2A default 0x80 use 0x41  — Set multiple touches back to off
	cap2.writeRegister(0x44, 0x41);  // 0x44 default 0x40 use 0x41  — Set interrupt on press but not release
	cap2.writeRegister(0x72, 0x00);  // 0x72 default 0x00  — Sets LED links back to off (default)
 
}

void Touch::checkTouch(){
  
  if(isReset)
    return;

    if (inputTimerRunning && millis() - startTime > inputTime) {
     // _serial->println("Zeit um");
      //display->showText("Zeit um");
      fehleingabe();
    }
    
  if (cap.touched() || cap2.touched())
		checkCode();

	delay(50);
}

int Touch::getTouchedKey(uint8_t touched) {
	for (uint8_t i = 0; i<8; i++) {
		if (touched & (1 << i)) {
			
			return i+1;
		}
	}
}

void Touch::checkCode() {
  //_serial->println("checktouch");
	
	if(!inputTimerRunning){
    startTime = millis();
    inputTimerRunning = true;
	}

	if (millis() - startTime < inputTime) {
		
		if (cap.touched() && cap2.touched()) {
			uint8_t touched1 = cap.touched();
			actualInput[0] = getTouchedKey(touched1);
			//_serial->print("A"); _serial->print(actualInput[0]); _serial->print("\t");
			uint8_t touched2 = cap2.touched();
			actualInput[1] = getTouchedKey(touched2);
			//_serial->print("B"); _serial->print(actualInput[1]); _serial->print("\t\n");

			int in1 = actualInput[0];
			int in2 = actualInput[1];
			//String str = "A" + String(in1) + "    " + "B" + String(in2) + "\n";

      

			if (actualInput[0] == actualInput[1]) {
        if(actualInput[0] != letzteEingabe){
          //_serial->print("actualInput "); _serial->print(actualInput[0]); _serial->print(" letzte Eingabe "); _serial->println(letzteEingabe);
  				if (actualInput[0] == code[counter]) {
            letzteEingabe = actualInput[0];
  					counter++;
            //_serial->println("jo");
  					while (cap.touched() || cap2.touched()){
  					  //_serial->println("while");
  					  strip->confirm();
  					  }
  					delay(100);
  					startTime = millis();
  				}
  				else {
  					//_serial->println("Fehleingabe1");
  					fehleingabe();
  					delay(500);
  					return;
  				}
        }
			}
			else {
				//_serial->println("Fehleingabe2");
				fehleingabe();
				delay(500);
				return;
			}

			if (counter == 11) {
				//_serial->println("Geschafft");
				//display->showText("Geschafft");
				releaseKristall();
        counter = 0;
        letzteEingabe = 0;
        inputTimerRunning = false;
				return;
			}
		}
	}else{
  	//41_serial->println("Zeit um");
  	//display->showText("Zeit um");
  	fehleingabe();
	}
}

bool Touch::isKristallFreigegeben(){
	if(millis() - releaseTime > defuseTime)
		kristallSperren();
    return kristallFreigegeben;
}

void Touch::kristallSperren(){
	resetActualInput();
	kristallFreigegeben = false;
}

void Touch::releaseKristall(){
	releaseTime = millis();
	kristallFreigegeben = true;
}

unsigned long Touch::getReleaseTime(){
	return releaseTime;
}

unsigned long Touch::getDefuseTime(){
	return defuseTime;
}

void Touch::fehleingabe(){
  counter = 0;
  inputTimerRunning = false;
  letzteEingabe = 0;
  
	if(isKristallFreigegeben()){
		resetActualInput();
		return;
	}

 if(isReset)
  return;

	if(actualInput[0] == 0 || actualInput[1] == 0){
		resetActualInput();
		return;
	}
	
	if(millis() - letzteFehleingabe > 3000){
		eskalation->raiseZstufe();
		letzteFehleingabe = millis();
	}

	resetActualInput();
}

void Touch::resetActualInput(){
	actualInput[0] = 0;
	actualInput[1] = 0;
}
