#include "NeoPixel.h"

const uint16_t PixelCount = 24;
const uint8_t PixelPin = 8;
const uint8_t AnimationChannels = 1;
const uint16_t animationTime = 10000;

RgbColor target(255,0,255);
uint16_t effectState;
uint16_t time;

NeoPixelAnimator animations(AnimationChannels);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
};

MyAnimationState animationState[AnimationChannels];

NeoPixel::NeoPixel(){
    effectState = 0;
    fader = 0;
    time = animationTime;
    blinkTasks = 0;
    actualColor = 1;
}


void NeoPixel::setup(){
    strip.Begin();
    strip.Show();

    SetRandomSeed();
}


void NeoPixel::SetRandomSeed()
{
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits
    seed = analogRead(0);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(0) << shifts;
        delay(1);
    }

    randomSeed(seed);
}

void BlendAnimUpdate(const AnimationParam& param)
{
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);

    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip.SetPixelColor(pixel, updatedColor);
    }
}

void FadeInFadeOutRinseRepeat(float luminance, RgbColor target)
{
    if (effectState == 0)
    {
        animationState[0].StartingColor = strip.GetPixelColor(0);
        animationState[0].EndingColor = target;

        animations.StartAnimation(0, time, BlendAnimUpdate);
    }
    else if (effectState == 1)
    {
        // fade to black
        animationState[0].StartingColor = strip.GetPixelColor(0);
        animationState[0].EndingColor = RgbColor(0);

        animations.StartAnimation(0, time, BlendAnimUpdate);
    }

    effectState = (effectState + 1) % 2;
}

void NeoPixel::makeLight()
{
    if (animations.IsAnimating())
    {
        animations.UpdateAnimations();
        strip.Show();
    }
    else
    {
        FadeInFadeOutRinseRepeat(1.0f, target); // 0.0 = black, 0.25 is normal, 0.5 is bright
    }
}

void NeoPixel::switchColor(int color){
        animations.StopAnimation(0);
        switch(color){
            case 1: //purple
                fader = 0;
                time = animationTime;
                target = RgbColor(255,0,255);
                break;
            case 2: //red
                target = RgbColor(255,0,0);
                break;
            case 3: //green
                target = RgbColor(50,255,10);
                break;
            default:
                break;
        }
        strip.ClearTo(0);
        //strip.Show();    
}


void NeoPixel::redBlink(int times){
    for(int i = 0; i < times; i++){
        for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
            strip.SetPixelColor(pixel, RgbColor(255,0,0));
        strip.Show();
        unsigned long now = millis();
        while(millis()-now < 500){}
        for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
            strip.SetPixelColor(pixel, RgbColor(0,0,0));
        strip.Show();
        now = millis();
        while(millis()-now < 300){}
    }
    makeLight();
}

void NeoPixel::fadeToRed(unsigned long kristallTime, unsigned long timeUsed){
    unsigned long tick = kristallTime / 255;

    if(timeUsed > (fader+1) * tick && fader < 256){
        fader++;
        target = RgbColor(fader,255 - fader,10);
        if(time > 500){
            time -= animationTime / 255;
            //Serial.println(time);
        }
    }
}

void NeoPixel::setEffectState(){
    effectState = 0;
}
