#ifndef __SERIAL_STREAM_H__
#define __SERIAL_STREAM_H__

#include "PlatformSpecificSerialStream.h"
#include <iostream>
#include <string>

using std::ostream;
using std::istream;
using std::string;

class SerialStream
{
public:
    bool open(unsigned comm);
    bool close();
    SerialStream& operator<<(string& output);
    SerialStream& operator>>(string& input);

    bool isOpen();
private:
    bool isopen;
    HANDLE_TYPE handle;
    PSIOSerialStream psio;
};

#endif // __SERIAL_STREAM_H__