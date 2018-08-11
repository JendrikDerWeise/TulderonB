#ifndef _NeoPixel_h_included_
#define _NeoPixel_h_included_

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

class NeoPixel{
    private:
        void SetRandomSeed();
        
        int actualColor;
        int fader;
        int lastColor;
        int blinkTasks;

    public:
        NeoPixel();
        void setup();
        void makeLight();
        void switchColor(int);
        void fadeToRed(unsigned long kristallTime, unsigned long timeUsed);
        void redBlink(int);
        void setEffectState();
};
#endif