/**
 * A Mirf example to test the latency between two Ardunio.
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
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load 
 * 'ping_server_interupt' on the server.
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define LED_PIN 0
#define RADIO_250KBPS_MODE 0b00100110
#define RADIO_1MBPS_MODE   0b00000110
#define RADIO_2MBPS_MODE   0b00001110


void setup(){
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"clie1");   
  Mirf.payload = sizeof(unsigned long);
  Mirf.configRegister(RF_SETUP, RADIO_250KBPS_MODE);
  Mirf.config();
  pinMode(LED_PIN, OUTPUT);
  delay(100);
}

void loop(){
  unsigned long time = millis();
  
  Mirf.setTADDR((byte *)"serv1");
  
  Mirf.send((byte *)&time);
  
  while(Mirf.isSending()){
  }
  while(!Mirf.dataReady()){
    if ( ( millis() - time ) > 1000 ) {
      digitalWrite(LED_PIN, HIGH);
      return;
    }
  }
  
  Mirf.getData((byte *) &time);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
} 
  
  
  
