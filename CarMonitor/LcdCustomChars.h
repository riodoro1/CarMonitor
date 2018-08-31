#pragma once

#include <LiquidCrystal.h>

enum CustomChar : uint8_t {
    Degree = 0,
    Thermometer = 1
};

inline void registerChars(const LiquidCrystal &lcd) {
    byte degreeCharData[8] = {
        0b00110,
        0b01001,
        0b01001,
        0b00110,
        0b00000,
        0b00000,
        0b00000,
        0b00000
    };
    
    byte thermometerCharData[8] = {
        0b00100,
        0b01010,
        0b01010,
        0b01010,
        0b01110,
        0b11111,
        0b11111,
        0b01110
    };
    
    lcd.createChar(CustomChar::Degree, degreeCharData);
    lcd.createChar(CustomChar::Thermometer, thermometerCharData);
}
