#pragma once

#include <Arduino.h>

#define IGNITION_PIN            2 //-
#define FAN_RPM_SENSOR_PIN      3 //-

#define LCD_RS                  4 //-
#define LCD_E                   5 //-
#define LCD_D4                  6 //-
#define LCD_D5                  7 //-
#define LCD_D6                  8 //-
#define LCD_D7                  9 //-

#define BUZZER_PIN              10 //-
#define BRIGHTNESS_OUT_PIN      11 //-

#define ONE_WIRE_PIN            12 //-

#define FAN_CONTROL_PIN         13 //-

#define BUTTONS_PIN             A0 //-

#define COOLANT_HIGH_PIN        A1 //-
#define COOLANT_LOW_PIN         A2 //-
#define ALTERNATOR_AMPS_PIN     A3 //-
#define BATTERY_AMPS_PIN        A4 //-
#define LIGHTS_PIN              A5 //-

#define BATTERY_VOLTS_PIN       A7 //-

#define BATTERY_VOLTS_R1        21.6
#define BATTERY_VOLTS_R2        9.8

#define ADC_MAX 1023

inline void pinInit() {
//direction
    pinMode(IGNITION_PIN, INPUT);
    pinMode(FAN_RPM_SENSOR_PIN, INPUT);
    
    pinMode(LIGHTS_PIN, INPUT);
    pinMode(BRIGHTNESS_OUT_PIN, OUTPUT);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(FAN_CONTROL_PIN, OUTPUT);
    pinMode(COOLANT_HIGH_PIN, INPUT);
    pinMode(COOLANT_LOW_PIN, INPUT);
    
//state
    analogWrite(BRIGHTNESS_OUT_PIN, 0);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(FAN_CONTROL_PIN, LOW);
}
