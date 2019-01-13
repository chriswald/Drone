#ifndef __RADIO_COMMON_H__
#define __RADIO_COMMON_H__

#include <Arduino.h>

#include "nRF24L01.h"

#define RADIO_250KBPS_MODE 0b00100110
#define RADIO_1MBPS_MODE   0b00000110
#define RADIO_2MBPS_MODE   0b00001110

class Radio {
public:
    void config(const char* rx, const char* tx, byte ce=8, byte csn=7, byte channel=0);
    void configInterrupt(void(*func)(void), byte pin, bool rx, bool tx, bool max_rt);
    void send(byte* packet);
    void get(byte* packet);
    bool dataReady();
    bool isSending();
    void setChannel(byte channel);
    void setTransferSpeed(byte speed);
    bool channelHasInterference(byte channel=-1);

    void readRegister(uint8_t reg, uint8_t * value, uint8_t len);
    void writeRegister(uint8_t reg, uint8_t * value, uint8_t len);

private:
    byte channel;
};

class HardwareSPI {
public:
    uint8_t transfer(uint8_t data);
    void begin();
    void end();
};

class nrf24l01p {
public:
    nrf24l01p();

    void init();
    void config();
    void send(uint8_t *value);
    void setRADDR(uint8_t * adr);
    void setTADDR(uint8_t * adr);
    bool dataReady();
    bool isSending();
    bool rxFifoEmpty();

    void getData(uint8_t * data);
    uint8_t getStatus();

    void transmitSync(uint8_t *dataout,uint8_t len);
    void transferSync(uint8_t *dataout,uint8_t *datain,uint8_t len);
    void configRegister(uint8_t reg, uint8_t value);
    void readRegister(uint8_t reg, uint8_t * value, uint8_t len);
    void writeRegister(uint8_t reg, uint8_t * value, uint8_t len);
    void powerUpRx();
    void powerUpTx();
    void powerDown();

    void csnHi();
    void csnLow();

    void ceHi();
    void ceLow();
    void flushRx();

    uint8_t PTX;
    uint8_t cePin;
    uint8_t csnPin;
    uint8_t channel;
    uint8_t payload;
    HardwareSPI *spi;
private:
};

extern nrf24l01p NRF;
extern HardwareSPI hardwareSPI;

#endif
