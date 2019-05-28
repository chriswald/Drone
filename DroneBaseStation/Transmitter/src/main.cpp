#include "main.h"
#include <SensorPins.h>
#include <Radio.h>

void SendString(char *message);
void ReceiveString(char *message);

Radio *radio;
const int packetBufferSize = 32;
uint8_t* packetBuffer;

// the setup routine runs once when you press reset:
void setup() {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    packetBuffer = (uint8_t*)calloc(packetBufferSize, sizeof(uint8_t));

    radio = new Radio();
    radio->config("clie1", "serv1", RADIO_CE, RADIO_CSN);
    radio->configInterrupt(ServerMessageReceived, RADIO_IRQ, true, false, false);
}

// the loop routine runs over and over again forever:
void loop()
{
    if (Serial.available() > 0)
    {
        uint8_t bufferPosition = 0;

        while ((Serial.available() > 0) && 
                (bufferPosition < packetBufferSize))
        {
            packetBuffer[bufferPosition] = Serial.read();
            bufferPosition ++;
        }

        char messageType = packetBuffer[0];
        ProcessPacket(messageType, packetBuffer, bufferPosition);
        
        // Clear the buffer
        memset(packetBuffer, 0, bufferPosition);
    }
}

void ProcessPacket(char messageType, uint8_t* buffer, uint8_t byteCount)
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

void SendRadioMessage(uint8_t* buffer, uint8_t byteCount)
{
    char message[2];
    message[0] = 'B';
    message[1] = 1;
    
    SendString(message);
}

void ReturnConnectedPacket()
{
    Serial.println("Connected");
}

void ServerMessageReceived()
{
    while (radio->dataReady())
    {
        char message[32];
        ReceiveString(message);
        //Serial.print("Teensy received: ");
        Serial.println(message);
    }
}

void SendString(char *message)
{
    uint8_t *data = (uint8_t*)calloc(32, sizeof(uint8_t));
    memcpy(data, message, strlen(message) + 1);
    radio->send(data);
    free(data);
}

void ReceiveString(char *message)
{
    uint8_t data[32];
    radio->get(data);
    memcpy(message, data, 32);
}