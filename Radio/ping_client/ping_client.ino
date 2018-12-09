#include <SPI.h>
#include <Radio.h>
#include "nRF24L01.h"

Radio client;
void setup(){
  Serial.begin(9600);
  client.config("clie1", "serv1");
  Serial.println("Beginning ... ");
  delay(100);
}

void loop(){
  unsigned long time = millis();
  
  Packet p;
  p = PingPacket();
  client.send(p);
  Serial.println("Finished sending");
  while(!client.dataReady()){
    if ( ( millis() - time ) > 1000 ) {
      Serial.println("Timeout on response from server!");
      return;
    }
  }
  
  client.get(p);
  time = *(unsigned long*)p.data;
  
  Serial.print("Ping: ");
  Serial.println((millis() - time));
  
  delay(1000);
} 
  
  
  
