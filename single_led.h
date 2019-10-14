#ifndef LED_PANEL_H
#define LED_PANELL_H
#include <Arduino.h>

enum ledPattern { OFF, ON, FLASH };
class Single_LED
{
  // Class Member Variables
  protected:
  // These are initialized at startup
  int ledPin;      // the number of the LED pin
  ledPattern  ActivePattern;  // which pattern is running
  unsigned long Interval;   // milliseconds until it calls OnComplete
  unsigned long lastShow; // last time we called Show
  
  void (*OnComplete)();  // Callback when the interval expires
  
  public:
  // These maintain the current state
  int ledState; // ledState used to set the LED

  Single_LED(int pin, void (*callback)());
  
  void Toggle();

  void TurnOn(unsigned long interval);
  void TurnOff(unsigned long interval);
  void Update();
  void Show(unsigned long interval);
};
#endif
