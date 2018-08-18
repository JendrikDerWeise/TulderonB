#include "Eskalation.h"

Eskalation::Eskalation(){
    eStufe = 1;
    zStufe = 0;
    fogPin = 7;
    fogTime = 10000;
    maxZstufe = 5;
    fogging = false;
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
    //Serial.print("e: ");Serial.print(eStufe);Serial.print(" z: ");Serial.println(zStufe);
}

void Eskalation::dezZstufe(){
    if(zStufe > 0)
        zStufe--;
}

void Eskalation::redBlinking(){
    light->setBlinking(eStufe);
}

void Eskalation::alarm(){
    Serial.print("alarm");
}

void Eskalation::explosion(){
    Serial.print("bumm");
    redBlinking();
    startFogger();
}

void Eskalation::startFogger(){
    startTime = millis();
    fogging = true;
}

void Eskalation::fogger(){
    unsigned long now = millis() - startTime;

    if(now < fogTime)
        digitalWrite(fogPin, HIGH);
    else{
        digitalWrite(fogPin, LOW);
        fogging = false;
    }
}

bool Eskalation::isFogging(){
    return fogging;
}

void Eskalation::eskalate(){
    switch(eStufe){
        case 1:
            redBlinking();
            break;
        case 2:
            alarm();
            redBlinking();
            break;
        case 3:
            alarm();
            redBlinking();
            startFogger();
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