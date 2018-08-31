#pragma once

#include "Sensor.h"
#include "SimpleString.h"

class DallasTemperature;

class DS18B20Sensor : public Sensor {
    const DallasTemperature &_reader;
    const uint8_t *_address;
    const SimpleString _name;
    
    const float _warningTreshold;
    const float _errorTreshold;
    
    float _lastValue;
    
public:
    DS18B20Sensor(const DallasTemperature &reader, const uint8_t *address, SimpleString name, float warningTreshold = 100, float errorTreshold = 100);
    ~DS18B20Sensor();
    
    virtual void fetchValue();

    virtual void printDescription(const LiquidCrystal &lcd) const;
    virtual void printValueString(const LiquidCrystal &lcd) const;
    
    float getLastTemperature() const;
    
private:
    bool readErrorOccured() const;
    static void requestTemperatures(const DallasTemperature &reader);
};
