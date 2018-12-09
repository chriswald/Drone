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
 */

#include <SPI.h>
#include <Radio.h>
#include <BattMon.h>

#define NUM_CHANNELS 84
#define FALLBACK_CHANNEL 32
byte cur_channel = FALLBACK_CHANNEL;
byte channels[NUM_CHANNELS];

int CONSECUTIVE_DROPPED_PACKETS = 0;

Radio client;

void setup(){
  Serial.begin(9600);
  client.config("clie1", "serv1");
  client.setChannel(32);
  for (int i = 0; i < NUM_CHANNELS; i ++)
    channels[i] = 0;
  channels[cur_channel] = 1;
  Serial.println("Beginning ... "); 
}

void loop(){
  static unsigned long time = 0;
  
  /*if (millis() - time > 1000) {
    if (interferenceOnChannel())
      transitionChannel();
    if (!pingServer()) {
      Serial.println("*** Periodic Ping Failed");
      cur_channel = FALLBACK_CHANNEL;
      ChangeChannel(cur_channel);
    }
    time = millis();
  }*/
  
  if (!Serial.available()) return;
  
  /*if (CONSECUTIVE_DROPPED_PACKETS > 5)
    reconfigureRadio();*/

  if (Serial.peek() == 'b') {
    Serial.read();
    sendBatteryVoltage();
  }

  if (Serial.peek() == 'p') {
    Serial.read();
    sendPing();
  }
  
  if (Serial.peek() == 'l') {
    Serial.read();
    sendLEDCommand();
  }
  
  if (Serial.peek() == 'r') {
    Serial.read();
    reconfigureRadio();
  }
  
  if (Serial.peek() == 't') {
    Serial.read();
    transitionChannel();
  }
}

void reconfigureRadio()
{
  Serial.print("Reconfiguring Radio...");
  client.setChannel(32);
  Packet p;
  p.typeID = PACKET_RECONFIGURE;
  client.send((byte*)&p);
  while (client.isSending()) ;
  
  bool ok = waitForResponse();
  if (ok) {
    Serial.println("DONE");
  } else {
    Serial.println("FAIL");
  }
}

bool interferenceOnChannel()
{
  return client.channelHasInterference();
}

void transitionChannel()
{
  Serial.println("--- Begin Channel Transition ---");
  bool done = false;
  for (byte i = 0; i < NUM_CHANNELS && !done; i ++)
  {
    client.setChannel(i);
    if (interferenceOnChannel())
    {
      channels[i] = 1;
    }
    else
    {
      client.setChannel(cur_channel);
      if (notifyServerOfNewChannel(i))
      {
        client.setChannel(i);
        delay(500); // Give the server time to switch
        Serial.println("Ping server");
        if (pingServer())
        {
          cur_channel = i;
          channels[i] = 1;
          done = true;
          break;
        } else {
          client.setChannel(cur_channel);
        }
      }
    }
  }
  
  if (done) {
    Serial.println("--- Transition SUCCESSFUL ---");
  }
  else {
    Serial.println("--- Transition FAILURE ---");
    for (int i = 0; i < NUM_CHANNELS; i ++)
      channels[i] = 0;
    channels[cur_channel] = 1;
    client.setChannel(cur_channel);
  }
}

bool notifyServerOfNewChannel(byte next_channel)
{
  Serial.println("Notify server of intent to change channel.");
  Packet notification;
  notification.typeID = PACKET_CHANNEL_TRANSITION;
  *notification.data = next_channel;
  for (byte retries = 0; retries < 3; retries ++)
  {
    client.send(notification);
    unsigned long time = millis();
    bool timeout = false;
    while (!client.dataReady() && !timeout) {
      if (millis() - time > 1000)
        timeout = true;
    }
    
    if (!timeout) {
      client.get(notification);
      if (notification.typeID == PACKET_CHANNEL_TRANSITION
          && *notification.data == next_channel) {
        Serial.println("*** Successful Notification");
        return true;
      }
      
      if (notification.typeID != PACKET_CHANNEL_TRANSITION)
        Serial.println("*** Notification type ID FAILED");
      if (*notification.data != next_channel)
        Serial.println("*** Nofification data FAILED");
      delay(500);
    }
  }
  
  Serial.println("*** Too many notification retries");
  return false;
}

bool pingServer()
{
  bool timeout = false;
  unsigned long pkt_time = millis();
  
  Packet ping;
  ping.typeID = PACKET_PING;
  *(unsigned long*)ping.data = pkt_time;
  for (byte retries = 0; retries < 3; retries ++)
  {
    client.send(ping);
    unsigned long time = millis();
    while (!client.dataReady() && !timeout) {
      if (millis() - time > 1000)
        timeout = true;
    }
    
    if (!timeout) {
      return true;
    }
  }
  
  Serial.println("*** Too many ping retries");
  return false;
}
  

void sendBatteryVoltage()
{
  BattMon monitor(A0, 1.2);
  float percent = monitor.getPercent();
  Packet packet;
  packet.typeID = PACKET_BATTERY_LEVEL;
  *(float*)packet.data = percent;
  Serial.print("Battery Voltage Percent: ");
  Serial.println(*(float*)packet.data);
  client.send(packet);
  
  waitForResponse();
}

void sendPing()
{
  unsigned long time = millis();
  Packet packet;
  packet.typeID = PACKET_PING;
  *(unsigned long*)packet.data = time;
  client.send(packet);
  
  waitForResponse();
  
  client.get(packet);
  if (packet.typeID == PACKET_PING)
  {
    time = *(unsigned long*)packet.data;
    Serial.print("Ping: ");
    Serial.println(millis() - time);
  }
  else
  {
    Serial.print("Invalid return packet type: ");
    switch(packet.typeID) {
    case PACKET_BATTERY_LEVEL:
      Serial.println("PACKET_BATTERY_LEVEL");
      break;
    case PACKET_PING:
      Serial.println("PACKET_PING");
      break;
    case PACKET_LED_COMMAND:
      Serial.println("PACKET_LED_COMMAND");
      break;
    default:
      Serial.print("UNKNOWN - ");
      Serial.println(packet.typeID);
      break;
    }
  }
}

void sendLEDCommand()
{
  while (Serial.peek() == ' ') Serial.read();
  int index = 0;
  Packet packet;
  packet.typeID = PACKET_LED_COMMAND;
  while (Serial.available() && index < 31)
    packet.data[index++] = (byte)Serial.read();
  packet.data[index] = '\0';
  Serial.print("LED command: ");
  Serial.println((char*)packet.data);
  client.send(packet);
    
  waitForResponse();
}

bool waitForResponse()
{
  unsigned long time = millis();
  while(!client.dataReady()){
    if ( ( millis() - time ) > 1000 ) {
      CONSECUTIVE_DROPPED_PACKETS ++;
      return false;
    }
  }
  CONSECUTIVE_DROPPED_PACKETS = 0;
  return true;
}
