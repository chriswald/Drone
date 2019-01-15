// Test application that receives messages from the radio and writes
// them out to serial.

#include <Arduino.h>
#include <SensorPins.h>
#include <Radio.h>

void ClientMessageReceived();
void SendString(char *message);
void ReceiveString(char *message);

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
        char message[32];
        ReceiveString(message);
        Serial.print("Test Receiver received: ");
        Serial.println(message);
    }
}

// Read a data packet from the radio and convert it into a string
void ReceiveString(char *message)
{
    uint8_t data[32];
    radio->get(data);
    memcpy(message, data, 32);
}