#ifndef _NeoPixel_h_included_
#define _NeoPixel_h_included_

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

class NeoPixel{
    private:
        void SetRandomSeed();
        void redBlink();

        int fader;
        int mpMultiplier;
        bool isBlinking;
        int blinked;
        unsigned long startTime;
        int blinkTimes;

    public:
        NeoPixel();
        void setup();
        void makeLight();
        void switchColor(int);
        void fadeToRed(unsigned long kristallTime, unsigned long timeUsed);
        void setEffectState();
        void setMPmultiplier(int mp);
        void setBlinking(int times);
        void confirm();
};
#endif
