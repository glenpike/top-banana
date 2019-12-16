#ifdef __AVR__
#include <avr/power.h>
#endif
#include <Adafruit_NeoPixel.h>
#define SERIAL_DEBUG 1

#include "Interfaces.h"
#include "NeoPixel_Section.h"
#include "Single_LED.h"
#include "Sequence.h"

// SINGLE LED PANELS
#define PANEL_PINK_BALLOON 6
#define PANEL_ROSETTE 9
#define PANEL_SPEECH_BUBBLE 10
#define PANEL_3_BALLOONS 8
#define PANEL_TRAGIC_SIGN 7
#define PANEL_THOUGHT_BUBBLE 5

// STRIP NeoPixel sections - 1 LED each
#define PURPLE_PATCH 0
#define PINK_PATCH 1
#define YELLOW_BUTTON 2
#define PINK_BUTTON 3
#define BLUE_BUTTON 4
#define POW_BUBBLE_1 5
#define POW_BUBBLE_2 6

// STRIP NeoPixel sections
#define BANNER_START 7
#define BANNER_LENGTH 6
#define RIBBON_START (BANNER_START + BANNER_LENGTH)
#define RIBBON_LENGTH 12
#define BORDER_START (RIBBON_START + RIBBON_LENGTH)
#define BORDER_LEFT 28
#define BORDER_TOP 21
#define BORDER_RIGHT 28
#define BORDER_BOTTOM 20
#define BORDER_LENGTH (BORDER_LEFT + BORDER_TOP + BORDER_RIGHT + BORDER_BOTTOM)
#define BORDER_SPARE 8
#define HAMMER_START (BORDER_START + BORDER_LENGTH + BORDER_SPARE)
#define HAMMER_LENGTH 14
#define FACE_START (HAMMER_START + HAMMER_LENGTH)
#define FACE_LENGTH 14
#define TEXT_START (FACE_START + FACE_LENGTH)
#define TEXT_LENGTH 17
#define MONEY_START (TEXT_START + TEXT_LENGTH)
#define MONEY_LENGTH 2

#define NEOPIXEL_PIN 4
#define NEOPIXEL_COUNT (MONEY_START + MONEY_LENGTH)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB);

Single_LED panelPinkBalloon(PANEL_PINK_BALLOON, NULL);
Single_LED panelRosette(PANEL_ROSETTE, NULL);
Single_LED panelSpeechBubble(PANEL_SPEECH_BUBBLE, NULL);
Single_LED panel3Balloons(PANEL_3_BALLOONS, NULL);
Single_LED panelTragicSign(PANEL_TRAGIC_SIGN, NULL);
Single_LED panelThoughtBubble(PANEL_THOUGHT_BUBBLE, NULL);

NeoPixel_Section purplePatch(&strip, PURPLE_PATCH, 1, NULL);
NeoPixel_Section pinkPatch(&strip, PINK_PATCH, 1, NULL);
NeoPixel_Section yellowButton(&strip, YELLOW_BUTTON, 1, NULL);
NeoPixel_Section pinkButton(&strip, PINK_BUTTON, 1, NULL);
NeoPixel_Section blueButton(&strip, BLUE_BUTTON, 1, NULL);
NeoPixel_Section powBubble1(&strip, POW_BUBBLE_1, 1, NULL);
NeoPixel_Section powBubble2(&strip, POW_BUBBLE_2, 1, NULL);

NeoPixel_Section banner(&strip, BANNER_START, BANNER_LENGTH, NULL);
NeoPixel_Section ribbon(&strip, RIBBON_START, RIBBON_LENGTH, NULL);
NeoPixel_Section border(&strip, BORDER_START, BORDER_LENGTH, NULL);
NeoPixel_Section hammer(&strip, HAMMER_START, HAMMER_LENGTH, NULL);
NeoPixel_Section face(&strip, FACE_START, FACE_LENGTH, NULL);
NeoPixel_Section text(&strip, TEXT_START, TEXT_LENGTH, NULL);
NeoPixel_Section money(&strip, MONEY_START, MONEY_LENGTH, NULL);
NeoPixel_Section everything(&strip, 0, 100, NULL);

AbstractAnimateable* stripAndPanels[] = {
  &panelThoughtBubble,
  &panelTragicSign,
  &panel3Balloons,
  &panelSpeechBubble,
  &panelRosette,
  &panelPinkBalloon,  
  &everything 
};

AbstractAnimateable* original[] = {
  &banner, 
  &powBubble1, &powBubble2,
  &yellowButton, &pinkButton, &blueButton,
  &purplePatch, &pinkPatch,
  &panelThoughtBubble,
  &panelTragicSign,
  &ribbon,
  &panel3Balloons,
  &panelSpeechBubble,
  &panelRosette,
  &money,
  &panelPinkBalloon,
  &border,
  &hammer, &face, &text, 
};

AbstractAnimateable* outsideToCentre[] = {
  &border,
  &hammer, &face, &text, &money,
  &purplePatch, &pinkPatch,
  &yellowButton, &pinkButton, &blueButton, &powBubble1, &powBubble2,
  &banner, &ribbon,
  &panelPinkBalloon, &panelRosette, &panelSpeechBubble, &panel3Balloons, &panelTragicSign, &panelThoughtBubble
};

