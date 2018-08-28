#ifndef _Touch_h_included_
#define _Touch_h_included_

#include "Adafruit_CAP1188.h"
#include "Eskalation.h"

class Touch{
    public:
        void setup(Stream *_serial, Eskalation *e, NeoPixel *s);
        void checkTouch();
        Touch();
        bool isKristallFreigegeben();
        void kristallSperren();
        void releaseKristall();
        void reset();
        void hold();
        void unhold();
        unsigned long getReleaseTime();
        unsigned long getDefuseTime();

    private:
        Adafruit_CAP1188 cap;
        Adafruit_CAP1188 cap2;
        int actualInput[2];
        long inputTime = 3000;
        bool kristallFreigegeben;
        bool isReset;
        void checkCode();
        void fehleingabe();
        void resetActualInput();
        int getTouchedKey(uint8_t touched);
        unsigned long releaseTime;
        unsigned long letzteFehleingabe;
        //Display *display;
        Stream *_serial;
        Eskalation *eskalation;
        NeoPixel *strip;

        const unsigned long defuseTime = 3600000; //1h = 3600000 1min = 60000
        

};
#endif
