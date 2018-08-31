#include "DualFIT0212Sensor.h"

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "PinManagment.h"

namespace {
    constexpr unsigned long errorDelay = 1000;
}

DualFIT0212Sensor::DualFIT0212Sensor()
:DelayedErrorSensor(errorDelay)
{
    
}

DualFIT0212Sensor::~DualFIT0212Sensor() {
    
}

void DualFIT0212Sensor::fetchValue() {
    int lowState = digitalRead(COOLANT_LOW_PIN);
    int highState = digitalRead(COOLANT_HIGH_PIN);

    if(lowState == LOW && highState == HIGH) {
        _coolantLevel = CoolantLevel::Ok;
    }
    else if(lowState == LOW && highState == LOW) {
        _coolantLevel = CoolantLevel::TooHigh;
    }
    else if(lowState == HIGH && highState == HIGH) {
        _coolantLevel = CoolantLevel::TooLow;
    }
    else {
        _coolantLevel = CoolantLevel::Undetermined;
    }
    
    setStatus(coolantLevelToSensorStatus(_coolantLevel));
}

bool DualFIT0212Sensor::wantsDisplay() const {
    return getStatus() != SensorStatus::Ok;
}

void DualFIT0212Sensor::printDescription(const LiquidCrystal &lcd) const {
    lcd.print("Coolant");
}

void DualFIT0212Sensor::printValueString(const LiquidCrystal &lcd) const {
    switch (_coolantLevel) {
        case Ok:
            lcd.print("   OK");
            break;
            
        case TooLow:
            lcd.print("  Low");
            break;
            
        case TooHigh:
            lcd.print("  High");
            break;
            
        case Undetermined:
            lcd.print("ERROR");
            break;
    }
}

static SensorStatus DualFIT0212Sensor::coolantLevelToSensorStatus(DualFIT0212Sensor::CoolantLevel coolantLevel) {
    if(coolantLevel == CoolantLevel::Ok)
        return SensorStatus::Ok;
    else
        return SensorStatus::Error;
}
