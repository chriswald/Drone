#include "main.h"
#include <SensorPins.h>
#include <Radio.h>

void SendString(char *message);
void ReceiveString(char *message);

Radio *radio;
const int packetBufferSize = 256;
byte* packetBuffer;

// the setup routine runs once when you press reset:
void setup() {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    packetBuffer = (byte*)calloc(packetBufferSize, sizeof(byte));

    radio = new Radio();
    radio->config("clie1", "serv1", RADIO_CE, RADIO_CSN);
    radio->configInterrupt(ServerMessageReceived, RADIO_IRQ, true, false, false);
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
            SendRadioMessage(buffer, byteCount);
            break;
        case 'C':   // Host checking if the device is connected
            ReturnConnectedPacket();
            break;
        default:    // Anything else? Ignore the packet
            break;
    }
}

void SendRadioMessage(byte* buffer, byte byteCount)
{
    char message[] = "message";
    uint8_t *data = (uint8_t*)calloc(0, sizeof(uint8_t) * 32);
    memcpy(data, message, strlen(message) + 1);
    radio->send(data);
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
        Serial.print("Teensy received: ");
        Serial.println(message);
    }
}

void SendString(char *message)
{
    uint8_t *data = (uint8_t*)calloc(0, sizeof(uint8_t) * 32);
    memcpy(data, message, strlen(message) + 1);
    radio->send(data);
}

void ReceiveString(char *message)
{
    uint8_t data[32];
    radio->get(data);
    memcpy(message, data, 32);
}