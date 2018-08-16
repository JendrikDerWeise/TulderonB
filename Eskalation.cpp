#include "Eskalation.h"

Eskalation::Eskalation(){
    eStufe = 1;
    zStufe = 0;
    fogPin = 7;
    fogTime = 10000;
    maxZstufe = 5;
}

void Eskalation::setup(NeoPixel *l){
    light = l;
	pinMode(fogPin, OUTPUT);
}

void Eskalation::raiseEstufe(){
    eStufe++;
}

void Eskalation::dezEstufe(){
    if(eStufe > 0)
        eStufe--;
}

void Eskalation::raiseZstufe(){
    if(zStufe == 5){
        zStufe = 0;
        raiseEstufe();
    }else        
        zStufe++;
    
    eskalate();
}

void Eskalation::dezZstufe(){
    if(zStufe > 0)
        zStufe--;
}

void Eskalation::redBlinking(){
    //light->redBlink(eStufe);
}

void Eskalation::alarm(){
    Serial.print("alarm");
}

void Eskalation::explosion(){
    Serial.print("bumm");
    //redBlinking();
    fogger();
}

void Eskalation::fogger(){
    unsigned long now = millis();
    digitalWrite(fogPin, HIGH);
    while(millis() - now < fogTime){
        //wait;
    }
    digitalWrite(fogPin, LOW);
}

void Eskalation::eskalate(){
    switch(eStufe){
        case 1:
            //redBlinking();
            break;
        case 2:
            alarm();
            //redBlinking();
            break;
        case 3:
            alarm();
            //redBlinking();
            //fogger();
            break;
        case 4:
            explosion();
            break;
    }
}

int Eskalation::getEstufe(){
    return eStufe;
}

int Eskalation::getZstufe(){
    return zStufe;
}