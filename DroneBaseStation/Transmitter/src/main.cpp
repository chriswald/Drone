#include <Arduino.h>

const int packetBufferSize = 256;
byte* packetBuffer;

// the setup routine runs once when you press reset:
void setup() {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    packetBuffer = (byte*)calloc(packetBufferSize, sizeof(byte));
}

// the loop routine runs over and over again forever:
void loop() {
    byte incomingByte = 0;
    byte byteCount = 0;
    byte bufferPosition = 0;

    if (Serial.available() > 0)
    {
        byteCount = Serial.read();
        packetBuffer[bufferPosition++] = byteCount;

        for (byte i = bufferPosition; i < byteCount; i ++)
        {
            if (Serial.available() <= 0) { break; }

            incomingByte = Serial.read();
            packetBuffer[bufferPosition++] = incomingByte;
        }

        packetBuffer[byteCount] = 0;
    }

    // Echo the block back and reset the buffer
    if (bufferPosition > 0)
    {
        for (byte i = 0; i < byteCount; i ++)
        {
            Serial.print((char)packetBuffer[i]);
        }

        Serial.flush();
        while (Serial.available() > 0) { Serial.read(); }

        memset(packetBuffer, 0, bufferPosition);
    }
}