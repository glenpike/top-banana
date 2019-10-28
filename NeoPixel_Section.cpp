#include "NeoPixel_Section.h"
//#define SERIAL_DEBUG 1
NeoPixel_Section::NeoPixel_Section(Adafruit_NeoPixel* strip, uint16_t start,
                                   uint16_t length, CallBackHandler *handler = NULL) {
    ledStrip = strip;
    stripStart = start;
    stripLength = length;
    pCallbackHandler = handler;
}

void NeoPixel_Section::SetPattern(AnimationConfig config) {
    activeAnimation = config.animation;
    updateInterval = config.updateInterval;
    currentStep = 0;
    animationComplete = false;
    switch (activeAnimation) {
    case ON:
        StripOn(config);
        break;
    case OFF:
        StripOff(config);
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

void NeoPixel_Section::Start() { 
  running = true;
  Serial.print("starting section @ ");
  Serial.println(stripStart);
}
void NeoPixel_Section::Pause() { running = false; }
void NeoPixel_Section::Reset() { currentStep = 0; }

void NeoPixel_Section::Update() {
    if (running && (millis() - lastUpdate) > updateInterval) // time to update
    {
        lastUpdate = millis();
        switch (activeAnimation) {
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
        default:
            break;
        }
        ledStrip->show();
    }
}

void NeoPixel_Section::Increment() {
    if (direction == FORWARD) {
        currentStep++;
        if (currentStep >= totalSteps) {
            currentStep = 0;
            if (animationComplete == false) {
                animationComplete = true;
                if (pCallbackHandler != NULL) {
                    pCallbackHandler->OnComplete(static_cast<void*>(this));
                }
            }
        }
    } else {
        --currentStep;
        if (currentStep <= 0) {
            currentStep = totalSteps - 1;
            if (animationComplete == false) {
                animationComplete = true;
                if (pCallbackHandler != NULL) {
                    pCallbackHandler->OnComplete(static_cast<void*>(this));
                }
            }
        }
    }
}

void NeoPixel_Section::Reverse() {
    if (direction == FORWARD) {
        direction = REVERSE;
        currentStep = totalSteps - 1;
    } else {
        direction = FORWARD;
        currentStep = 0;
    }
    // animationComplete == false;
}

void NeoPixel_Section::StripOn(AnimationConfig config) { color1 = config.color1; }

void NeoPixel_Section::StripOff(AnimationConfig config) {
    color1 = ledStrip->Color(0, 0, 0);
}

void NeoPixel_Section::RainbowCycle(AnimationConfig config) {
    // Maybe don't have totalSteps as 255 - make it dependent on length?
    totalSteps = 255;
    direction = config.direction;
}

void NeoPixel_Section::TheaterChase(AnimationConfig config) {
    totalSteps = stripLength;
    color1 = config.color1;
    color2 = config.color2;
    direction = config.direction;
}

void NeoPixel_Section::ColorWipe(AnimationConfig config) {
    totalSteps = stripLength;
    color1 = config.color1;
    direction = config.direction;
}

void NeoPixel_Section::Scanner(AnimationConfig config) {
    totalSteps = (stripLength - 1) * 2;
    color1 = config.color1;
}

void NeoPixel_Section::Fade(AnimationConfig config) {
    totalSteps = config.steps;
    color1 = config.color1;
    color2 = config.color2;
    direction = config.direction;
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
        ledStrip->fill(color1, stripStart, stripLength);
    }
    Increment();
}

void NeoPixel_Section::RainbowCycleUpdate() {
    for (int i = 0; i < stripLength; i++) {
        ledStrip->setPixelColor(i + stripStart,
                                Wheel(((i * 256 / stripLength) + currentStep) & 255));
    }
    Increment();
}
void NeoPixel_Section::TheaterChaseUpdate() {
    for (int i = 0; i < stripLength; i++) {
        if ((i + currentStep) % 3 == 0) {
            ledStrip->setPixelColor(i + stripStart, color1);
        } else {
            ledStrip->setPixelColor(i + stripStart, color2);
        }
    }
    Increment();
#ifdef SERIAL_DEBUG
    Serial.print("TheaterChaseUpdate ");
    Serial.print(currentStep);
    Serial.print(" ");
    Serial.print(color1);
    Serial.print(" ");
    Serial.println(color2);
#endif
}

void NeoPixel_Section::ColorWipeUpdate() {
    ledStrip->setPixelColor(currentStep + stripStart, color1);
    Increment();
}

void NeoPixel_Section::ScannerUpdate() {
    for (int i = 0; i < stripLength; i++) {
        if (i == currentStep) // Scan Pixel to the right
        {
            ledStrip->setPixelColor(i + stripStart, color1);
        } else if (i == totalSteps - currentStep) // Scan Pixel to the left
        {
            ledStrip->setPixelColor(i + stripStart, color1);
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
        ((Red(color1) * (totalSteps - currentStep)) + (Red(color2) * currentStep)) /
        totalSteps;
    uint8_t green =
        ((Green(color1) * (totalSteps - currentStep)) + (Green(color2) * currentStep)) /
        totalSteps;
    uint8_t blue =
        ((Blue(color1) * (totalSteps - currentStep)) + (Blue(color2) * currentStep)) /
        totalSteps;

    ColorSet(ledStrip->Color(red, green, blue));
    //    ledStrip->show();
    Increment();
#ifdef SERIAL_DEBUG
    Serial.print("FadeUpdate ");
    Serial.print(currentStep);
    Serial.print(" ");
    Serial.print(red);
    Serial.print(" ");
    Serial.print(green);
    Serial.print(" ");
    Serial.println(blue);
#endif
}
