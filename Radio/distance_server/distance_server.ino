/**
 * An Mirf example which copies back the data it recives.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define RADIO_250KBPS_MODE 0b00100110
#define RADIO_1MBPS_MODE   0b00000110
#define RADIO_2MBPS_MODE   0b00001110

void setup(){
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = sizeof(unsigned long);
  Mirf.configRegister(RF_SETUP, RADIO_250KBPS_MODE);
  Mirf.config();
}

void loop(){   
  byte data[Mirf.payload];
  
  if(!Mirf.isSending() && Mirf.dataReady()){
    Mirf.getData(data);
    Mirf.setTADDR((byte *)"clie1");
    Mirf.send(data);
  }
}
