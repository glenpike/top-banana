#ifndef SEQUENCE_H
#define SEQUENCE_H
#define SERIAL_DEBUG 1

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
    void Reset() { currentSection = 0; }

    void SetAnimateables(AbstractAnimateable* sects[], uint8_t len) {
        numSections = len;
#ifdef SERIAL_DEBUG
        Serial.print("SetAnimateables ");
        Serial.println(numSections);
#endif
        sections = sects;
        currentSection = 0;

        for (byte i = 0; i < numSections; i++) {
            sections[i]->SetCallback(this);
        }
    }

    void SetAnimations(AnimationConfig* configs, uint8_t len, bool parallel = true) {
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
            if(!sections[currentSection]->isRunning()) {
              sections[currentSection]->Start();
            }
            sections[currentSection]->Update();
          }
        }
    }

    void OnComplete(void *p) { 
        bool patternComplete = true;
        if(isParallel) {
          for (byte i = 0; i < numSections; i++) {
#ifdef SERIAL_DEBUG
            if (sections[i] == p) {
              Serial.print("section completed: ");
              Serial.print(i);    
            }
            Serial.print(" section ");
            Serial.print(i);
            Serial.print(" pattern complete? ");
            Serial.println(sections[i]->isAnimationComplete());
#endif
            if (sections[i]->isAnimationComplete() == false) {
                 patternComplete = false;
            }
          }
        } else {
          if(p != sections[currentSection]) {
            Serial.print("not section we wanted! ");
          } else {
            Serial.print("current section complete ");   
            Serial.println(currentSection);
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
