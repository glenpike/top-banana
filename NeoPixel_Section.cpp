#include "NeoPixel_Section.h"
#define SERIAL_DEBUG 1
NeoPixel_Section::NeoPixel_Section(Adafruit_NeoPixel* strip, uint16_t start,
                                   uint16_t length, CallBackHandler *handler = NULL) {
    ledStrip = strip;
    stripStart = start;
    stripLength = length;
    pCallbackHandler = handler;
}

void NeoPixel_Section::SetPattern(AnimationConfig *config) {
    pAnimationConfig = config;
    currentStep = 0;
    animationComplete = false;
    switch (pAnimationConfig->animation) {
    case ON:
        // uint8_t updateInterval, uint32_t color1
        break;
    case OFF:
        // uint8_t updateInterval
        pAnimationConfig->color1 = ledStrip->Color(0, 0, 0);
        break;
    case RAINBOW_CYCLE:
        // uint8_t updateInterval, Direction dir = FORWARD)
        if(!pAnimationConfig->steps) {
          pAnimationConfig->steps = stripLength;
        }
        break;
    case THEATER_CHASE:
        // uint8_t updateInterval, Direction dir = FORWARD, uint32_t color1, uint32_t color2)
        if(!pAnimationConfig->steps) {
          pAnimationConfig->steps = stripLength;
        }
        break;
    case COLOR_WIPE:
        // uint8_t updateInterval, Direction dir = FORWARD, uint32_t color)
        pAnimationConfig->steps = stripLength;
        if(pAnimationConfig->direction == REVERSE) {
          currentStep = stripLength;
        }
        break;
    case SCANNER:
        // uint8_t updateInterval, uint32_t color1)
        pAnimationConfig->steps = (stripLength - 1) * 2;
        break;
    case FADE:
        // uint8_t updateInterval, Direction dir = FORWARD, uint32_t color1, uint32_t
        // color2, uint16_t steps)
        break;
    default:
        break;
    }
#ifdef SERIAL_DEBUG
    Serial.print("NeoPixel_Section::SetPattern @ ");
    Serial.print(stripStart);
    Serial.print(" activeAnimation is ");
    Serial.print(pAnimationConfig->animation);
    Serial.print(" totalSteps is ");
    Serial.print(pAnimationConfig->steps);
    Serial.print(" updateInterval ");
    Serial.println(pAnimationConfig->updateInterval);
#endif
}

void NeoPixel_Section::Start() {
  running = true;
#ifdef SERIAL_DEBUG
  Serial.print(F("starting section @ "));
  Serial.println(stripStart);
#endif
}
void NeoPixel_Section::Pause() {
    running = false;
#ifdef SERIAL_DEBUG
    Serial.print("NeoPixel_Section::Pause @ ");
    Serial.println(stripStart);
#endif
}
void NeoPixel_Section::Reset() {
    currentStep = 0;
    animationComplete = false;
#ifdef SERIAL_DEBUG
    Serial.print("NeoPixel_Section::Reset @ ");
    Serial.println(stripStart);
#endif
}

void NeoPixel_Section::Update() {
    if (running && (millis() - lastUpdate) > pAnimationConfig->updateInterval) // time to update
    {
        lastUpdate = millis();
#ifdef SERIAL_DEBUG
        Serial.print("NeoPixel_Section::Update section @ ");
        Serial.print(stripStart);
        Serial.print(" activeAnimation is ");
        Serial.print(pAnimationConfig->animation);
        Serial.print(" currentStep is ");
        Serial.print(currentStep);
        Serial.print(" time now ");
        Serial.println(lastUpdate);
#endif
        switch (pAnimationConfig->animation) {
        case ON:
            StripAllUpdate();
            break;
        case OFF:
            StripAllUpdate();
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
        case FLASH:
            FlashUpdate();
            break;
        default:
            break;
        }
        ledStrip->show();
    }
}

void NeoPixel_Section::Increment() {
    // Serial.print("NeoPixel_Section::Increment @ ");
    // Serial.println(currentStep);
    
    if (pAnimationConfig->direction == FORWARD) {
        currentStep++;
        if (currentStep >= pAnimationConfig->steps) {
            currentStep = 0;
            if (animationComplete == false) {
                animationComplete = true;
                if (pCallbackHandler != NULL) {
                    pCallbackHandler->OnComplete(static_cast<void*>(this));
                }
            }
        }
    } else {
      // BUGS IN HERE IF WE START IN REVERSE
        --currentStep;
        if (currentStep < 0) {
            currentStep = pAnimationConfig->steps - 1;
            if (animationComplete == false) {
                animationComplete = true;
                if (pCallbackHandler != NULL) {
                    pCallbackHandler->OnComplete(static_cast<void*>(this));
                }
            }
        }
    }
    // Serial.print("NeoPixel_Section::Increment now ");
    // Serial.println(currentStep);
}

