#pragma once

#include "Sensor.h"
#include "SimpleString.h"

#define MINUTE 60000

class DS18B20Sensor;

class FanSensor : public Sensor {
    uint8_t _controlPin;
    
    const DS18B20Sensor &_engineTempSensor;
    
    unsigned long _lastCheckTime;
    float _lastRpm;
    
    const float _tempTreshold;
    const float _hysteresis;
    
    bool _isInControl;
    
public:
    FanSensor(const DS18B20Sensor &engineTempSensor, uint8_t controlPin, const float tempTreshold, const float hysteresis);
    FanSensor();
    
    virtual void fetchValue();
    
    virtual void printDescription(const LiquidCrystal &lcd) const;
    virtual void printValueString(const LiquidCrystal &lcd) const;
    
    static void fanSensorInterrupt();
    
private:
    float measureRpm();
};
