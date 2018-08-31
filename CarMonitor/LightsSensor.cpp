#include "LightsSensor.h"

#include <Arduino.h>
#include <LiquidCrystal.h>

namespace {
    constexpr unsigned long errorDelay = 5000;
}

LightsSensor::LightsSensor(uint8_t lightsPin, uint8_t ignitionPin)
:DelayedErrorSensor(errorDelay)
,_lightsPin(lightsPin)
,_ignitionPin(ignitionPin)
{
}

void LightsSensor::fetchValue() {
    if(digitalRead(_ignitionPin) != digitalRead(_lightsPin)) {
        setStatus(SensorStatus::Error);
    }
    else {
        setStatus(SensorStatus::Ok);
    }
}

bool LightsSensor::wantsDisplay() const {
    return getStatus() != SensorStatus::Ok;
}

void LightsSensor::printDescription(const LiquidCrystal &lcd) const {
    lcd.print("LIGHTS");
}

void LightsSensor::printValueString(const LiquidCrystal &lcd) const {
    if(digitalRead(_lightsPin) == HIGH) {
        lcd.print("   ON");
    }
    else {
        lcd.print("  OFF");
    }
}
