#include "NeoPixel_Section.h"
//#define SERIAL_DEBUG 1
NeoPixel_Section::NeoPixel_Section(Adafruit_NeoPixel* strip, uint16_t start,
                                   uint16_t length, void (*callback)()) {
    ledStrip = strip;
    stripStart = start;
    stripLength = length;
    OnComplete = callback;
}

void NeoPixel_Section::SetPattern(PatternConfig config) {
    switch (config.pattern) {
    case STRIP_ON:
        StripOn(config);
        break;
    case STRIP_OFF:
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

void NeoPixel_Section::Update() {
    if ((millis() - lastUpdate) > Interval) // time to update
    {
        lastUpdate = millis();
        switch (ActivePattern) {
        case STRIP_ON:
            StripOnUpdate();
            break;
        case STRIP_OFF:
            StripOffUpdate();
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
    if (Direction == FORWARD) {
        Index++;
        if (Index >= TotalSteps) {
            Index = 0;
            if (OnComplete != NULL) {
                OnComplete();
            }
        }
    } else {
        --Index;
        if (Index <= 0) {
            Index = TotalSteps - 1;
            if (OnComplete != NULL) {
                OnComplete();
            }
        }
    }
}

void NeoPixel_Section::Reverse() {
    if (Direction == FORWARD) {
        Direction = REVERSE;
        Index = TotalSteps - 1;
    } else {
        Direction = FORWARD;
        Index = 0;
    }
}

void NeoPixel_Section::StripOn(PatternConfig config) {
    ActivePattern = STRIP_ON;
    Interval = config.interval;
    Color1 = config.color1;
    Index = 0;
}

void NeoPixel_Section::StripOff(PatternConfig config) {
    ActivePattern = STRIP_OFF;
    Interval = config.interval;
    Index = 0;
}

void NeoPixel_Section::RainbowCycle(PatternConfig config) {
    ActivePattern = RAINBOW_CYCLE;
    Interval = config.interval;
    TotalSteps = 255;
    Index = 0;
    Direction = config.dir;
}

void NeoPixel_Section::TheaterChase(PatternConfig config) {
    ActivePattern = THEATER_CHASE;
    Interval = config.interval;
    TotalSteps = stripLength;
    Color1 = config.color1;
    Color2 = config.color2;
    Index = 0;
    Direction = config.dir;
}

void NeoPixel_Section::ColorWipe(PatternConfig config) {
    ActivePattern = COLOR_WIPE;
    Interval = config.interval;
    TotalSteps = stripLength;
    Color1 = config.color1;
    Index = 0;
    Direction = config.dir;
}

void NeoPixel_Section::Scanner(PatternConfig config) {
    ActivePattern = SCANNER;
    Interval = config.interval;
    TotalSteps = (stripLength - 1) * 2;
    Color1 = config.color1;
    Index = 0;
}

void NeoPixel_Section::Fade(PatternConfig config) {
    ActivePattern = FADE;
    Interval = config.interval;
    TotalSteps = config.steps;
    Color1 = config.color1;
    Color2 = config.color2;
    Index = 0;
    Direction = config.dir;
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
void NeoPixel_Section::StripOnUpdate() {
    if (Index == 0) {
        ledStrip->fill(Color1, stripStart, stripLength);
    }
    Increment();
}

void NeoPixel_Section::StripOffUpdate() {
    if (Index == 0) {
        ledStrip->fill(ledStrip->Color(0, 0, 0), stripStart, stripLength);
    }
    Increment();
}
void NeoPixel_Section::RainbowCycleUpdate() {
    for (int i = 0; i < stripLength; i++) {
        ledStrip->setPixelColor(i + stripStart,
                                Wheel(((i * 256 / stripLength) + Index) & 255));
    }
    Increment();
}
void NeoPixel_Section::TheaterChaseUpdate() {
    for (int i = 0; i < stripLength; i++) {
        if ((i + Index) % 3 == 0) {
            ledStrip->setPixelColor(i + stripStart, Color1);
        } else {
            ledStrip->setPixelColor(i + stripStart, Color2);
        }
    }
    //    ledStrip->show();
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

void NeoPixel_Section::ColorWipeUpdate() {
    ledStrip->setPixelColor(Index + stripStart, Color1);
    Increment();
}

void NeoPixel_Section::ScannerUpdate() {
    for (int i = 0; i < stripLength; i++) {
        if (i == Index) // Scan Pixel to the right
        {
            ledStrip->setPixelColor(i + stripStart, Color1);
        } else if (i == TotalSteps - Index) // Scan Pixel to the left
        {
            ledStrip->setPixelColor(i + stripStart, Color1);
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
    // Calculate linear interpolation between Color1 and Color2
    // Optimise order of operations to minimize truncation error
    uint8_t red =
        ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) /
        TotalSteps;
    uint8_t green =
        ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) /
        TotalSteps;
    uint8_t blue =
        ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) /
        TotalSteps;

    ColorSet(ledStrip->Color(red, green, blue));
    //    ledStrip->show();
    Increment();
#ifdef SERIAL_DEBUG
    Serial.print("FadeUpdate ");
    Serial.print(Index);
    Serial.print(" ");
    Serial.print(red);
    Serial.print(" ");
    Serial.print(green);
    Serial.print(" ");
    Serial.println(blue);
#endif
}
