#include <SPI.h>
#include <Radio.h>
#include <avr/interrupt.h>
#include <avr/io.h>

Radio client;

byte consec_packets_dropped = 0;
bool last_ping_ackd = true;

ISR(TIMER1_COMPA_vect) {
  if (!last_ping_ackd)
  {
    consec_packets_dropped ++;
    Serial.println("Missed ping");
    if (consec_packets_dropped > 4)
    {
      transferToFallbackChannel();
      consec_packets_dropped = 0;
    }
  }
  sendPing();
};

void setup() {
  Serial.begin(9600);
  
  client.config("clie1", "serv1");
  client.configInterrupt(messageReceived, 0, true, false, false);
  
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();
  
  delay(100);
}

void loop() {
  if (client.dataReady())
  {
    Packet p;
    client.get(p);
    if (p.typeID != PACKET_PING)
    {
      return;
    }
    
    last_ping_ackd = true;
    consec_packets_dropped = 0;
    Serial.print("Ping: ");
    Serial.println(millis()-*(unsigned long*)p.data);
    
    while (client.dataReady())
      client.get(p);
  }
}

void messageReceived()
{
  
}

void sendPing()
{
  Serial.println("Send Ping");
  Packet p = PingPacket();
  client.send(p);
  last_ping_ackd = false;
}

void transferToFallbackChannel()
{
  Serial.println("Transfer to fallback channel");
}
