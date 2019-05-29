#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>

void ProcessPacket(char messageType, byte* buffer, size_t byteCount);
void SendRadioMessage(byte* buffer, size_t byteCount);
void ReturnConnectedPacket();
void ServerMessageReceived();

void SendString(char *message);
void ReceiveString(char *message);

#endif // __MAIN_H__