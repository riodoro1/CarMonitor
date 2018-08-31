#pragma once

#include "Sensor.h"

class DelayedErrorSensor : public Sensor {
    unsigned long _delay;
    
    bool _isInDelay;
    SensorStatus _previousStatus;
    unsigned long _delayDeadline;
    
public:
    DelayedErrorSensor(unsigned long delay);
    
protected:
    virtual bool setStatus(SensorStatus status);
};

inline DelayedErrorSensor::DelayedErrorSensor(unsigned long delay)
:_delay(delay)
{
    
}

inline bool DelayedErrorSensor::setStatus(SensorStatus status) {
    if(_previousStatus != SensorStatus::Error && status == SensorStatus::Error) {
        if(!_isInDelay) {
            _isInDelay = true;
            _delayDeadline = millis() + _delay;
        }
    }
    
    if(_isInDelay) {
        if(millis() < _delayDeadline) {
            return Sensor::setStatus(SensorStatus::Warning);
        }
        else {
            _isInDelay = false;
        }
    }
    
    _previousStatus = status;
    return Sensor::setStatus(status);
}
