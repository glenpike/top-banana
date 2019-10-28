#ifndef SINGLE_LED_H
#define SINGLE_LED_H
#include <Arduino.h>

#include "Interfaces.h"

class Single_LED : public AbstractAnimateable {
  protected:
    int ledPin;
    
    AnimationConfig activeConfig;
    bool animationComplete = false;

    bool running = false;
    unsigned long updateInterval;
    unsigned long lastUpdate;

    uint16_t totalSteps;
    uint16_t currentStep;

    CallBackHandler *pCallbackHandler;

    int currentLedState;
    int nextLedState;
  public:

    Single_LED(int pin, CallBackHandler *handler);

    void SetCallback(CallBackHandler *handler = NULL) { pCallbackHandler = handler; }
    void SetPattern(AnimationConfig config);
    void Start();
    void Pause();
    void Reset();
    void Update();
    bool isAnimationComplete() { return animationComplete; }
    bool isRunning() { return running; }

    void Increment();
    // void TurnOn(AnimationConfig config);
    // void TurnOff(AnimationConfig config);
    void Flash(AnimationConfig config);

    void TurnOnUpdate();
    void TurnOffUpdate();
    void FlashUpdate();
};
#endif
