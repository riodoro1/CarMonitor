#include "BatteryVoltageSensor.h"

#include <Arduino.h>
#include <LiquidCrystal.h>

namespace {
    constexpr unsigned long errorDelay = 5000;
}

BatteryVoltageSensor::BatteryVoltageSensor(uint8_t batteryPin, float dividerCoefficient, float min)
:DelayedErrorSensor(errorDelay)
,_batteryPin(batteryPin)
,_dividerCoefficient(dividerCoefficient)
,_min(min)
{

}

void BatteryVoltageSensor::fetchValue() {
    _lastValue = measureVoltage();
    
    if ( _lastValue < _min ) {
        setStatus(SensorStatus::Error);
    }
}


void BatteryVoltageSensor::printDescription(const LiquidCrystal &lcd) const {
    lcd.print("BAT");
}

void BatteryVoltageSensor::printValueString(const LiquidCrystal &lcd) const {
    lcd.print(_lastValue, 1);
    lcd.print(" V");
}

float BatteryVoltageSensor::measureVoltage() const {
    int adcValue = analogRead(_batteryPin);
    return adcValue * 5.0 / 1023.0 / _dividerCoefficient;
}
