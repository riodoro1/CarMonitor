#pragma once

#include "DelayedErrorSensor.h"
#include "SimpleString.h"

class LightsSensor : public DelayedErrorSensor {
    uint8_t _lightsPin;
    uint8_t _ignitionPin;
    
public:
    LightsSensor(uint8_t lightsPin, uint8_t ignitionPin);
    LightsSensor();
    
    virtual void fetchValue();
    
    virtual bool wantsDisplay() const;
    virtual void printDescription(const LiquidCrystal &lcd) const;
    virtual void printValueString(const LiquidCrystal &lcd) const;
};
