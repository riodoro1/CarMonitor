#pragma once

#include <Arduino.h>

class LiquidCrystal;

enum SensorStatus : uint8_t {
    Ok = 0,
    Warning = 1,
    Error = 2,
    ErrorIgnored = 3
};

class Sensor {
public:
    Sensor() = default;
    ~Sensor() = default;
    
    virtual void fetchValue() = 0;
    
    virtual bool wantsDisplay() const;
    virtual void printDescription(const LiquidCrystal &lcd) const = 0;
    virtual void printValueString(const LiquidCrystal &lcd) const = 0;
    
    SensorStatus getStatus() const;
    void ignoreError();
    void unignore();
    
protected:
    SensorStatus _status;
    
protected:
    virtual bool setStatus(SensorStatus status);
};

inline bool Sensor::wantsDisplay() const {
    return true;
}

inline SensorStatus Sensor::getStatus() const {
    return _status;
}

inline void Sensor::ignoreError() {
    if(_status == SensorStatus::Error) {
        _status = ErrorIgnored;
    }
}

inline void Sensor::unignore() {
    if(_status == ErrorIgnored) {
        _status = Error;
    }
}

inline bool Sensor::setStatus(SensorStatus status) {
    if(_status == ErrorIgnored && status == Error) {
        return false;
    }
    else {
        _status = status;
        return true;
    }
}
