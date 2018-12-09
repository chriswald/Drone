#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;

HANDLE threadHandle;

DWORD WINAPI ReadSerial(void* data)
{
    HANDLE radioHandle = *(HANDLE*) data;
    char result[1024];
    DWORD charsread;
    while (true)
    {
        ReadFile(radioHandle, result, 1024, &charsread, NULL);
        printf("%.*s", charsread, result);
    }
}

void createThread(HANDLE* radioHandle)
{
    threadHandle = CreateThread(
        NULL,
        0,
        ReadSerial,
        radioHandle,
        0,
        NULL);
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
    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutConstant = 1;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 1;
    timeouts.WriteTotalTimeoutMultiplier = 1;

    if (!SetCommTimeouts(*handle, &timeouts))
    {
        cout << "Error setting timeouts" << endl;
        return false;
    }
}

int main(int argc, char** argv)
{
    /*if (argc < 3)
    {
        cout << "Usage: SerialSendFile <com number> <filename>" << endl;
        return -1;
    }*/
    int commnum = 3;// atoi(argv[1]);
    char* filename = "test.txt";// argv[2];

    HANDLE outHandle;

    ifstream fin;
    fin.open(filename);
    if (!fin.is_open())
    {
        cout << "Could not open file: " << filename << endl;
        getchar();
        return -1;
    }

    bool success = openComm(&outHandle, commnum);

    if (!success)
    {
        getchar();
        return -1;
    }

    PurgeComm(outHandle, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_RXABORT);

    createThread(&outHandle);

    string fileline;

    while (fin.good())
    {
        std::getline(fin, fileline);
        cout << fileline << endl;
        WriteFile(outHandle, fileline.c_str(), fileline.size(), NULL, NULL);        
    }

    getchar();
    return 0;
}