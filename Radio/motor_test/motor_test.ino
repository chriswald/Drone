int Arming_Time = 0;
int pin = 9;

//Enter value 1100us to Arm the ESC
//Enter value >= 1200us to run Motor but max 200us
int Pulse = 1000;

void setup() {
  pinMode(pin, OUTPUT);
  
  //Will create a 1100us pulse to arm the ESC
  //The pulse will last for 20ms x 500 counts = 10,000ms
  for (Arming_Time = 0; Arming_Time < 500; Arming_Time += 1)
  {
    digitalWrite(pin, HIGH);
    delayMicroseconds(1100);
    digitalWrite(pin, LOW);
    delay(20-(Pulse/1000));
  }
}

void loop() {
  //Increasing speed
  //for (Pulse = 1150; Pulse <= 1400; Pulse += 1)
  {
    Pulse = 1200;
    digitalWrite(pin, HIGH);
    delayMicroseconds(Pulse);
    digitalWrite(pin, LOW);
    delay(20-(Pulse/1000));
  }
  
  //Decreasing speed
  /*for (Pulse = 1400; Pulse >= 1150; Pulse -= 1)
  {
    digitalWrite(pin, HIGH);
    delayMicroseconds(Pulse);
    digitalWrite(pin, LOW);
    delay(20-(Pulse/1000));
  }*/
}
