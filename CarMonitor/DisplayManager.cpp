#include "DisplayManager.h"

#include <LiquidCrystal.h>

#include "Sensor.h"
#include "DS18B20Sensor.h"
#include "PinManagment.h"

DisplayManager::DisplayManager(const LiquidCrystal &lcd, SimpleCyclicIterator<Sensor*> &sensors, unsigned int refreshPeriod, unsigned int displayChangeInterval)
:_lcd(lcd)
,_sensors(sensors)
,_sensorsNeedRefresh(true)
,_sensorsHaveErrors(false)
,_refreshPeriod(refreshPeriod)
,_lastDisplayRefresh(0)
,_displayChangeInterval(displayChangeInterval)
,_lastDisplayChange(0)
,_isPaused(false)
,_brightnessLevel(BRIGHTNESS_LEVELS)
{
    analogWrite(BRIGHTNESS_OUT_PIN, 0xFF);
}

void DisplayManager::requestSensorsRefresh() {
    _sensorsNeedRefresh = true;
}

void DisplayManager::refresh() {
    if(_sensorsNeedRefresh) {
        refreshSensors();
        _sensorsNeedRefresh = false;
    }
    
    serviceButtons();
    
    if(displayNeedsRefresh()) {
        refreshDisplay();
    }
}

// =============================================================== Sensors refresh

void DisplayManager::refreshSensors() {
    if ( _sensors.count() == 0 ) {
        return;
    }
    
    Sensor **start = _sensors.current();
    _sensorsHaveErrors = false;

    do {
        Sensor *current = *_sensors.current();
        current->fetchValue();
        if(current->getStatus() == SensorStatus::Error) {
            _sensorsHaveErrors = true;
            _isPaused = false;
        }
    }while (_sensors.next() != start);
}

// =============================================================== Display refresh and changing

void DisplayManager::changeDisplay() {
    if(_sensorsHaveErrors) {
        do {
            _sensors.next();
        } while((*_sensors.current())->getStatus() != SensorStatus::Error);
    }
    else {
        Sensor **start = _sensors.current();
        do {
            _sensors.next();
        } while((*_sensors.current())->wantsDisplay() == false && _sensors.next() != start);
    }
    
    _lastDisplayChange = millis();
}

bool DisplayManager::shouldChangeDisplay() const {
    return !_isPaused && (_lastDisplayChange < millis() - _displayChangeInterval);
}

void DisplayManager::refreshDisplay() {
    static bool blinkFlag = true;
    
    if(shouldChangeDisplay()) {
        changeDisplay();
        blinkFlag = true;
    }
    
    _lcd.clear();
    
    Sensor *sensor = *_sensors.current();
    
    sensor->printDescription(_lcd);
    _lcd.setCursor(0,1);
    sensor->printValueString(_lcd);
    
    SensorStatus status = sensor->getStatus();
    
    if((status != SensorStatus::Ok) && blinkFlag) {
        _lcd.setCursor(7,1);
        _lcd.write('!');
    }
    if(status == SensorStatus::Error) {
        digitalWrite(BUZZER_PIN, blinkFlag);
    }
    else {
        digitalWrite(BUZZER_PIN, LOW);
    }
    
    blinkFlag = !blinkFlag;
    _lastDisplayRefresh = millis();
}

bool DisplayManager::displayNeedsRefresh() const{
    return _lastDisplayRefresh < millis() - _refreshPeriod;
}

// =============================================================== Button handling

void DisplayManager::serviceButtons() {
    const ButtonType button = pressedButton();
    
    for(uint8_t i=0; i<ButtonType::ButtonTypeCount; i++) {
        if(button == i) {
            _buttonPressCounter[i]++;
        }
        else {
            if(_buttonPressCounter[i] > BUTTON_HELD_COUNT) {
                buttonHeld(i);
            }
            else if(_buttonPressCounter[i] > BUTTON_PRESSED_COUNT) {
                buttonPressed(i);
            }
            _buttonPressCounter[i] = 0;
        }
    }
}

static ButtonType DisplayManager::pressedButton() {
    int adcValue = analogRead(BUTTONS_PIN);
    
    if(adcValue < ADC_MAX * PAUSE_BUTTON_TRESHOLD) {
        return Pause;
    }
    else if(adcValue < ADC_MAX * BRIGHTNESS_BUTTON_TRESHOLD) {
        return Brightness;
    }
    else if(adcValue < ADC_MAX * IGNORE_BUTTON_TRESHOLD) {
        return Ignore;
    }
    else {
        return ButtonTypeCount;
    }
}

void DisplayManager::buttonPressed(ButtonType type) {
    switch (type) {
        case Brightness:
            brightnessPressed();
            break;
        case Pause:
            pausePressed();
            break;
        case Ignore:
            ignorePressed();
            break;
    }
}

void DisplayManager::buttonHeld(ButtonType type) {
    switch (type) {
        case Ignore:
            ignoreHeld();
            break;
            
        default:
            break;
    }
}

void DisplayManager::brightnessPressed() {
    if(_brightnessLevel == 0) _brightnessLevel = BRIGHTNESS_LEVELS;
    _brightnessLevel--;
    
    uint8_t brightnessStep = 255 / (BRIGHTNESS_LEVELS - 1);
    analogWrite(BRIGHTNESS_OUT_PIN, brightnessStep * _brightnessLevel);
}

void DisplayManager::pausePressed() {
    _isPaused = !_isPaused;
}

void DisplayManager::ignorePressed() {
    Sensor *current = *_sensors.current();
    current->ignoreError();
}

void DisplayManager::ignoreHeld() {
    Sensor **start = _sensors.current();
    do {
        Sensor *current = *_sensors.current();
        current->unignore();
    }while (_sensors.next() != start);
}