// #define NUM_ANIMATIONS 7
#define NUM_ANIMATIONS 20
AnimationConfig ledOn = { ON, 50, 1};
AnimationConfig animOff = { OFF, 10, 0};
AnimationConfig whiteWipe = { COLOR_WIPE, 5, NULL, FORWARD, strip.Color(128, 128, 128) };
AnimationConfig greenWipe = { COLOR_WIPE, 5, NULL, FORWARD, strip.Color(31, 255, 31) };
AnimationConfig blueWipe = { COLOR_WIPE, 5, NULL, FORWARD, strip.Color(0, 0, 255) };
AnimationConfig redWipe = { COLOR_WIPE, 50, NULL, FORWARD, strip.Color(255, 0, 0) };
AnimationConfig yellowWipe = { COLOR_WIPE, 5, NULL, REVERSE, strip.Color(255, 255, 0) };
AnimationConfig purpleWipe = { COLOR_WIPE, 5, NULL, FORWARD, strip.Color(255, 0, 192) };
AnimationConfig pinkWipe = { COLOR_WIPE, 5, NULL, FORWARD, strip.Color(255, 31, 192) };
AnimationConfig flashYellow = { FLASH, 50, 20, NULL, strip.Color(255, 255, 0) };
AnimationConfig flashBlue = { FLASH, 50, 20, NULL, strip.Color(0, 0, 255) };
AnimationConfig chase = { THEATER_CHASE, 20, NULL, FORWARD, strip.Color(31, 255, 31), strip.Color(255, 0, 192) };
AnimationConfig rainbow = { RAINBOW_CYCLE, 20, 20, FORWARD };

AnimationConfig* testOutside[] = {
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &chase,
  &ledOn,
  &ledOn,
  &ledOn,
  &ledOn,
  &ledOn,
  &ledOn
};

AnimationConfig* testAllOn[] = {
  &whiteWipe,
  &blueWipe,
  &redWipe,
  &yellowWipe,
  &purpleWipe,
  &pinkWipe,
  &pinkWipe,
  &yellowWipe,
  &ledOn,
  &ledOn,
  &pinkWipe,
  &ledOn,
  &ledOn,
  &ledOn,
  &blueWipe,
  &ledOn,
  &redWipe,
  &blueWipe,
  &yellowWipe,
  &blueWipe
};
AnimationConfig* testOriginal[] = {
  &yellowWipe,
  &blueWipe,
  &blueWipe,
  &yellowWipe,
  &pinkWipe,
  &blueWipe,
  &purpleWipe,
  &pinkWipe,
  &ledOn,
  &ledOn,
  &blueWipe,
  &ledOn,
  &ledOn,
  &ledOn,
  &whiteWipe,
  &ledOn,
  &greenWipe,
  &purpleWipe,
  &flashYellow,
  &yellowWipe
};

// Uses the 'stripAndPanels' config!
AnimationConfig* testRainbow[] = {
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &rainbow
};

AnimationConfig* testChase[] = {
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &chase
};

AnimationConfig* testAllOff[] = {
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,

  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff
};

void SequenceComplete();

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

Sequence seq(&SequenceComplete);

#define MAX_STATES 4
uint8_t currentState = 0;
uint8_t lastState = currentState;

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(9600); // We initialize serial connection so that we could
                        // print values from sensor.
    Serial.print(F("setup with "));
    Serial.print(NUM_ANIMATIONS);
    Serial.print(F(" animations "));
    Serial.print(NEOPIXEL_COUNT);
    Serial.print(F(" NeoPixels: freeMemory "));
    Serial.println(freeMemory());
#endif    
    strip.begin();
    strip.clear();
    strip.show();
    delay(1000);
    everything.ColorSet(strip.Color(255, 255, 0));
    strip.show();
    delay(1000);
    everything.ColorSet(strip.Color(0, 255, 255));
    strip.show();
    delay(1000);
    everything.ColorSet(strip.Color(255, 0, 255));
    strip.show();
    delay(1000);

    seq.SetAnimateables(original, NUM_ANIMATIONS);
    seq.SetAnimations(testOriginal, NUM_ANIMATIONS, false);
    seq.Start();
    strip.show();
}

void loop() {
    seq.Update();
    strip.show();
    // delay(10);
    if (lastState != currentState) {
      nextAnimation();
      lastState = currentState;
      // // everything.ColorSet(strip.Color(255, 0, 255));
      // // strip.show();
      // // delay(1000);
      // seq.Pause();
      // seq.Reset();
      // seq.SetAnimateables(stripAndPanels, 7);
      // seq.SetAnimations(testRainbow, 7, true);
      // seq.Start();
    }
}

void nextAnimation() {
  seq.Pause();
  seq.Reset();
  switch(currentState) {
    case 0:
      delay(1000);
      seq.SetAnimateables(original, NUM_ANIMATIONS);
      seq.SetAnimations(testOriginal, NUM_ANIMATIONS, false);
      break;
    case 1:
      delay(1000);
      seq.SetAnimateables(stripAndPanels, 7);
      seq.SetAnimations(testRainbow, 7, true);
      break;
    case 2:
      delay(1000);
      seq.SetAnimateables(stripAndPanels, 7);
      seq.SetAnimations(testChase, 7, false);
      break;
    case 3:
      delay(1000);
      seq.SetAnimateables(original, NUM_ANIMATIONS);
      seq.SetAnimations(testAllOff, NUM_ANIMATIONS, false);
      break;
  }
  seq.Start();
}

void SequenceComplete() {
#ifdef SERIAL_DEBUG
    Serial.print(F("SequenceComplete "));
    Serial.print(currentState);
    Serial.print(F(" freeMemory "));
    Serial.println(freeMemory());
#endif

  currentState++;
  if (currentState == MAX_STATES) {
    currentState = 0;
  }
  
}
