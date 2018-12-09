#ifndef __DRONE_RADIO_H__
#define __DRONE_RADIO_H__

#include "Radio.h"
#include "Packet.h"
#include <stdint.h>

class DroneRadio
{
public:
    void config(const char* rx, const char* tx, byte ce=8, byte csn=7, byte channel=0);
    void configInterrupt(void(*func)(void), byte pin, bool rx, bool tx, bool max_rt);

    void send(Packet& p);
    void get (Packet& p);
    bool dataReady();
    bool isSending();

    void readRegister(uint8_t reg, uint8_t * value, uint8_t len);
private:
    Radio radio;
};

#endif // __DRONE_RADIO_H__