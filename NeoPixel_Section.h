#ifndef NEOPIXEL_SECTION_H
#define NEOPIXEL_SECTION_H
#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

enum sectionPattern {
    STRIP_OFF,
    STRIP_ON,
    RAINBOW_CYCLE,
    THEATER_CHASE,
    COLOR_WIPE,
    SCANNER,
    FADE
};
enum direction { FORWARD, REVERSE };

class PatternConfig {
  public:
    sectionPattern pattern;
    uint32_t interval;
    direction dir;
    uint32_t color1;
    uint32_t color2;
    uint16_t steps;

    PatternConfig(sectionPattern pttn = STRIP_OFF, uint32_t intrvl = 10,
                  direction d = FORWARD, uint32_t clr1 = 0, uint32_t clr2 = 0,
                  uint16_t stps = 0) {
        pattern = pttn;
        interval = intrvl;
        dir = d;
        color1 = clr1;
        color2 = clr2;
        steps = stps;
    }
};

struct CallBackHandler {
    virtual void OnComplete(void *p);
};

class NeoPixel_Section {

  public:
    Adafruit_NeoPixel* ledStrip;
    uint16_t stripStart;
    uint16_t stripLength;

    sectionPattern ActivePattern;
    direction Direction;

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t Color1, Color2; // What colours are in use
    uint16_t TotalSteps;     // total number of steps in the pattern
    uint16_t Index;          // current step within the pattern

    CallBackHandler *m_pCallbackHandler;

    uint8_t PatternCompleted = 0;
    
    NeoPixel_Section(Adafruit_NeoPixel* strip, uint16_t start, uint16_t length,
                     CallBackHandler *handler);
                     
    void SetCallback(CallBackHandler *handler = NULL) { m_pCallbackHandler = handler; }
    void SetPattern(PatternConfig config);
    void Update();
    // Consider 'single-shot' animation or continuous?
    void Increment();
    void Reverse();

    // uint8_t interval, uint32_t color1
    void StripOn(PatternConfig config);
    // uint8_t interval
    void StripOff(PatternConfig config);
    // uint8_t interval, direction dir = FORWARD)
    void RainbowCycle(PatternConfig config);
    // uint8_t interval, direction dir = FORWARD, uint32_t color1, uint32_t
    // color2)
    void TheaterChase(PatternConfig config);
    // uint8_t interval, direction dir = FORWARD, uint32_t color)
    void ColorWipe(PatternConfig config);
    // uint8_t interval, uint32_t color1)
    void Scanner(PatternConfig config);
    // uint8_t interval, direction dir = FORWARD, uint32_t color1, uint32_t
    // color2, uint16_t steps)
    void Fade(PatternConfig config);

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
    uint32_t Wheel(byte WheelPos);

  protected:
    void StripAllUpdate();

    void RainbowCycleUpdate();
    void TheaterChaseUpdate();

    void ColorWipeUpdate();

    void ScannerUpdate();

    void FadeUpdate();
};
#endif
