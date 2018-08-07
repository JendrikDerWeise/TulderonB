#ifndef _Touch_h_included_
#define _Touch_h_included_

#include "Adafruit_CAP1188.h"

class Touch{
    public:
        void setup(Stream *_serial);
        void checkTouch();
        Touch();
        bool isKristallFreigegeben();
        void kristallSperren();
        
    private:
        Adafruit_CAP1188 cap;
        Adafruit_CAP1188 cap2;
        long inputTime = 2000;
        bool kristallFreigegeben;
        void checkCode();
        int getTouchedKey(uint8_t touched);
        //Display *display;
        Stream *_serial;
        

};
#endif