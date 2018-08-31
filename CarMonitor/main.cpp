#include <Arduino.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimerOne.h>

#include "PinManagment.h"
#include "DebugFunctions.h"
#include "LcdCustomChars.h"

#include "DS18B20Sensor.h"
#include "DualFIT0212Sensor.h"
#include "ACS758BSensor.h"
#include "FanSensor.h"
#include "LightsSensor.h"
#include "BatteryVoltageSensor.h"

#include "DisplayManager.h"
#include "Watchdog.h"

#include "SimpleCyclicIterator.h"

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature dallasTemperature(&oneWire);
TimerOne timer;

uint8_t outsideTempAddress[8] = {0x28, 0xFF, 0x0D, 0x6C, 0x33, 0x16, 0x03, 0xB8};
uint8_t insideTempAddress[8] = {0x28, 0xFF, 0x07, 0xCB, 0xC0, 0x16, 0x04, 0xC0};
uint8_t engineTempAddress[8] = {0x28, 0xFF, 0x0D, 0xBA, 0x23, 0x16, 0x03, 0xFA};

DS18B20Sensor outsideTempSensor(dallasTemperature, outsideTempAddress, SimpleString("Outside"));
DS18B20Sensor insideTempSensor(dallasTemperature, insideTempAddress, SimpleString("Inside"), 40, 70);
DS18B20Sensor engineTempSensor(dallasTemperature, engineTempAddress, SimpleString("Engine"), 90, 100);

DualFIT0212Sensor coolantSensor;

ACS758BSensor batteryCurrentSensor("BAT ", BATTERY_AMPS_PIN, -100, 100, -2, 50);
ACS758BSensor alternatorCurrentSensor("ALT ", ALTERNATOR_AMPS_PIN, -100, 100, -2, 80);

FanSensor fanSensor(engineTempSensor, FAN_CONTROL_PIN, 40, 2);
LightsSensor lightsSensor(LIGHTS_PIN, IGNITION_PIN);
BatteryVoltageSensor batteryVoltageSensor(BATTERY_VOLTS_PIN, BATTERY_VOLTS_R2/(BATTERY_VOLTS_R2 + BATTERY_VOLTS_R1), 11);

Sensor *sensors[] = {
    //&outsideTempSensor,
    //&insideTempSensor,
    //&engineTempSensor,
    //&coolantSensor,
    //&batteryVoltageSensor,
    //&batteryCurrentSensor,
    //&alternatorCurrentSensor,
    //&fanSensor,
    &lightsSensor
};

SimpleCyclicIterator<Sensor*> sensorsIterator(sensors, sizeof(sensors) / sizeof(*sensors));

Watchdog watchdog(5000);
DisplayManager displayManager(lcd, sensorsIterator, 500, 2000);

void watchdogError() {
    digitalWrite(BUZZER_PIN, HIGH);
    while(1);
}

void timerISR() {
    if(!watchdog.check()) watchdogError();
    displayManager.requestSensorsRefresh();
}

int main(void)
{
    init();
    pinInit();
    
    Serial.begin(9600);
    
    registerChars(lcd);
    lcd.begin(8, 2);

    dallasTemperature.begin();
    dallasTemperature.setWaitForConversion(true);
    dallasTemperature.setResolution(9);
    
    enumerateDallasSensors(dallasTemperature);
    
    timer.initialize(1000000);
    timer.attachInterrupt(timerISR);
    timer.start();
    
    attachInterrupt(digitalPinToInterrupt(FAN_RPM_SENSOR_PIN), FanSensor::fanSensorInterrupt, FALLING);
    
    while(1) {
        loop();
    }
    return 0;
}

void loop() {
    watchdog.feed();
    displayManager.refresh();
}

