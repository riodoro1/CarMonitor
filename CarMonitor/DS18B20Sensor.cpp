#include "DS18B20Sensor.h"

#include <LiquidCrystal.h>
#include <DallasTemperature.h>

#include "LcdCustomChars.h"

DS18B20Sensor::DS18B20Sensor(const DallasTemperature &reader, const uint8_t *address, SimpleString name, float warningTreshold, float errorTreshold)
:_reader(reader)
,_address(address)
,_name(name)
,_warningTreshold(warningTreshold)
,_errorTreshold(errorTreshold)
{
}

DS18B20Sensor::~DS18B20Sensor() {
    delete _address;
}

void DS18B20Sensor::fetchValue() {
    requestTemperatures(_reader);
    
    _lastValue = _reader.getTempC(_address);
    
    if(_lastValue >= _errorTreshold || readErrorOccured()) {
        setStatus(SensorStatus::Error);
    }
    else if (_lastValue >= _warningTreshold) {
        setStatus(SensorStatus::Warning);
    }
    else {
        setStatus(SensorStatus::Ok);
    }
}

void DS18B20Sensor::printDescription(const LiquidCrystal &lcd) const {
    lcd.print(_name.cString());
}

void DS18B20Sensor::printValueString(const LiquidCrystal &lcd) const {
    if(readErrorOccured()) {
        lcd.print("NO DATA");
    }
    else {
        lcd.print(_lastValue, 1);
        lcd.write(CustomChar::Degree);
        lcd.write("C");
    }
}

float DS18B20Sensor::getLastTemperature() const {
    return _lastValue;
}

bool DS18B20Sensor::readErrorOccured() const {
    if(_lastValue == -127.0) {
        return true;
    }
    return false;
}

static void DS18B20Sensor::requestTemperatures(const DallasTemperature &reader) {
    static int fetchCountdown = 0;
    
    if(fetchCountdown <= 0) {
        reader.requestTemperatures();
        fetchCountdown = reader.getDS18Count() - 1;
    }
    else {
        fetchCountdown--;
    }
}
