#ifndef SEQUENCE_H
#define SEQUENCE_H
#define SERIAL_DEBUG 1

#include "Interfaces.h"


class Sequence : public CallBackHandler, public AbstractPlayer {
  public:
    // Array of Sections
    AbstractAnimateable** sections;
    uint8_t numSections;

    bool isParallel = true;
    bool animationComplete = false;
    bool running = false;
    uint8_t currentSection;

    CallBackHandler *pCallbackHandler;
    // void (*OnCompleteHandler)(); // Callback on completion of pattern

    Sequence(CallBackHandler *handler) { //void (*callback)()) {
        currentSection = 0;
        pCallbackHandler = handler;
        // OnCompleteHandler = callback;
    }

    void SetCallback(CallBackHandler *handler = NULL) { pCallbackHandler = handler; }

    bool isAnimationComplete() { return animationComplete; }
    bool isRunning() { return running; }
    void Start() { running = true; }
    void Pause() { running = false; }
    void Reset() {
      currentSection = 0;
      for (byte i = 0; i < numSections; i++) {
          sections[i]->Reset();
      }
    }

    void SetAnimateables(AbstractPlayer* sects[], uint8_t len) {
        numSections = len;
#ifdef SERIAL_DEBUG
        Serial.print(F("SetAnimateables "));
        Serial.println(numSections);
#endif
        sections = sects;
        currentSection = 0;

        for (byte i = 0; i < numSections; i++) {
            sections[i]->SetCallback(this);
        }
    }

    void SetAnimations(AnimationConfig* configs[], uint8_t len, bool parallel = true) {
        isParallel = parallel;
        for (byte i = 0; i < numSections; i++) {
            if (i < len) {
                sections[i]->Pause();
                sections[i]->Reset();
                sections[i]->SetPattern(configs[i]);
            }
        }
        currentSection = 0;
    }

    void Update() {
        if(running) {
          if(isParallel) {
            for (byte i = 0; i < numSections; i++) {
                if(!sections[i]->isRunning()) {
                  sections[i]->Start();
                }
                sections[i]->Update();
            }
          } else if(currentSection < numSections) {
            // In series, Rainbow, etc. don't update if they're not current - is wrong?
            if(!sections[currentSection]->isRunning()) {
              sections[currentSection]->Start();
            }
            sections[currentSection]->Update();
          }
        }
    }

    void OnComplete(void *p) {
        bool patternComplete = true;
        //Parallel is a bit buggy??
        if(isParallel) {
          for (byte i = 0; i < numSections; i++) {
#ifdef SERIAL_DEBUG
            if (sections[i] == p) {
              Serial.print(F("Sequence::OnComplete - section completed: "));
              Serial.print(i);
            }
            // Serial.print(F(" section "));
            // Serial.print(i);
            // Serial.print(F(" pattern complete? "));
            // Serial.println(sections[i]->isAnimationComplete());
#endif
            if (sections[i]->isAnimationComplete() == false) {
                 patternComplete = false;
            }
          }
        } else {
          if(p != sections[currentSection]) {
            Serial.print(F("not section we wanted! "));
          } else {
            Serial.print(F("current section complete "));
            Serial.println(currentSection);
            currentSection++;
            if(currentSection != numSections) {
              patternComplete = false;
            }
          }
        }

        if (patternComplete == true) {
          if (pCallbackHandler != NULL) {
              Serial.println(F("calling OnComplete "));
              pCallbackHandler->OnComplete(static_cast<void*>(this));
          }
        }
    }
};
#endif
