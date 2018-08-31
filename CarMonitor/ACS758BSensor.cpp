#include "ACS758BSensor.h"

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "PinManagment.h"

ACS758BSensor::ACS758BSensor(const SimpleString &componentName, uint8_t pin, float rangeFrom, float rangeTo, float min, float max)
:_componentName(componentName)
,_pin(pin)
,_rangeFrom(rangeFrom)
,_rangeTo(rangeTo)
,_min(min)
,_max(max)
{
    
}

ACS758BSensor::~ACS758BSensor() {
    
}

void ACS758BSensor::fetchValue() {
    int adcReading = analogRead(_pin);
    _lastValue = valueFromReading(adcReading);
    
    if ( _lastValue < _min || _lastValue > _max ) {
        setStatus(SensorStatus::Error);
    }
}

void ACS758BSensor::printDescription(const LiquidCrystal &lcd) const {
    lcd.print(_componentName.cString());
}

void ACS758BSensor::printValueString(const LiquidCrystal &lcd) const {
    lcd.print(_lastValue, 0);
    lcd.print(" A");
}

float ACS758BSensor::valueFromReading(int adcReading) {
    float scaled = (float)adcReading / ADC_MAX;
    return (1 - scaled) * _rangeFrom + scaled * _rangeTo;
}
