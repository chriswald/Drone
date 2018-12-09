#include "Logger.h"
#include <stdio.h>
#include <Arduino.h>

Logger::Logger(char tag, char delim) : _tag(tag), _delim(delim), _index(0) {}

void Logger::log(float value)
{
    _q[_index] = value;
    _index++;
    if (_index > 9)
        flush();
}

void Logger::flush()
{
    char out[72];

    out[0] = _tag;
    for (int i = 0, index=1; i < 10; i ++)
        index += sprintf(out+index, "%c%0.4f", _delim, _q[i]);

    _index = 0;

    Serial.println(out);
}