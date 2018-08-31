#pragma once

#include <Arduino.h>
#include "SimpleCyclicIterator.h"

#define BRIGHTNESS_LEVELS 4
#define BUTTON_PRESSED_COUNT 1
#define BUTTON_HELD_COUNT 6000

#define IGNORE_BUTTON_TRESHOLD 0.9
#define BRIGHTNESS_BUTTON_TRESHOLD 0.6
#define PAUSE_BUTTON_TRESHOLD 0.4


enum ButtonType : uint8_t {
    Brightness = 0,
    Pause = 1,
    Ignore = 2,
    ButtonTypeCount
};

class LiquidCrystal;
class Sensor;

class DisplayManager {
    const LiquidCrystal &_lcd;
    SimpleCyclicIterator<Sensor*> _sensors;
    
    bool _sensorsNeedRefresh;
    bool _sensorsHaveErrors;
    
    unsigned int _refreshPeriod;
    unsigned long _lastDisplayRefresh;
    unsigned int _displayChangeInterval;
    unsigned long _lastDisplayChange;
    
    unsigned long _buttonPressCounter[ButtonType::ButtonTypeCount];
    
    bool _isPaused;
    uint8_t _brightnessLevel;
    
public:
    DisplayManager(const LiquidCrystal &lcd, SimpleCyclicIterator<Sensor*> &sensors, unsigned int refreshPeriod, unsigned int changeDisplayInterval);
    
    void requestSensorsRefresh();
    
    void refresh();
    
private:
    void refreshSensors();
    
    void changeDisplay();
    bool shouldChangeDisplay() const;
    
    void refreshDisplay();
    bool displayNeedsRefresh() const;
    
    void serviceButtons();
    static ButtonType pressedButton();
    
    void buttonPressed(ButtonType type);
    void buttonHeld(ButtonType type);
    
    void brightnessPressed();
    void pausePressed();
    void ignorePressed();
    void ignoreHeld();
};
