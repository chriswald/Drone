#include "BattMon.h"

void BattMon::config(int analog_pin, float battery_max)
{
    pin = analog_pin;
    pinMode(pin, INPUT);
    maxADCVoltage = 3.3;
    maxBatteryVoltage = battery_max;
}

void BattMon::setMaxADCPinVoltage(float voltage)
{
    maxADCVoltage = voltage;
}

float BattMon::getVoltage()
{
    int adc = analogRead(pin);
    float max_adc = maxBatteryVoltage/maxADCVoltage*1024.0;
    float voltage = maxBatteryVoltage * ((float)adc/max_adc);
    return voltage;
}

float BattMon::getPercent()
{
    float voltage = getVoltage();
    float percent = voltage / maxBatteryVoltage * 100.0;
    return percent;
}