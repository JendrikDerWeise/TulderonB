#ifndef _NeoPixel_h_included_
#define _NeoPixel_h_included_

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

class NeoPixel{
    private:
        void SetRandomSeed();
        

    public:
        NeoPixel();
        void setup();
        void makeLight();
        void switchColor(int);
};
#endif