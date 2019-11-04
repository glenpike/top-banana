#include "Single_LED.h"

Single_LED::Single_LED(int pin, CallBackHandler *handler = NULL) {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);
    currentLedState = LOW;
    pCallbackHandler = handler;
    digitalWrite(ledPin, currentLedState);
}

void Single_LED::SetPattern(AnimationConfig *config) {
    pAnimationConfig = config;
    currentStep = 0;
    animationComplete = false;
    switch (pAnimationConfig->animation) {
    case FLASH:
        if(currentLedState == HIGH) {
          nextLedState = LOW;
        } else {
          nextLedState = HIGH;
        }
        break;
    default:
        break;
    }
}

void Single_LED::Start() { running = true; }
void Single_LED::Pause() { running = false; }
void Single_LED::Reset() { currentStep = 0; }

void Single_LED::Update() {
    if (running && (millis() - lastUpdate) > pAnimationConfig->updateInterval) // time to update
    {
        lastUpdate = millis();
        switch (pAnimationConfig->animation) {
        case ON:
            TurnOnUpdate();
            break;
        case OFF:
            TurnOffUpdate();
            break;
        case FLASH:
            FlashUpdate();
            break;
        default:
            break;
        }
        Increment();
        digitalWrite(ledPin, currentLedState);
    }
}

void Single_LED::Increment() {
    currentStep++;
    if (currentStep >= pAnimationConfig->steps) {
        currentStep = 0;
        if (animationComplete == false) {
            animationComplete = true;
            if (pCallbackHandler != NULL) {
                pCallbackHandler->OnComplete(static_cast<void*>(this));
            }
        }
    }
}

void Single_LED::TurnOnUpdate() {
  if(pAnimationConfig->steps == 0 || currentStep == pAnimationConfig->steps - 1) {
    currentLedState = HIGH;
  }
}
void Single_LED::TurnOffUpdate() {
  if(pAnimationConfig->steps == 0 || currentStep == pAnimationConfig->steps - 1) {
    currentLedState = LOW;
  }
}
void Single_LED::FlashUpdate() {
  if(currentStep < pAnimationConfig->steps / 2 && currentLedState != nextLedState) {
    currentLedState = nextLedState;
  } else if (currentStep >= pAnimationConfig->steps / 2 && currentLedState == nextLedState) {
    if(currentLedState == LOW) {
      currentLedState = HIGH;
      nextLedState = LOW;
    } else {
      currentLedState = LOW;
      nextLedState = HIGH;
    }
  }
}
