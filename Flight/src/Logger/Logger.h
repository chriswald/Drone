#ifndef __LOGGER_H__
#define __LOGGER_H__

class Logger
{
public:
    Logger(char tag, char delim=',');
    void log(float value);

private:
    float _q[10];
    char _tag, _delim;
    unsigned char _index;

    void flush();
};

#endif // __LOGGER_H__