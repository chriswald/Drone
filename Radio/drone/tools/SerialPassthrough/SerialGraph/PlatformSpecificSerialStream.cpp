#include "PlatformSpecificSerialStream.h"

#ifdef WINDOWS
bool PSIOSerialStream::openComm(HANDLE_TYPE& handle, unsigned comm)
{
    char commpath[14];
    sprintf_s(commpath, "\\\\.\\COM%d", comm);
    handle = CreateFile(commpath, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);

    if (!GetCommState(handle, &serialParams))
    {
        return false;
}

    serialParams.BaudRate = CBR_9600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;

    if (!SetCommState(handle, &serialParams))
    {
        return false;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutConstant = 1;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 1;
    timeouts.WriteTotalTimeoutMultiplier = 1;

    if (!SetCommTimeouts(handle, &timeouts))
    {
        return false;
    }

    PurgeComm(handle, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);

    return true;
}

bool PSIOSerialStream::close(HANDLE_TYPE& handle)
{
    return CloseHandle(handle);
}

string PSIOSerialStream::read(HANDLE_TYPE& handle)
{
    string ret = "";
    char buffer[1024];
    DWORD bytesRead = 0;

    memset(buffer, 0, 1024);
    ReadFile(handle, buffer, 1023, &bytesRead, NULL);
    while (bytesRead != 0)
    {
        ret += buffer;

        memset(buffer, 0, 1024);
        ReadFile(handle, buffer, 1023, &bytesRead, NULL);
    }

    return ret;
}

void PSIOSerialStream::write(HANDLE_TYPE& handle, string& output)
{
    WriteFile(handle, output.c_str(), output.length(), NULL, NULL);
}

#else // LINUX

#endif