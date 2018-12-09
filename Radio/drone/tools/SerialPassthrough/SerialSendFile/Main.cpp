#include <Windows.h>
#include <iostream>
#include <sstream>
#include <cstdio>

using namespace std;

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool openComm(HANDLE* handle, int num)
{
    char commpath[14];
    sprintf_s(commpath, "\\\\.\\COM%d", num);

    *handle = CreateFile(commpath, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (*handle == INVALID_HANDLE_VALUE)
    {
        cout << "Error opening serial" << endl;
        return false;
    }

    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);

    if (!GetCommState(*handle, &serialParams))
    {
        cout << "Error getting comm state" << endl;
        return false;
    }

    serialParams.BaudRate = CBR_9600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;

    if (!SetCommState(*handle, &serialParams))
    {
        cout << "Error setting comm state" << endl;
        return false;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(*handle, &timeouts))
    {
        cout << "Error setting timeouts" << endl;
        return false;
    }
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        cout << "Usage: SerialSendFile <com number> <filename>" << endl;
        return -1;
    }

    int commnum = atoi(argv[1]);
    char* filename = argv[2];

    HANDLE outHandle, fileHandle;

    fileHandle = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    bool success = openComm(&outHandle, commnum);

    if (!success)
    {
        return -1;
    }

    int bufferlen = 1024;
    char buffer[1025];
    DWORD bytesread = 0;
    int result = 0;
    
    result = ReadFile(fileHandle, buffer, bufferlen, &bytesread, NULL);
    while (result && bytesread != 0)
    {
        WriteFile(outHandle, buffer, strlen(buffer), &bytesread, NULL);
        result = ReadFile(fileHandle, buffer, bufferlen, &bytesread, NULL);
    }

    ReadFile(outHandle, buffer, bufferlen, &bytesread, NULL);
    cout << buffer << endl;

    return 0;
}