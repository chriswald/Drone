#include <SPI.h>
#include <Radio.h>

#define RADIO_INT_PIN 2

Radio server;

bool drop_packets = false;

bool track_pings = false;
unsigned long time_of_last_ping = 0;

void setup()
{
  Serial.begin(9600);
  
  server.config("serv1", "clie1");
  server.configInterrupt(messageReceived, RADIO_INT_PIN, true, false, false);
  delay(100);
}

void loop()
{
  if (Serial.available())
  {
    if (Serial.read() == 'd')
    {
      drop_packets = !drop_packets;
    }
  }
  
  /*if (track_pings)
  {
    if (millis()-time_of_last_ping > 1100)
    {
      Serial.println("Missed Ping deadline");
      transferToFallbackChannel();
    }
  }*/
}

void transferToFallbackChannel()
{
  Serial.println("Transfer to fallback channel");
}

void messageReceived()
{
  Packet p;
  server.get(p);
  
  if (p.typeID == PACKET_PING)
    sendPingReply(p);
  else
    Serial.println("*** Got packet of non-ping type.");
  
  digitalWrite(RADIO_INT_PIN, HIGH);
}

void sendPingReply(Packet& p)
{
  track_pings = true;
  time_of_last_ping = millis();
  if (!drop_packets)
    server.send(p);
}
