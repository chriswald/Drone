#include "Radio.h"
#include "nRF24L01.h"

#define RADIO_PAYLOAD_SIZE 32

void Radio::config(const char* rx, const char* tx, byte ce, byte csn, byte chan)
{
    channel = chan;
    NRF.spi = &hardwareSPI;
    NRF.cePin = ce;
    NRF.csnPin = csn;
    NRF.channel = chan;
    NRF.init();
    NRF.setRADDR((byte *)rx);
    NRF.setTADDR((byte *)tx);
    NRF.payload = RADIO_PAYLOAD_SIZE;
    NRF.config();
    NRF.configRegister(RF_SETUP, RADIO_250KBPS_MODE);
    delay(100);
}

void Radio::configInterrupt(void(*func)(void), byte pin, bool rx, bool tx, bool max_rt)
{
    pinMode(pin, INPUT);
    attachInterrupt(pin, func, FALLING);

    byte reg;
    readRegister(CONFIG, &reg, sizeof(byte));
    reg &= ~(0x111<<4);
    reg |= (((!rx)<<2)|((!tx)<<1)|(!max_rt))<<4;
    writeRegister(CONFIG, &reg, sizeof(byte));
}

void Radio::send(byte* packet)
{
    noInterrupts();
    NRF.send(packet);
    while (NRF.isSending()) ;
    interrupts();
}

void Radio::get(byte* packet)
{
    NRF.getData(packet);
}

bool Radio::dataReady()
{
    uint8_t status = NRF.getStatus();
    if ( status & (1 << RX_DR) ) return 1;
    return !NRF.rxFifoEmpty();
}

bool Radio::isSending()
{
    return NRF.isSending();
}

void Radio::setChannel(byte chan)
{
    channel = chan;
    NRF.channel = channel;
    NRF.configRegister(RF_CH,channel);
}

void Radio::setTransferSpeed(byte speed)
{
    NRF.configRegister(RF_SETUP, speed);
}

bool Radio::channelHasInterference(byte chan)
{
    if (chan != -1)
    {
        NRF.channel = chan;
        NRF.config();
    }

    byte val;
    byte interference;
    NRF.readRegister(CD, &val, sizeof(byte));
    interference = val & 1;
    if (chan != -1)
    {
        NRF.channel = channel;
        NRF.config();
    }

    return interference;
}

void Radio::readRegister(uint8_t reg, uint8_t * value, uint8_t len)
{
    NRF.readRegister(reg, value, len);
}

void Radio::writeRegister(uint8_t reg, uint8_t * value, uint8_t len)
{
    NRF.writeRegister(reg, value, len);
}
