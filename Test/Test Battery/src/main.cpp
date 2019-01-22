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

// Set up the serial channel and radio
void setup() 
{
    Serial.begin(9600);

    radio = new Radio();
    radio->config("serv1", "clie1", RADIO_CE, RADIO_CSN);
    radio->configInterrupt(ClientMessageReceived, RADIO_IRQ, true, false, false);
}

// Do nothing
void loop() 
{
    delay(10);
}

// When a message is received write it to serial
void ClientMessageReceived()
{
    while (radio->dataReady())
    {
        uint8_t data[32];
        ReceiveData(data);

        char message[32];
        DataToString(data, message);
        Serial.print("Test Echo received: ");
        Serial.println(message);

        ProcessMessage(data);
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
    memcpy(message, data, sizeof(uint8_t) * 32);
}

void ProcessMessage(uint8_t *data)
{
    // If a battery level request was received ...
    if (data[0] == 'B')
    {
        uint8_t *returnData = (uint8_t*)calloc(32, sizeof(uint8_t));
        returnData[0] = 'B';

        uint8_t batteryLevel = 0;

        // Check which battery the request is for
        if (data[1] == 1)
        {
            // Return the System battery
            returnData[1] = 1;
            batteryLevel = 10; // Fake a battery level
        }
        else if (data[1] == 2)
        {
            // Return the motor battery
            returnData[1] = 2;
            batteryLevel = 75; // Fake a battery level
        }

        returnData[2] = batteryLevel;
        SendData(returnData);
    }
}