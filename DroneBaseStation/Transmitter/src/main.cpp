#include "main.h"

const int packetBufferSize = 256;
byte* packetBuffer;

// the setup routine runs once when you press reset:
void setup() {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    packetBuffer = (byte*)calloc(packetBufferSize, sizeof(byte));
}

// the loop routine runs over and over again forever:
void loop()
{
    if (Serial.available() > 0)
    {
        char messageType = Serial.read();

        byte bufferPosition = 0;

        while ((Serial.available() > 0) && 
                (bufferPosition < packetBufferSize))
        {
            packetBuffer[bufferPosition] = Serial.read();
            bufferPosition ++;
        }

        ProcessPacket(messageType, packetBuffer, bufferPosition);
        
        // Clear the buffer
        memset(packetBuffer, 0, bufferPosition);
    }
}

void ProcessPacket(char messageType, byte* buffer, byte byteCount)
{
    switch (messageType)
    {
        case 'R':   // Packet to be sent over the radio
            break;
        case 'C':   // Host checking if the device is connected
            ReturnConnectedPacket();
            break;
        default:    // Anything else? Ignore the packet
            break;
    }
}

void ReturnConnectedPacket()
{
    Serial.println("Connected");
}