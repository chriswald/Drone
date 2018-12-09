#ifndef __BATTERY_MONITOR_H__
#define __BATTERY_MONITOR_H__

#include <Arduino.h>

class BattMon {
public:
    void config(int analog_pin, float battery_max);
    void setMaxADCPinVoltage(float voltage);
    float getVoltage();
    float getPercent();
private:
    int   pin;
    float maxBatteryVoltage;
    float maxADCVoltage;
};

#endif // __BATTERY_MONITOR_H__