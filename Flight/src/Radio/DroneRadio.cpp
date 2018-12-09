#include "DroneRadio.h"

void DroneRadio::config(const char* rx, const char* tx, uint8_t ce, uint8_t csn, uint8_t channel)
{
    radio.config(rx, tx, ce, csn, channel);
}

void DroneRadio::configInterrupt(void(*func)(void), byte pin, bool rx, bool tx, bool max_rt)
{
    radio.configInterrupt(func, pin, rx, tx, max_rt);
}

void DroneRadio::send(Packet& p)
{
    uint8_t data[PACKET_SIZE];
    p.package(data, NULL);
    radio.send(data);
}

void DroneRadio::get(Packet& p)
{
    uint8_t data[PACKET_SIZE];
    radio.get(data);
    p.unpackage(data, PACKET_SIZE);
}

bool DroneRadio::dataReady()
{
    return radio.dataReady();
}

bool DroneRadio::isSending()
{
    return radio.isSending();
}

void DroneRadio::readRegister(uint8_t reg, uint8_t * value, uint8_t len)
{
    radio.readRegister(reg, value, len);
}