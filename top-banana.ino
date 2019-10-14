#define SERIAL_DEBUG 1

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "single_led.h"

enum stripPattern { STRIP_OFF, STRIP_ON, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE };
enum  direction { FORWARD, REVERSE };

class PatternConfig
{
  public:
    stripPattern pattern;
    uint8_t interval;
    direction dir;
    uint32_t color1;
    uint32_t color2;
    uint16_t steps;

  PatternConfig(stripPattern pttn = STRIP_OFF, uint8_t intrvl = 0, direction d = FORWARD, uint32_t clr1 = 0, uint32_t clr2 = 0, uint16_t stps = 0)
  {
    pattern = pttn;
    interval = intrvl;
    dir = d;
    color1 = clr1;
    color2 = clr2;
    steps = stps;  
  }
};

// Initialise with config - start address, end address or pin.  Also animation type, interval and callback.
//Sequence is an array of animation types, interval and callbacks - type of sequence - series or parallel - when all callbacks complete, animation is complete?
class StripPattern
{

  public:
  Adafruit_NeoPixel* ledStrip;
  uint16_t stripStart;
  uint16_t stripLength;
  
  stripPattern ActivePattern;
  direction Direction;

  unsigned long Interval;   // milliseconds between updates
  unsigned long lastUpdate; // last update of position
  
  uint32_t Color1, Color2;  // What colours are in use
  uint16_t TotalSteps;  // total number of steps in the pattern
  uint16_t Index;  // current step within the pattern
  
  void (*OnComplete)();  // Callback on completion of pattern

  StripPattern(Adafruit_NeoPixel* strip, uint16_t start, uint16_t length, void (*callback)())
  {
    ledStrip = strip;
    stripStart = start;
    stripLength = length;
    OnComplete = callback;
  }

  void SetPattern(PatternConfig config)
  {
    switch(config.pattern)
      {
        case STRIP_ON:
            break;
        case STRIP_OFF:
            break;
        case RAINBOW_CYCLE:
            RainbowCycle(config);
            break;
        case THEATER_CHASE:
            TheaterChase(config);
            break;
        case COLOR_WIPE:
            ColorWipe(config);
            break;
        case SCANNER:
            Scanner(config);
            break;
        case FADE:
            Fade(config);
            break;
        default:
            break;
      }
    }

  // Update the pattern
  void Update()
  {
    if((millis() - lastUpdate) > Interval) // time to update
    {
      lastUpdate = millis();
      switch(ActivePattern)
      {
        case ON:
            break;
        case OFF:
            break;
        case RAINBOW_CYCLE:
            RainbowCycleUpdate();
            break;
        case THEATER_CHASE:
            TheaterChaseUpdate();
            break;
        case COLOR_WIPE:
            ColorWipeUpdate();
            break;
        case SCANNER:
            ScannerUpdate();
            break;
        case FADE:
            FadeUpdate();
            break;
        default:
            break;
      }
    }
  }

  // Increment the Index and reset at the end
    void Increment()
    {
        if (Direction == FORWARD)
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
    }
    
    // Reverse pattern direction
    void Reverse()
    {
        if (Direction == FORWARD)
        {
            Direction = REVERSE;
            Index = TotalSteps-1;
        }
        else
        {
            Direction = FORWARD;
            Index = 0;
        }
    }
    
