#include "RangeSensor.h"

void RangeSensor::config(byte trig_pin, byte echo_pin)
{
    _trig = trig_pin;
    _echo = echo_pin;
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
}

float RangeSensor::getDistanceCM()
{
    float duration, distance;
    digitalWrite(_echo, LOW);
    digitalWrite(_trig, LOW);
    delayMicroseconds(4);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);
    duration = pulseIn(_echo, HIGH, 300000);
    distance = (duration/2) / 29.1;
    return distance;
}