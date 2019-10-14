
#include "single_led.h"

Single_LED::Single_LED(int pin, void (*callback)())
{
  ledPin = pin;
  pinMode(ledPin, OUTPUT);  
  ledState = LOW;
  OnComplete = callback;
  Interval = 0;
}

void Single_LED::Toggle()
{
  if(ledState == HIGH)
  {
    TurnOff(Interval);
  }
  else
  {
    TurnOn(Interval);
  }
}

void Single_LED::TurnOn(unsigned long interval)
{
  if(ledState == LOW)
  {
    ledState = HIGH;
  }
  Show(interval);
}

void Single_LED::TurnOff(unsigned long interval)
{
  if(ledState == HIGH)
  {
    ledState = LOW;
  }
  Show(interval);
}

void Single_LED::Update()
{
  if((millis() - lastShow) > Interval) // time to update
  {
    if (OnComplete != NULL)
    {
        OnComplete(); // call the comlpetion callback
    }
  }
}

void Single_LED::Show(unsigned long interval)
{
  Interval = interval;
  lastShow = millis();
  digitalWrite(ledPin, ledState);
  #ifdef SERIAL_DEBUG
    Serial.print("LED.Show: ");
    Serial.print(ledPin);
    Serial.print(" = ");
    Serial.print(ledState);
    Serial.print(" for ");
    Serial.print(interval);
    Serial.print(" current ");
    Serial.println(lastShow);
  #endif
}
