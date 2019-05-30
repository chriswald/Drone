// Test application that responds with the battery level when
// requested

#include <Arduino.h>
#include <SensorPins.h>
#include <Radio.h>

void ClientMessageReceived();
void SendData(uint8_t *data);
void ReceiveData(uint8_t *data);
void ProcessMessage(uint8_t *data);
void DataToString(uint8_t *data, char *message);

Radio *radio;
const int packetBufferSize = 32;

uint8_t* radioSendBuffer;
uint8_t* radioReceiveBuffer;

// Set up the serial channel and radio
void setup() 
{
    Serial.begin(9600);

    radioSendBuffer = (uint8_t*)calloc(packetBufferSize, sizeof(uint8_t));
    radioReceiveBuffer = (uint8_t*)calloc(packetBufferSize, sizeof(uint8_t));

    radio = new Radio();
    radio->config("serv1", "clie1", RADIO_CE, RADIO_CSN);
    radio->configInterrupt(ClientMessageReceived, RADIO_IRQ, true, false, false);
}

void loop() 
{
    // Do nothing
}

// When a message is received write it to serial
void ClientMessageReceived()
{
    while (radio->dataReady())
    {
        memset(radioReceiveBuffer, 0, packetBufferSize);
        ReceiveData(radioReceiveBuffer);

        ProcessMessage(radioReceiveBuffer);
    }
}

void SendData(uint8_t *data)
{
    radio->send(data);
}

void ReceiveData(uint8_t *data)
{
    radio->get(data);
}

void DataToString(uint8_t *data, char *message)
{
    memcpy(message, data, sizeof(uint8_t) * packetBufferSize);
}

void ProcessMessage(uint8_t *data)
{
    memset(radioSendBuffer, 8, packetBufferSize);

    // If a battery level request was received ...
    if (data[0] == 'B')
    {
        radioSendBuffer[0] = 'B';

        uint8_t batteryLevel = 0;

        // Check which battery the request is for
        if (data[1] == 1)
        {
            // Return the System battery
            radioSendBuffer[1] = 1;
            batteryLevel = 10; // Fake a battery level
        }
        else if (data[1] == 2)
        {
            // Return the motor battery
            radioSendBuffer[1] = 2;
            batteryLevel = 75; // Fake a battery level
        }

        radioSendBuffer[2] = batteryLevel;

        SendData(radioSendBuffer);
    }
}