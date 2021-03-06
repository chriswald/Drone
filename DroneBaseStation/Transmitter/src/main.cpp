#include "main.h"
#include <SensorPins.h>
#include <Radio.h>

Radio *radio;
const int packetBufferSize = 32;
uint8_t* packetBuffer;

uint8_t* radioSendBuffer;
uint8_t* radioReceiveBuffer;

// the setup routine runs once when you press reset:
void setup() {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    packetBuffer = (uint8_t*)calloc(packetBufferSize, sizeof(uint8_t));
    radioSendBuffer = (uint8_t*)calloc(packetBufferSize, sizeof(uint8_t));
    radioReceiveBuffer = (uint8_t*)calloc(packetBufferSize, sizeof(uint8_t));

    radio = new Radio();
    radio->config("clie1", "serv1", RADIO_CE, RADIO_CSN);
    radio->configInterrupt(ServerMessageReceived, RADIO_IRQ, true, false, false);
}

// the loop routine runs over and over again forever:
void loop()
{
    if (Serial.available() > 0)
    {
        size_t bufferPosition = 0;

        while ((Serial.available() > 0) && 
                (bufferPosition < packetBufferSize))
        {
            packetBuffer[bufferPosition] = Serial.read();
            bufferPosition ++;
        }

        char messageType = packetBuffer[0];
        ProcessPacket(messageType, packetBuffer, bufferPosition);
        
        // Clear the buffer
        memset(packetBuffer, 0, packetBufferSize);
    }
}

void ProcessPacket(char messageType, uint8_t* buffer, size_t byteCount)
{
    switch (messageType)
    {
        case 'C':   // Host checking if the device is connected
            ReturnConnectedPacket();
            break;
        default:
            SendRadioMessage(buffer, byteCount);
            break;
    }
}

void ReturnConnectedPacket()
{
    Serial.println("Connected");
}

void ServerMessageReceived()
{
    while (radio->dataReady())
    {
        ReceiveRadioMessage(radioReceiveBuffer);
        Serial.println((char*)radioReceiveBuffer);
    }
}

void SendRadioMessage(uint8_t* buffer, size_t byteCount)
{
    memset(radioSendBuffer, 0, packetBufferSize);
    memcpy(radioSendBuffer, buffer, byteCount);
    radio->send(radioSendBuffer);
}

void ReceiveRadioMessage(uint8_t* buffer)
{
    memset(buffer, 0, packetBufferSize);
    radio->get(buffer);
}