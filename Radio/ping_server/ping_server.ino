#include <SPI.h>
#include <Radio.h>
#include "nRF24L01.h"

Radio server;

void setup(){
  Serial.begin(9600);
  server.config("serv1", "clie1");
  Serial.println("Beginning ... ");
  delay(100);
}

void loop(){
  byte data[NRF.payload];
  if(!NRF.isSending() && NRF.dataReady()){
    Serial.println("Got packet");
    NRF.getData(data);
    NRF.send(data);
    Serial.println("Reply sent.");
  }
}
