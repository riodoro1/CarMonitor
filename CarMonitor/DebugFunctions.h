#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>

inline void enumerateDallasSensors(const DallasTemperature &dallas) {
    for (int i=0; i<dallas.getDeviceCount(); i++) {
        Serial.print("Found OneWire sensor ");
        Serial.print(i);
        Serial.print(". Address: ");
        
        uint8_t address[8];
        dallas.getAddress(address, i);
        for (int j=0; j<8; j++) {
            if(address[j] <= 0xf) Serial.print("0");
            Serial.print(address[j], HEX);
            Serial.print(" ");
        }
        
        Serial.println();
    }
}
