#define SERIAL_DEBUG 1

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "Interfaces.h"
#include "NeoPixel_Section.h"
#include "Single_LED.h"
#include "Sequence.h"

void SequenceComplete();

#define NEOPIXEL_PIN 4
#define NEOPIXEL_COUNT 103
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB);

// SINGLE LED PANELS
#define PANEL_PINK_BALLOON 3
#define PANEL_ROSETTE 10
#define PANEL_SPEECH_BUBBLE 11
#define PANEL_3_BALLOONS 9
#define PANEL_TRAGIC_SIGN 5
#define PANEL_THOUGHT_BUBBLE 2

// TODO - rename to match panels.
Single_LED led1(PANEL_PINK_BALLOON, NULL);
Single_LED led2(PANEL_ROSETTE, NULL);
Single_LED led3(PANEL_SPEECH_BUBBLE, NULL);
Single_LED led4(PANEL_3_BALLOONS, NULL);
Single_LED led5(PANEL_TRAGIC_SIGN, NULL);
Single_LED led6(PANEL_THOUGHT_BUBBLE, NULL);

NeoPixel_Section purplePatch(&strip, 0, 1, NULL);
NeoPixel_Section pinkPatch(&strip, 1, 1, NULL);
NeoPixel_Section yellowButton(&strip, 2, 1, NULL);
NeoPixel_Section pinkButton(&strip, 3, 1, NULL);
NeoPixel_Section blueButton(&strip, 4, 1, NULL);
NeoPixel_Section powBubble1(&strip, 5, 1, NULL);
NeoPixel_Section powBubble2(&strip, 6, 1, NULL);
NeoPixel_Section banner(&strip, 7, 6, NULL);
NeoPixel_Section ribbon(&strip, 13, 12, NULL);
// TODO - make these match actual counts from here
NeoPixel_Section left(&strip, 25, 10, NULL);
NeoPixel_Section top(&strip, 35, 10, NULL);
NeoPixel_Section right(&strip, 45, 10, NULL);
NeoPixel_Section bottom(&strip, 55, 10, NULL);
NeoPixel_Section hammer(&strip, 65, 10, NULL);
NeoPixel_Section face(&strip, 75, 10, NULL);
NeoPixel_Section text(&strip, 85, 10, NULL);
NeoPixel_Section money(&strip, 95, 8, NULL);


AbstractAnimateable* outsideToCentre[] = {
  &left, &top, &right, &bottom,
  // &hammer, &face, &text, &money,
  // &purplePatch, &pinkPatch,
  // &yellowButton, &pinkButton, &blueButton, &powBubble1, &powBubble2,
  // &banner, &ribbon,
  &led1, &led2, &led3, &led4, &led5, &led6
};

AnimationConfig testAllOn[] = {
  { SCANNER, 100, NULL, NULL, strip.Color(255, 0, 0) },
  { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(0, 255, 0) },
  { SCANNER, 100, NULL, NULL, strip.Color(0, 0, 255) },
  { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(255, 255, 0) },
  // { THEATER_CHASE, 100, NULL, FORWARD, strip.Color(255, 0, 255), strip.Color(0, 0, 255) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(255, 255, 0) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(255, 128, 0) },
  // { RAINBOW_CYCLE, 100, NULL, FORWARD },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(255, 31, 192) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(255, 64, 255) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(255, 255, 0) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(255, 64, 255) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(31, 31, 255) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(31, 31, 255) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(31, 31, 255) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(255, 255, 9) },
  // { COLOR_WIPE, 100, NULL, FORWARD, strip.Color(0, 0, 255) },
  { ON, 10, 1},
  { ON, 10, 1},
  { ON, 10, 1},
  { ON, 10, 1},
  { ON, 10, 1},
  { ON, 10, 1}
};

AnimationConfig testAllOff[] = {
  { OFF, 100 },
  { OFF, 100 },
  { OFF, 100 },
  { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  // { OFF, 100 },
  { OFF, 10, 1},
  { OFF, 10, 1},
  { OFF, 10, 1},
  { OFF, 10, 1},
  { OFF, 10, 1},
  { OFF, 10, 1}
};

Sequence seq(&SequenceComplete);

bool onAnim = true;

uint8_t counter = 0;

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(9600); // We initialize serial connection so that we could
                        // print values from sensor.
    Serial.println(F("setup "));
#endif    
    strip.begin();
    strip.clear();
    strip.show();

    delay(1000);
    seq.SetAnimateables(outsideToCentre, 10);
    seq.SetAnimations(testAllOn, 10, false);
    seq.Start();
    strip.show();
}

void loop() {
     seq.Update();
     strip.show();
}

void SequenceComplete() {
#ifdef SERIAL_DEBUG
    Serial.println(F("SequenceComplete"));
#endif
  seq.Pause();
  seq.Reset();
    
  if(onAnim) {
    seq.SetAnimations(testAllOff, 10, false);
    onAnim = false;
  } else {
    seq.SetAnimations(testAllOn, 10, false);
    onAnim = true;
  }
  seq.Start();
}
