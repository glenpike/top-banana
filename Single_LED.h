#ifndef SINGLE_LED_H
#define SINGLE_LED_H
#include <Arduino.h>

enum ledPattern { OFF, ON, FLASH };
class Single_LED {
  protected:
    int ledPin;               // the number of the LED pin
    ledPattern ActivePattern; // which pattern is running
    unsigned long Interval;   // milliseconds until it calls OnComplete
    unsigned long lastShow;   // last time we called Show

    void (*OnComplete)(); // Callback when the interval expires

  public:
    int ledState; // ledState used to set the LED

    Single_LED(int pin, void (*callback)());

    void Toggle();

    void TurnOn(unsigned long interval);
    void TurnOff(unsigned long interval);
    void Update();
    void Show(unsigned long interval);
};
#endif
