#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>

void ProcessPacket(char messageType, byte* buffer, byte byteCount);
void ReturnConnectedPacket();

#endif // __MAIN_H__