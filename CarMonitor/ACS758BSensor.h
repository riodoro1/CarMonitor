#pragma once

#include "Sensor.h"
#include "SimpleString.h"

class ACS758BSensor : public Sensor {
    SimpleString _componentName;
    uint8_t _pin;
    float _rangeFrom;
    float _rangeTo;
    float _lastValue;
    
    float _min;
    float _max;
    
public:
    ACS758BSensor(const SimpleString &componentName, uint8_t pin, float rangeFrom, float rangeTo, float min, float max);
    ~ACS758BSensor();
    
    virtual void fetchValue();
    
    virtual void printDescription(const LiquidCrystal &lcd) const;
    virtual void printValueString(const LiquidCrystal &lcd) const;
    
private:
    float valueFromReading(int adcReading);
};
