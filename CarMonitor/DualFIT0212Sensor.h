#pragma once

#include "DelayedErrorSensor.h"

class DualFIT0212Sensor : public DelayedErrorSensor {
    enum CoolantLevel : uint8_t {
        Ok = 0,
        TooLow = 1,
        TooHigh = 2,
        Undetermined = 3
    };
    CoolantLevel _coolantLevel;
public:
    DualFIT0212Sensor();
    ~DualFIT0212Sensor();
    
    virtual void fetchValue();
    
    virtual bool wantsDisplay() const;
    virtual void printDescription(const LiquidCrystal &lcd) const;
    virtual void printValueString(const LiquidCrystal &lcd) const;
    
private:
    static SensorStatus coolantLevelToSensorStatus(CoolantLevel coolantLevel);
};
