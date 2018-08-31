#pragma once

#include <Arduino.h>

class Watchdog {
    unsigned long _lastFeed;
    unsigned long _treshold;
public:
    Watchdog(unsigned long treshold = 2000);
    
    void feed();
    bool check();
};
