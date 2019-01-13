#include "Radio.h"
#include "nRF24L01.h"

#define mirf_ADDR_LEN   5
#define mirf_CONFIG ((1<<EN_CRC) | (0<<CRCO) )
nrf24l01p::nrf24l01p() {
    cePin = 8;
    csnPin = 7;
    channel = 1;
    payload = 16;
    spi = NULL;
}

void nrf24l01p::init() {
    pinMode(cePin,OUTPUT);
    pinMode(csnPin,OUTPUT);

    ceLow();
    csnHi();
    spi->begin();
}
void nrf24l01p::config() {
    configRegister(RF_CH,channel);
    configRegister(RX_PW_P0, payload);
    configRegister(RX_PW_P1, payload);
    powerUpRx();
    flushRx();
}
void nrf24l01p::send(uint8_t *value) {
    uint8_t status;
    status = getStatus();

    while (PTX) {
        status = getStatus();

        if((status & ((1 << TX_DS)  | (1 << MAX_RT)))){
            PTX = 0;
            break;
        }
    }

    ceLow();

    powerUpTx();

    csnLow();
    spi->transfer( FLUSH_TX );
    csnHi();

    csnLow();
    spi->transfer( W_TX_PAYLOAD );
    transmitSync(value,payload);
    csnHi();

    ceHi();
}
void nrf24l01p::setRADDR(uint8_t * adr) {
    ceLow();
    writeRegister(RX_ADDR_P1,adr,mirf_ADDR_LEN);
    ceHi();
}
void nrf24l01p::setTADDR(uint8_t * adr){
    writeRegister(RX_ADDR_P0,adr,mirf_ADDR_LEN);
    writeRegister(TX_ADDR,adr,mirf_ADDR_LEN);
}
bool nrf24l01p::dataReady() {
    uint8_t status = getStatus();

    if ( status & (1 << RX_DR) ) return 1;
    return !rxFifoEmpty();
}
bool nrf24l01p::isSending() {
    uint8_t status;
    if(PTX){
        status = getStatus();
        if((status & ((1 << TX_DS)  | (1 << MAX_RT)))){
            powerUpRx();
            return false;
        }

        return true;
    }
    return false;
}
bool nrf24l01p::rxFifoEmpty() {
    uint8_t fifoStatus;

    readRegister(FIFO_STATUS,&fifoStatus,sizeof(fifoStatus));
    return (fifoStatus & (1 << RX_EMPTY));
}

void nrf24l01p::getData(uint8_t * data) {
    csnLow();
    spi->transfer( R_RX_PAYLOAD );
    transferSync(data,data,payload);
    csnHi();
    configRegister(STATUS,(1<<RX_DR));
}
uint8_t nrf24l01p::getStatus() {
    uint8_t rv;
    readRegister(STATUS,&rv,1);
    return rv;
}

void nrf24l01p::transmitSync(uint8_t *dataout,uint8_t len){
    uint8_t i;
    for(i = 0;i < len;i++){
        spi->transfer(dataout[i]);
    }
}
void nrf24l01p::transferSync(uint8_t *dataout,uint8_t *datain,uint8_t len){
    uint8_t i;
    for(i = 0;i < len;i++){
        datain[i] = spi->transfer(dataout[i]);
    }
}
void nrf24l01p::configRegister(uint8_t reg, uint8_t value) {
    csnLow();
    spi->transfer(W_REGISTER | (REGISTER_MASK & reg));
    spi->transfer(value);
    csnHi();
}
void nrf24l01p::readRegister(uint8_t reg, uint8_t * value, uint8_t len) {
    csnLow();
    spi->transfer(R_REGISTER | (REGISTER_MASK & reg));
    transferSync(value,value,len);
    csnHi();
}
void nrf24l01p::writeRegister(uint8_t reg, uint8_t * value, uint8_t len) {
    csnLow();
    spi->transfer(W_REGISTER | (REGISTER_MASK & reg));
    transmitSync(value,len);
    csnHi();
}
void nrf24l01p::powerUpRx() {
    PTX = 0;
    ceLow();
    configRegister(CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (1<<PRIM_RX) ) );
    ceHi();
    configRegister(STATUS,(1 << TX_DS) | (1 << MAX_RT));
}
void nrf24l01p::powerUpTx() {
    PTX = 1;
    configRegister(CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (0<<PRIM_RX) ) );
}
void nrf24l01p::powerDown() {
    ceLow();
    configRegister(CONFIG, mirf_CONFIG );
}

void nrf24l01p::csnHi() {
    digitalWrite(csnPin,HIGH);
}
void nrf24l01p::csnLow() {
    digitalWrite(csnPin,LOW);
}

void nrf24l01p::ceHi() {
    digitalWrite(cePin,HIGH);
}
void nrf24l01p::ceLow() {
    digitalWrite(cePin,LOW);
}
void nrf24l01p::flushRx() {
    csnLow();
    spi->transfer( FLUSH_RX );
    csnHi();
}

nrf24l01p NRF;
