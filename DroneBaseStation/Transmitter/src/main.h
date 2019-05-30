#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>

void ProcessPacket(char messageType, uint8_t* buffer, size_t byteCount);
void SendRadioMessage(uint8_t* buffer, size_t byteCount);
void ReceiveRadioMessage(uint8_t* buffer);
void ReturnConnectedPacket();
void ServerMessageReceived();


#endif // __MAIN_H__