    // Initialize for a RainbowCycle
    void RainbowCycle(PatternConfig config) //uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = RAINBOW_CYCLE;
        Interval = config.interval;
        TotalSteps = 255;
        Index = 0;
        Direction = config.dir;
    }
    
    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate()
    {
        for(int i=0; i< stripLength; i++)
        {
            ledStrip->setPixelColor(i+stripStart, Wheel(((i * 256 / stripLength) + Index) & 255));
        }
        Increment();
    }

    // Initialize for a Theater Chase
    void TheaterChase(PatternConfig config) //uint8_t interval, direction dir = FORWARD, uint32_t color1, uint32_t color2)
    {
        ActivePattern = THEATER_CHASE;
        Interval = config.interval;
        TotalSteps = stripLength;
        Color1 = config.color1;
        Color2 = config.color2;
        Index = 0;
        Direction = config.dir;
   }
    
    // Update the Theater Chase Pattern
    void TheaterChaseUpdate()
    {
        for(int i=0; i< stripLength; i++)
        {
            if ((i + Index) % 3 == 0)
            {
                ledStrip->setPixelColor(i+stripStart, Color1);
            }
            else
            {
                ledStrip->setPixelColor(i+stripStart, Color2);
            }
        }
        ledStrip->show();
        Increment();
        #ifdef SERIAL_DEBUG
          Serial.print("TheaterChaseUpdate ");
          Serial.print(Index);
          Serial.print(" ");
          Serial.print(Color1);
          Serial.print(" ");
          Serial.println(Color2);
        #endif
    }

    // Initialize for a ColorWipe
    void ColorWipe(PatternConfig config) //uint8_t interval, direction dir = FORWARD, uint32_t color)
    {
        ActivePattern = COLOR_WIPE;
        Interval = config.interval;
        TotalSteps = stripLength;
        Color1 = config.color1;
        Index = 0;
        Direction = config.dir;
    }
    
    // Update the Color Wipe Pattern
    void ColorWipeUpdate()
    {
        ledStrip->setPixelColor(Index+stripStart, Color1);
        Increment();
    }
    
    // Initialize for a SCANNNER
    void Scanner(PatternConfig config) //uint8_t interval, uint32_t color1)
    {
        ActivePattern = SCANNER;
        Interval = config.interval;
        TotalSteps = (stripLength - 1) * 2;
        Color1 = config.color1;
        Index = 0;
    }

    // Update the Scanner Pattern
    void ScannerUpdate()
    { 
        for (int i = 0; i < stripLength; i++)
        {
            if (i == Index)  // Scan Pixel to the right
            {
                 ledStrip->setPixelColor(i+stripStart, Color1);
            }
            else if (i == TotalSteps - Index) // Scan Pixel to the left
            {
                 ledStrip->setPixelColor(i+stripStart, Color1);
            }
            else // Fading tail
            {
                 ledStrip->setPixelColor(i+stripStart, DimColor(ledStrip->getPixelColor(i+stripStart)));
            }
        }
        Increment();
    }
    
    // Initialize for a Fade
    void Fade(PatternConfig config) //uint8_t interval, direction dir = FORWARD, uint32_t color1, uint32_t color2, uint16_t steps)
    {
        ActivePattern = FADE;
        Interval = config.interval;
        TotalSteps = config.steps;
        Color1 = config.color1;
        Color2 = config.color2;
        Index = 0;
        Direction = config.dir;
    }
    
    // Update the Fade Pattern
    void FadeUpdate()
    {
        // Calculate linear interpolation between Color1 and Color2
        // Optimise order of operations to minimize truncation error
        uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
        uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
        uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;
        
        ColorSet(ledStrip->Color(red, green, blue));
        Increment();
    }
   
    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
        // Shift R, G and B components one bit to the right
        uint32_t dimColor = ledStrip->Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
        return dimColor;
    }

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
        for (int i = 0; i < stripLength; i++)
        {
            ledStrip->setPixelColor(i+stripStart, color);
        }
    }

    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
        return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
        return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
        return color & 0xFF;
    }
    
    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85)
        {
            return ledStrip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            return ledStrip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        else
        {
            WheelPos -= 170;
            return ledStrip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
        }
    }
};


void Panel1Complete();
void Strip1Complete();
void Strip2Complete();

Single_LED Panel1(6, &Panel1Complete);

#define LED_PIN 4
#define LED_COUNT 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB );

//PatternConfig(stripPattern pttn = STRIP_OFF, uint8_t intrvl = 0, direction d = FORWARD, uint32_t clr1 = 0, uint32_t clr2 = 0, uint16_t stps = 0)
  
PatternConfig config(THEATER_CHASE, 100, FORWARD, strip.Color(255,255,0), strip.Color(0,0,50));
PatternConfig config2(FADE, 100, FORWARD, strip.Color(255,0,0), strip.Color(0,0,0), 30);
//uint8_t interval, direction dir = FORWARD, uint32_t color1, uint32_t color2, uint16_t steps)
//StripPattern(Adafruit_NeoPixel* strip, uint16_t start, uint16_t length, void (*callback)())
  
StripPattern pattern(&strip, 0, 10, &Strip1Complete);
StripPattern pattern2(&strip, 10, 20, &Strip2Complete);

void setup() {
  delay(1000);
  // put your setup code here, to run once:
  #ifdef SERIAL_DEBUG
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
  #endif
  strip.begin();
  Panel1.TurnOn(500);
//  colorSet(strip.Color(127,0,0), 5, 0, 150);
//  strip.show();
//  delay(100);
  pattern.SetPattern(config);
  pattern2.SetPattern(config2);
}

void loop() {
  // put your main code here, to run repeatedly:
  Panel1.Update();
  pattern.Update();
  pattern2.Update();
//  strip.show();
}

void colorSet(uint32_t c, uint8_t wait, int start, int numPixels) {
  for(uint16_t i=0; i<numPixels; i++) {
    strip.setPixelColor(i + start, c);
    strip.show();
    delay(wait);
  }
}

void Panel1Complete()
{
    #ifdef SERIAL_DEBUG
      Serial.print("Panel1Complete");
      Serial.println(Panel1.ledState);
    #endif
    Panel1.Toggle();
}

void Strip1Complete()
{
    #ifdef SERIAL_DEBUG
      Serial.println("Strip1Complete");
//      Serial.println(Panel1.ledState);
    #endif
    pattern.Color1 = pattern.Wheel(random(255));
    pattern.Reverse();
}
void Strip2Complete()
{
    #ifdef SERIAL_DEBUG
      Serial.println("Strip2Complete");
//      Serial.println(Panel1.ledState);
    #endif
    if(pattern2.Direction == FORWARD)
    {
      pattern2.Color1 = pattern.Wheel(random(255));
    }
    else
    {
      pattern2.Color2 = pattern.Wheel(random(255));
    }
    pattern2.Reverse();
}
