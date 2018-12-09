#include <Windows.h>
#include <iostream>
#include <sstream>
#include <cstdio>

using namespace std;

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main()
{
    // Open serial port
    HANDLE inHandle, outHandle;

    inHandle = CreateFile("\\\\.\\COM8", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    outHandle = CreateFile("\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (inHandle == INVALID_HANDLE_VALUE)
    {
        cout << "Error opening in serial" << endl;
        getchar();
        return -1;
    }

    if (outHandle == INVALID_HANDLE_VALUE)
    {
        cout << "Error opening out serial" << endl;
        getchar();
        return -1;
    }
    
    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);

    if (!GetCommState(inHandle, &serialParams))
    {
        cout << "Error getting in comm state" << endl;
        getchar();
        return -1;
    }

    if (!GetCommState(outHandle, &serialParams))
    {
        cout << "Error getting out comm state" << endl;
        getchar();
        return -1;
    }

    serialParams.BaudRate = CBR_9600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;

    if (!SetCommState(inHandle, &serialParams))
    {
        cout << "Error setting in comm state" << endl;
        getchar();
        return -1;
    }

    if (!SetCommState(outHandle, &serialParams))
    {
        cout << "Error setting out comm state" << endl;
        getchar();
        return -1;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(inHandle, &timeouts))
    {
        cout << "Error setting in timeouts" << endl;
        getchar();
        return -1;
    }

    if (!SetCommTimeouts(outHandle, &timeouts))
    {
        cout << "Error setting out timeouts" << endl;
        getchar();
        return -1;
    }

    int bufferlen = 13;
    char buffer[14];
    DWORD bytesread = 0;
    stringstream ss;
    int val;

    char outbuff[20];

    cout << "Buffer         " << "SubStr  " << "Value  " << "Output" << endl;
    cout << "=============  " << "======  " << "=====  " << "========" << endl;
    cout << "Initializing\r";

    Sleep(3000);
    PurgeComm(inHandle, PURGE_RXCLEAR | PURGE_RXABORT);
    PurgeComm(outHandle, PURGE_TXABORT | PURGE_TXCLEAR);

    while (true)
    {
        ReadFile(inHandle, buffer, bufferlen, &bytesread, NULL);
        buffer[13] = '\0';

        string s(buffer);
        int ind1, ind2;
        ind1 = s.find(",");
        ind2 = s.find(",", ind1+1);
        
        ss << s.substr(ind1+1, ind2-ind1-1);
        
        ss >> val;
        val = map(val, 0, 1023, 1100, 2000); 
        sprintf_s(outbuff, "joy %d", val);

        cout.width(15);
        cout << std::left << buffer;
        cout.width(8);
        cout << std::left << ss.str();
        cout.width(7);
        cout << std::left << val;
        cout.width(7);
        cout << outbuff;
        cout << "\t\t\r";

        WriteFile(outHandle, outbuff, strlen(outbuff), &bytesread, NULL);
        Sleep(200);

        ss.clear();
        ss.str(string());
        PurgeComm(inHandle, PURGE_RXCLEAR | PURGE_RXABORT);
    }

    return 0;
}