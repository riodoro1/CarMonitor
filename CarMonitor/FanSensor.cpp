#include "FanSensor.h"

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "DS18B20Sensor.h"

FanSensor::FanSensor(const DS18B20Sensor &engineTempSensor, uint8_t controlPin, const float tempTreshold, const float hysteresis)
:_controlPin(controlPin)
,_engineTempSensor(engineTempSensor)
,_lastCheckTime(0)
,_lastRpm(0.0)
,_tempTreshold(tempTreshold)
,_hysteresis(hysteresis)
,_isInControl(false)
{
}

FanSensor::FanSensor() {
    
}

void FanSensor::fetchValue() {
    float rpm = measureRpm();
    float engineTemperature = _engineTempSensor.getLastTemperature();
    
    _lastRpm = rpm;
    
    if(engineTemperature > _tempTreshold) {
        digitalWrite(_controlPin, HIGH);
        
        if(_isInControl && rpm < 1000.0) {
            setStatus(SensorStatus::Error);
            return;
        }
        
        _isInControl = true;
    }
    else if(engineTemperature < _tempTreshold - _hysteresis) {
        digitalWrite(_controlPin, LOW);
        _isInControl = false;
    }
    setStatus(SensorStatus::Ok);
}

void FanSensor::printDescription(const LiquidCrystal &lcd) const {
    lcd.print("FAN");
}

void FanSensor::printValueString(const LiquidCrystal &lcd) const {
    lcd.print(_lastRpm, 0);
    lcd.print(" RPM");
}

volatile unsigned int _revCounter = 0;

void FanSensor::fanSensorInterrupt() {
    _revCounter++;
}

float FanSensor::measureRpm() {
    unsigned long now = millis();
    unsigned long interval = now - _lastCheckTime;
    
    float rpm = MINUTE * (float)_revCounter / (float)interval;
    
    _lastCheckTime = now;
    _revCounter = 0;
    
    return rpm;
}
