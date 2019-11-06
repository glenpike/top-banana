#ifdef __AVR__
#include <avr/power.h>
#endif
#include <Adafruit_NeoPixel.h>
#define SERIAL_DEBUG 1

#include "Interfaces.h"
#include "NeoPixel_Section.h"
#include "Single_LED.h"
#include "Sequence.h"

void SequenceComplete();

#define NEOPIXEL_PIN 4
#define NEOPIXEL_COUNT 120
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB);

// SINGLE LED PANELS
#define PANEL_PINK_BALLOON 3
#define PANEL_ROSETTE 10
#define PANEL_SPEECH_BUBBLE 11
#define PANEL_3_BALLOONS 9
#define PANEL_TRAGIC_SIGN 5
#define PANEL_THOUGHT_BUBBLE 2

// TODO - rename to match panels.
Single_LED panelPinkBalloon(PANEL_PINK_BALLOON, NULL);
Single_LED panelRosette(PANEL_ROSETTE, NULL);
Single_LED panelSpeechBubble(PANEL_SPEECH_BUBBLE, NULL);
Single_LED panel3Balloons(PANEL_3_BALLOONS, NULL);
Single_LED panelTragicSign(PANEL_TRAGIC_SIGN, NULL);
Single_LED panelThoughtBubble(PANEL_THOUGHT_BUBBLE, NULL);

NeoPixel_Section purplePatch(&strip, 0, 1, NULL);
NeoPixel_Section pinkPatch(&strip, 1, 1, NULL);
NeoPixel_Section yellowButton(&strip, 2, 1, NULL);
NeoPixel_Section pinkButton(&strip, 3, 1, NULL);
NeoPixel_Section blueButton(&strip, 4, 1, NULL);
NeoPixel_Section powBubble1(&strip, 5, 1, NULL);
NeoPixel_Section powBubble2(&strip, 6, 1, NULL);
NeoPixel_Section banner(&strip, 7, 6, NULL);
NeoPixel_Section ribbon(&strip, 13, 12, NULL);
NeoPixel_Section border(&strip, 25, 40, NULL);
NeoPixel_Section hammer(&strip, 65, 10, NULL);
NeoPixel_Section face(&strip, 75, 10, NULL);
NeoPixel_Section text(&strip, 85, 10, NULL);
NeoPixel_Section money(&strip, 95, 8, NULL);


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

#define NUM_ANIMATIONS 20
AnimationConfig ledOn = { ON, 50, 1};
AnimationConfig animOff = { OFF, 10, 0};
AnimationConfig whiteWipe = { COLOR_WIPE, 5, NULL, FORWARD, strip.Color(128, 128, 128) };
AnimationConfig greenWipe = { COLOR_WIPE, 5, NULL, FORWARD, strip.Color(31, 255, 31) };
AnimationConfig blueWipe = { COLOR_WIPE, 20, NULL, FORWARD, strip.Color(0, 0, 255) };
AnimationConfig redWipe = { COLOR_WIPE, 50, NULL, FORWARD, strip.Color(255, 0, 0) };
AnimationConfig yellowWipe = { COLOR_WIPE, 20, NULL, FORWARD, strip.Color(255, 255, 0) };
AnimationConfig purpleWipe = { COLOR_WIPE, 20, NULL, FORWARD, strip.Color(255, 0, 192) };
AnimationConfig pinkWipe = { COLOR_WIPE, 20, NULL, FORWARD, strip.Color(255, 31, 192) };
AnimationConfig flashYellow = { FLASH, 50, 20, NULL, strip.Color(255, 255, 0) };
AnimationConfig chase = { THEATER_CHASE, 20, NULL, FORWARD, strip.Color(31, 255, 31), strip.Color(255, 0, 192) };
AnimationConfig rainbow = { RAINBOW_CYCLE, 20, NULL, FORWARD };

AnimationConfig* testAllOn[] = {
  &whiteWipe,
  &blueWipe,
  &redWipe,
  &yellowWipe,
  &purpleWipe,
  &pinkWipe,
  &pinkWipe,
  &yellowWipe,
  &pinkWipe,
  &blueWipe,
  &redWipe,
  &blueWipe,
  &yellowWipe,
  &blueWipe,
  
  &ledOn,
  &ledOn,
  &ledOn,
  &ledOn,
  &ledOn,
  &ledOn
};
/*
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
*/
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

AnimationConfig* testRainbow[] = {
  &rainbow,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &rainbow,
  &animOff,
  &animOff,
  &animOff,
  &animOff,

  &animOff,
  &animOff,
  &rainbow,
  &rainbow,
  &rainbow,
  &rainbow
};

AnimationConfig* testChase[] = {
  &chase,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &animOff,
  &chase,
  &animOff,
  &animOff,
  &animOff,
  &animOff,

  &animOff,
  &animOff,
  &chase,
  &chase,
  &chase,
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

Sequence seq(&SequenceComplete);

#define MAX_STATES 4
uint8_t currentState = 0;

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(9600); // We initialize serial connection so that we could
                        // print values from sensor.
    Serial.println(F("setup "));
#endif    
    strip.begin();
    strip.clear();
    strip.show();

    seq.SetAnimateables(original, NUM_ANIMATIONS);
    seq.SetAnimations(testOriginal, NUM_ANIMATIONS, false);
    seq.Start();
    strip.show();
}

void loop() {
    seq.Update();
    strip.show();
    // delay(1000);
    // left.ColorSet(strip.Color(255, 255, 0));
    // strip.show();
    // delay(1000);
    // left.ColorSet(strip.Color(0, 255, 255));
    // strip.show();
    // delay(1000);
    // left.ColorSet(strip.Color(255, 0, 255));
    // strip.show();
    // delay(1000);

}

void SequenceComplete() {
#ifdef SERIAL_DEBUG
    Serial.println(F("SequenceComplete"));
#endif
  seq.Pause();
  seq.Reset();

  currentState++;
  if (currentState == MAX_STATES) {
    currentState = 0;
  }
  switch(currentState) {
    case 0:
      delay(1000);
      seq.SetAnimations(testOriginal, NUM_ANIMATIONS, false);
      break;
    case 1:
      delay(1000);
      seq.SetAnimations(testAllOff, NUM_ANIMATIONS, false);
      break;
    case 2:
      delay(1000);
      seq.SetAnimations(testChase, NUM_ANIMATIONS, false);
      break;
    case 3:
      delay(1000);
      seq.SetAnimations(testAllOff, NUM_ANIMATIONS, false);
      break;
  }
  seq.Start();
}
