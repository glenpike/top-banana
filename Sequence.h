#ifndef SEQUENCE_H
#define SEQUENCE_H
// #define SEQUENCE_DEBUG 1

#include "Interfaces.h"


class Sequence : CallBackHandler {
  public:
    // Array of Sections
    AbstractAnimateable** sections;
    uint8_t numSections;

    bool isParallel = true;
    bool running = false;
    uint8_t currentSection;

    void (*OnCompleteHandler)(); // Callback on completion of pattern

    // Array to monitor 'OnComplete' callbacks
    Sequence(void (*callback)()) {
        currentSection = 0;
        OnCompleteHandler = callback;
    }

    void Start() { running = true; }
    void Pause() { running = false; }
    void Reset() {
      currentSection = 0;
      for (byte i = 0; i < numSections; i++) {
          sections[i]->Reset();
      }
    }

    void SetAnimateables(AbstractAnimateable* sects[], uint8_t len) {
        numSections = len;
#ifdef SEQUENCE_DEBUG
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
#ifdef SEQUENCE_DEBUG
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
#ifdef SEQUENCE_DEBUG
            Serial.println(F("not section we wanted! "));
#endif
            patternComplete = false;
          } else {
#ifdef SEQUENCE_DEBUG
            Serial.print(F("current section complete "));
            Serial.println(currentSection);
#endif
            currentSection++;
            if(currentSection != numSections) {
              patternComplete = false;
            }
          }
        }

        if (patternComplete == true) {
          if (OnCompleteHandler != NULL) {
              OnCompleteHandler();
          }
        }
    }
};
#endif