void NeoPixel_Section::Reverse() {
    if (pAnimationConfig->direction == FORWARD) {
        pAnimationConfig->direction = REVERSE;
        currentStep = pAnimationConfig->steps - 1;
    } else {
        pAnimationConfig->direction = FORWARD;
        currentStep = 0;
    }
    animationComplete == false;
}


uint32_t NeoPixel_Section::DimColor(uint32_t color) {
    // Shift R, G and B components one bit to the right
    uint32_t dimColor =
        ledStrip->Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
}

void NeoPixel_Section::ColorSet(uint32_t color) {
    for (int i = 0; i < stripLength; i++) {
        ledStrip->setPixelColor(i + stripStart, color);
    }
// #ifdef SERIAL_DEBUG
//     Serial.print("NeoPixel_Section::ColorSet @ ");
//     Serial.print(stripStart);
//     Serial.print(" ledStrip is ");
//     Serial.print(ledStrip->numPixels());
//     Serial.print(" stripLength is ");
//     Serial.print(stripLength);
//     Serial.print(" color ");
//     Serial.println(color);
// #endif
    ledStrip->show();
}

uint8_t NeoPixel_Section::Red(uint32_t color) { return (color >> 16) & 0xFF; }
uint8_t NeoPixel_Section::Green(uint32_t color) { return (color >> 8) & 0xFF; }
uint8_t NeoPixel_Section::Blue(uint32_t color) { return color & 0xFF; }
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t NeoPixel_Section::Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return ledStrip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return ledStrip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
    } else {
        WheelPos -= 170;
        return ledStrip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}
void NeoPixel_Section::StripAllUpdate() {
    if (currentStep == 0) {
        ledStrip->fill(pAnimationConfig->color1, stripStart, stripLength);
    }
    Increment();
}

void NeoPixel_Section::RainbowCycleUpdate() {
    int offset = (int)(((float)currentStep / pAnimationConfig->steps) * 255);
    for (int i = 0; i < stripLength; i++) {
        ledStrip->setPixelColor(i + stripStart,
                                Wheel(((i * 256 / stripLength) + offset) & 255));
    }
    Increment();
}
void NeoPixel_Section::TheaterChaseUpdate() {
    for (int i = 0; i < stripLength; i++) {
        if ((i + currentStep) % 3 == 0) {
            ledStrip->setPixelColor(i + stripStart, pAnimationConfig->color1);
        } else {
            ledStrip->setPixelColor(i + stripStart, pAnimationConfig->color2);
        }
    }
    Increment();
#ifdef SERIAL_DEBUG
    Serial.print(F("TheaterChaseUpdate "));
    Serial.print(currentStep);
    Serial.print(F(" "));
    Serial.print(pAnimationConfig->color1);
    Serial.print(F(" "));
    Serial.println(pAnimationConfig->color2);
#endif
}

void NeoPixel_Section::ColorWipeUpdate() {
    ledStrip->setPixelColor(currentStep + stripStart, pAnimationConfig->color1);
    Increment();
}

void NeoPixel_Section::ScannerUpdate() {
    for (int i = 0; i < stripLength; i++) {
        if (i == currentStep) // Scan Pixel to the right
        {
            ledStrip->setPixelColor(i + stripStart, pAnimationConfig->color1);
        } else if (i == pAnimationConfig->steps - currentStep) // Scan Pixel to the left
        {
            ledStrip->setPixelColor(i + stripStart, pAnimationConfig->color1);
        } else // Fading tail
        {
            ledStrip->setPixelColor(
                i + stripStart,
                DimColor(ledStrip->getPixelColor(i + stripStart)));
        }
    }
    Increment();
}

void NeoPixel_Section::FadeUpdate() {
    // Calculate linear interpolation between color1 and color2
    // Optimise order of operations to minimize truncation error
    uint8_t red =
        ((Red(pAnimationConfig->color1) * (pAnimationConfig->steps - currentStep)) + (Red(pAnimationConfig->color2) * currentStep)) /
        pAnimationConfig->steps;
    uint8_t green =
        ((Green(pAnimationConfig->color1) * (pAnimationConfig->steps - currentStep)) + (Green(pAnimationConfig->color2) * currentStep)) /
        pAnimationConfig->steps;
    uint8_t blue =
        ((Blue(pAnimationConfig->color1) * (pAnimationConfig->steps - currentStep)) + (Blue(pAnimationConfig->color2) * currentStep)) /
        pAnimationConfig->steps;

    ColorSet(ledStrip->Color(red, green, blue));
    //    ledStrip->show();
    Increment();
#ifdef SERIAL_DEBUG
    Serial.print(F("FadeUpdate "));
    Serial.print(currentStep);
    Serial.print(F(" "));
    Serial.print(red);
    Serial.print(F(" "));
    Serial.print(green);
    Serial.print(F(" "));
    Serial.println(blue);
#endif
}

void NeoPixel_Section::FlashUpdate() {
    if (currentStep % 2 == 0) {
        ColorSet(pAnimationConfig->color1);
    } else {
        ColorSet(pAnimationConfig->color2);
    }
    Increment();
}
