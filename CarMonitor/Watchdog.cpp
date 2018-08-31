#include "Watchdog.h"

Watchdog::Watchdog(unsigned long treshold)
:_treshold(treshold)
{
    feed();
}

void Watchdog::feed() {
    _lastFeed = millis();
}

bool Watchdog::check() {
    unsigned long timeSinceLastFeed = millis() - _lastFeed;
    if(timeSinceLastFeed > _treshold)
        return false;
    else
        return true;
}
