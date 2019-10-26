#ifndef SEQUENCE_H
#define SEQUENCE_H
#define SERIAL_DEBUG 1
/* has a config for a number of strips, their animations, colours, etc. and
 * their duration */
// Also has a strip and a callback.
// tracks the animations internally and calls the callback when they are all
// marked as complete.
// class SectionConfig {
//   public:
//     uint8_t start;
//     uint8_t length;
//     SectionConfig(uint8_t s, uint8_t l) {
//         start = s;
//         length = l;
//     }
// }
#include "NeoPixel_Section.h"


class Sequence : CallBackHandler {
  public:
    // Array of Sections
    NeoPixel_Section* sections;
    uint8_t numSections;

    uint8_t currentSection;

    void (*OnCompleteHandler)(); // Callback on completion of pattern

    // Array to monitor 'OnComplete' callbacks
    Sequence(void (*callback)()) {
        currentSection = 0;
        OnCompleteHandler = callback;
    }

    void SetSections(NeoPixel_Section* sects, uint8_t len) {
        numSections = len;
#ifdef SERIAL_DEBUG
        Serial.print("SetSections ");
        Serial.println(numSections);
#endif
        sections = sects;
        currentSection = 0;

        for (byte i = 0; i < numSections; i++) {
#ifdef SERIAL_DEBUG
            Serial.print("section ");
            Serial.print(i);
            Serial.print(" ");
            Serial.println(sections[i].stripStart);
#endif
            // Can't easily pass a pointer to a member function...
            sections[i].SetCallback(this);
        }
    }

    void SetPatterns(PatternConfig* configs, uint8_t len) {
        for (byte i = 0; i < numSections; i++) {
            if (i < len) {
                sections[i].SetPattern(configs[i]);
            }
        }
        currentSection = 0;
    }

    void Update() {
        // Parallel vs Series - do we want to update all animations?
        for (byte i = 0; i < numSections; i++) {
            sections[i].Update();
        }
        //        #ifdef SERIAL_DEBUG
        //            Serial.print("Update ");
        //            Serial.println(numSections);
        //#endif
    }

    void OnComplete(void *p) {
#ifdef SERIAL_DEBUG
        Serial.println("OnComplete");
#endif
        // for series, we keep track of the current animation.
         uint8_t patternComplete = 1;
         for (byte i = 0; i < numSections; i++) {
             if (sections[i].PatternCompleted == 0) {
                 patternComplete = 0;
             }
         }

         if (patternComplete == 1) {
           if (OnCompleteHandler != NULL) {
               OnCompleteHandler();
           }
         }
    }
};
#endif
