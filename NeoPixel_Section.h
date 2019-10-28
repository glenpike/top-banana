#ifndef NEOPIXEL_SECTION_H
#define NEOPIXEL_SECTION_H
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "Interfaces.h"

class NeoPixel_Section : public AbstractAnimateable {

  public:
    Adafruit_NeoPixel* ledStrip;
    uint16_t stripStart;
    uint16_t stripLength;

    Animation activeAnimation;
    Direction direction;
    bool animationComplete = false;

    bool running = false;
    unsigned long updateInterval;
    unsigned long lastUpdate;

    uint32_t color1, color2;
    uint16_t totalSteps;
    uint16_t currentStep;

    CallBackHandler *pCallbackHandler;

    NeoPixel_Section(Adafruit_NeoPixel* strip, uint16_t start, uint16_t length,
                     CallBackHandler *handler);
                     
    void SetCallback(CallBackHandler *handler = NULL) { pCallbackHandler = handler; }
    void SetPattern(AnimationConfig config);
    void Start();
    void Pause();
    void Reset();
    void Update();
    bool isAnimationComplete() { return animationComplete; }
    bool isRunning() { return running; }

    // Consider 'single-shot' animation or continuous?
    void Increment();
    void Reverse();

    // uint8_t updateInterval, uint32_t color1
    void StripOn(AnimationConfig config);
    // uint8_t updateInterval
    void StripOff(AnimationConfig config);
    // uint8_t updateInterval, Direction dir = FORWARD)
    void RainbowCycle(AnimationConfig config);
    // uint8_t updateInterval, Direction dir = FORWARD, uint32_t color1, uint32_t
    // color2)
    void TheaterChase(AnimationConfig config);
    // uint8_t updateInterval, Direction dir = FORWARD, uint32_t color)
    void ColorWipe(AnimationConfig config);
    // uint8_t updateInterval, uint32_t color1)
    void Scanner(AnimationConfig config);
    // uint8_t updateInterval, Direction dir = FORWARD, uint32_t color1, uint32_t
    // color2, uint16_t steps)
    void Fade(AnimationConfig config);

    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color);

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color);

    // Returns the colour components
    uint8_t Red(uint32_t color);
    uint8_t Green(uint32_t color);
    uint8_t Blue(uint32_t color);

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte wheelPos);

  protected:
    void StripAllUpdate();

    void RainbowCycleUpdate();
    void TheaterChaseUpdate();

    void ColorWipeUpdate();

    void ScannerUpdate();

    void FadeUpdate();
};
#endif
