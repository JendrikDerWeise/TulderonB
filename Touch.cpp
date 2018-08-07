#include "Touch.h"

Touch::Touch(){
    cap = Adafruit_CAP1188();
    cap2 = Adafruit_CAP1188();
	
}

void Touch::setup(Stream *_serialRef){
	//display = disp;
	_serial = _serialRef;
    _serial->println("CAP1188 test!");

	if (!cap.begin(0x28)) {
		_serial->println("CAP1 not found");
		//display->showText("CAP1 not found");
		while (1);
	}
	_serial->println("CAP1 found!");
	//display->showText("CAP1 found!");
	delay(1000); 

	if (!cap2.begin(0x2C)) {
		_serial->println("CAP2 not found");
		//display->showText("CAP2 not found");
		while (1);
	}
	_serial->println("CAP2 found!");
	//display->showText("CAP2 found!");
	delay(1000);
}

void Touch::checkTouch(){
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
	int code[] = { 2,6,6,2 };
	int actualInput[2];
	bool failure = false;
	long startTime = millis();
	int counter = 0;

	while (millis() - startTime < inputTime) {
		if (cap.touched() && cap2.touched()) {
			uint8_t touched1 = cap.touched();
			actualInput[0] = getTouchedKey(touched1);
			_serial->print("A"); _serial->print(actualInput[0]); _serial->print("\t");
			uint8_t touched2 = cap2.touched();
			actualInput[1] = getTouchedKey(touched2);
			_serial->print("B"); _serial->print(actualInput[1]); _serial->print("\t\n");

			int in1 = actualInput[0];
			int in2 = actualInput[1];
			String str = "A" + String(in1) + "    " + "B" + String(in2) + "\n";
			//display->showText(str);


			if (actualInput[0] == actualInput[1]) {
				if (actualInput[0] == code[counter]) {
					counter++;
					while (cap.touched() || cap2.touched());
					delay(100);
					startTime = millis();
				}
				else {
					_serial->println("Fehleingabe");
					//display->showText("Fehleingabe");
					delay(500);
					return;
				}
			}
			else {
				_serial->println("Fehleingabe");
				//display->showText("Fehleingabe");
				delay(500);
				return;
			}

			if (counter == 4) {
				_serial->println("Geschafft");
				//display->showText("Geschafft");
				return;
			}
		}
	}
	_serial->println("Zeit um");
	//display->showText("Zeit um");
}