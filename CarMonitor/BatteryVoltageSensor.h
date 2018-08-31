#pragma once

#include "DelayedErrorSensor.h"
#include "SimpleString.h"

class BatteryVoltageSensor : public DelayedErrorSensor {
    uint8_t _batteryPin;
    float _dividerCoefficient;
    
    float _lastValue;
    float _min;
    
public:
    BatteryVoltageSensor(uint8_t batteryPin, float dividerCoefficient, float min);
    BatteryVoltageSensor();
    
    virtual void fetchValue();
    
    virtual void printDescription(const LiquidCrystal &lcd) const;
    virtual void printValueString(const LiquidCrystal &lcd) const;
    
private:
    float measureVoltage() const;
};
