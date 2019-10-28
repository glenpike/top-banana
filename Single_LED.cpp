#include "Single_LED.h"

Single_LED::Single_LED(int pin, CallBackHandler *handler = NULL) {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);
    currentLedState = LOW;
    pCallbackHandler = handler;
    digitalWrite(ledPin, currentLedState);
}

void Single_LED::SetPattern(AnimationConfig config) {
    activeConfig = config;
    updateInterval = config.updateInterval;
    totalSteps = config.steps;
    currentStep = 0;
    animationComplete = false;
    switch (activeConfig.animation) {
    case FLASH:
        Flash(config);
        break;
    default:
        break;
    }
}

void Single_LED::Start() { running = true; }
void Single_LED::Pause() { running = false; }
void Single_LED::Reset() { currentStep = 0; }

void Single_LED::Update() {
    if (running && (millis() - lastUpdate) > updateInterval) // time to update
    {
        lastUpdate = millis();
        switch (activeConfig.animation) {
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
    if (currentStep >= totalSteps) {
        currentStep = 0;
        if (animationComplete == false) {
            animationComplete = true;
            if (pCallbackHandler != NULL) {
                pCallbackHandler->OnComplete(static_cast<void*>(this));
            }
        }
    }
}

void Single_LED::Flash(AnimationConfig config) {
  if(currentLedState == HIGH) {
    nextLedState = LOW;
  } else {
    nextLedState = HIGH;
  }
}

void Single_LED::TurnOnUpdate() {
  if(totalSteps == 0 || currentStep == totalSteps - 1) {
    currentLedState = HIGH;
  }
}
void Single_LED::TurnOffUpdate() {
  if(totalSteps == 0 || currentStep == totalSteps - 1) {
    currentLedState = LOW;
  }
}
void Single_LED::FlashUpdate() {
  if(currentStep < totalSteps / 2 && currentLedState != nextLedState) {
    currentLedState = nextLedState;
  } else if (currentStep >= totalSteps / 2 && currentLedState == nextLedState) {
    if(currentLedState == LOW) {
      currentLedState = HIGH;
      nextLedState = LOW;
    } else {
      currentLedState = LOW;
      nextLedState = HIGH;
    }
  }
}
