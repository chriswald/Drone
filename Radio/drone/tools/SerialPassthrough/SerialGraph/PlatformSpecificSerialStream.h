#ifndef __PLATFORM_SPECIFIC_SERIAL_STREAM_H__
#define __PLATFORM_SPECIFIC_SERIAL_STREAM_H__

#define WINDOWS
//#define LINUX

#include <string>

using std::string;

// Platform Includes
#ifdef WINDOWS
#include <Windows.h>
#else // LINUX

#endif

// Platform Types
#ifdef WINDOWS
typedef HANDLE HANDLE_TYPE;
#else // LINUX

#endif

// Platform Usings
#ifdef WINDOWS

#else // LINUX

#endif

class PSIOSerialStream
{
public:
    bool openComm(HANDLE_TYPE& handle, unsigned comm);
    bool close(HANDLE_TYPE& handle);
    string read(HANDLE_TYPE& handle);
    void write(HANDLE_TYPE& handle, string& output);
};

#endif // __PLATFORM_SPECIFIC_SERIAL_STREAM_H__