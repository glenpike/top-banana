#define SERIAL_DEBUG 1

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "NeoPixel_Section.h"
#include "Single_LED.h"
#include "Sequence.h"
// Initialise with config - start address, end address or pin.  Also animation
// type, interval and callback.
// Sequence is an array of animation types, interval and callbacks - type of
// sequence - series or parallel - when all callbacks complete, animation is
// complete?

void Panel1Complete();
void Strip1Complete();
void Strip2Complete();
void Strip3Complete();

Single_LED Panel1(6, &Panel1Complete);

#define LED_PIN 4
#define LED_COUNT 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB);

// PatternConfig config(THEATER_CHASE, 100, FORWARD, strip.Color(255, 255, 0),
// strip.Color(0, 0, 50));
// PatternConfig config(SCANNER, 100, strip.Color(255, 255, 0));
// PatternConfig config2(RAINBOW_CYCLE, 100, FORWARD);
// PatternConfig config3(THEATER_CHASE, 100, FORWARD, strip.Color(255, 255, 0);

PatternConfig configs[] = {
  PatternConfig(SCANNER, 100, strip.Color(255, 255, 0)),
  PatternConfig(RAINBOW_CYCLE, 100, FORWARD),
  PatternConfig(THEATER_CHASE, 100, FORWARD, strip.Color(255, 255, 0))
};
PatternConfig configs2[] = {
  PatternConfig(STRIP_ON, 100, strip.Color(255, 0, 0)),
  PatternConfig(STRIP_ON, 100, strip.Color(0, 255, 0)),
  PatternConfig(STRIP_ON, 100, strip.Color(0, 0, 255))
};
// PatternConfig config3(STRIP_ON, 100, strip.Color(0, 255, 0));
// PatternConfig config4(STRIP_OFF, 100);

NeoPixel_Section sections[] = {
  NeoPixel_Section(&strip, 0, 10, NULL),
  NeoPixel_Section(&strip, 10, 20, NULL),
  NeoPixel_Section(&strip, 30, 20, NULL)
};

Sequence seq(&Strip1Complete);
// NeoPixel_Section pattern(&strip, 0, 10, &Strip1Complete);
// NeoPixel_Section pattern2(&strip, 10, 20, &Strip2Complete);
// NeoPixel_Section pattern3(&strip, 30, 20, &Strip3Complete);
uint8_t counter = 0;

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(9600); // We initialize serial connection so that we could
                        // print values from sensor.
    Serial.println("setup ");
#endif    
    strip.begin();
    strip.clear();
    strip.show();

    delay(1000);
// put your setup code here, to run once:


    Panel1.TurnOn(500);
     seq.SetSections(sections, 3);
     seq.SetPatterns(configs, 3);
    // pattern.SetPattern(config);
    // pattern2.SetPattern(config2);
    // pattern3.SetPattern(config3);
    strip.show();
}

void loop() {
    // put your main code here, to run repeatedly:
    Panel1.Update();
    // pattern.Update();
    // pattern2.Update();
    // pattern3.Update();
     seq.Update();
}

void Panel1Complete() {
    //#ifdef SERIAL_DEBUG
    //  Serial.print("Panel1Complete");
    //  Serial.println(Panel1.ledState);
    //#endif
    Panel1.Toggle();
}

void Strip1Complete() {
#ifdef SERIAL_DEBUG
    Serial.println("Strip1Complete");
    //      Serial.println(Panel1.ledState);
#endif
//  counter++;
//  if(counter % 2 == 0) {
//    seq.SetPatterns(configs, 3);
//  } else {
//    seq.SetPatterns(configs2, 3);
//  }
    //   pattern.Color1 = pattern.Wheel(random(255));
    // pattern.Reverse();
}
//void Strip2Complete() {
//    #ifdef SERIAL_DEBUG
//      Serial.println("Strip2Complete");
//    #endif
//    if (pattern.Direction == FORWARD) {
//        pattern.Color1 = pattern.Wheel(random(255));
//    } else {
//        pattern.Color2 = pattern.Wheel(random(255));
//    }
//    pattern.Reverse();
//}
//
//void Strip3Complete() {
//#ifdef SERIAL_DEBUG
//    Serial.println("Strip3Complete");
//    //      Serial.println(Panel1.ledState);
//#endif
//    if (pattern3.ActivePattern == STRIP_ON) {
//        pattern3.SetPattern(config4);
//    } else {
//        pattern3.SetPattern(config3);
//        pattern3.Color1 = pattern.Wheel(random(255));
//    }
//}
