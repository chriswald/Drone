#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>

void ProcessPacket(char messageType, byte* buffer, byte byteCount);
void SendRadioMessage(byte* buffer, byte byteCount);
void ReturnConnectedPacket();
void ServerMessageReceived();

#endif // __MAIN_H__