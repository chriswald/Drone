#ifndef __RANGE_SENSOR_H__
#define __RANGE_SENSOR_H__

#include <Arduino.h>

class RangeSensor
{
public:
    void  config(byte trig_pin, byte echo_pin);
    float getDistanceCM();

private:
    byte _trig, _echo;
};

#endif // __RANGE_SENSOR_H__