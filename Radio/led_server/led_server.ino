/**
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
#include <Radio.h>

#define LED_ON() digitalWrite(2, HIGH)
#define LED_OFF() digitalWrite(2, LOW)

#define FALLBACK_CHANNEL 32;
byte cur_channel = FALLBACK_CHANNEL;
unsigned long time_of_last_ping = 0;

Radio server;

void setup(){
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  server.config("serv1", "clie1");
  server.setChannel(32);
  
  Serial.println("Listening...");
  LED_ON();
}

void loop(){  
  Packet packet;
  if(server.dataReady()){
    server.get(packet);
    
    if (packet.typeID == PACKET_BATTERY_LEVEL)
      processBatteryVoltage(packet.data);
      
    if (packet.typeID == PACKET_PING)
      server.send(packet);
      
    if (packet.typeID == PACKET_LED_COMMAND)
      processLEDCommand(packet.data);
    
    if (packet.typeID == PACKET_CHANNEL_TRANSITION)
      reconfigureRadio(packet.data);
  }
}

void reconfigureRadio(byte* data)
{
  Serial.print("Reconfiguring Radio...");
  byte next_channel = *data;
  
  Packet packet;
  packet.typeID = PACKET_CHANNEL_TRANSITION;
  *packet.data = next_channel;
  Serial.print("Sending confirmation for channel ");
  Serial.println(next_channel);
  server.send(packet);
  server.setChannel(next_channel);
}

void processBatteryVoltage(byte* data)
{
  float percent = *(float*)data;
  if (percent < 25.0)
    LED_ON();
  else
    LED_OFF();
  Packet p;
  server.send(p);
}

void processPing(byte* data)
{
  Serial.println("Got ping.");
  time_of_last_ping = millis();
  Packet packet;
  packet.typeID = PACKET_PING;
  memcpy(packet.data, data, 31);
  server.send(packet);
}

void processLEDCommand(byte* data)
{
  if (strcmp((char*)data, "on") == 0)
  {
    digitalWrite(2, HIGH);
  }
  
  if (strcmp((char*)data, "off") == 0)
  {
    digitalWrite(2, LOW);
  }
  
  Packet p;
  server.send(p);
}
