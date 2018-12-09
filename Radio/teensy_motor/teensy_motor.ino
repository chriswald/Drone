#define TIMER_REFRESH 5000 // us

IntervalTimer speedTimer;
IntervalTimer stopSpeedTimer;
int speed = 0;
int mindex = 0;
int sindex = 0;

struct Motor {
  int pin;
  int speed;
};

Motor motors[4];

void stopSpeed()
{
  stopSpeedTimer.end();
  digitalWrite(motors[mindex].pin, LOW);
  mindex = (mindex + 1) % 4;
}

void setSpeed()
{
  digitalWrite(motors[mindex].pin, HIGH);
  if (motors[mindex].speed != 0) {
    stopSpeedTimer.begin(stopSpeed, motors[mindex].speed);
  } else {
    mindex = (mindex + 1) % 4;
  }
}

void armMotors()
{
  Serial.println("--- ARMING MOTORS ---");
  speedTimer.end();  
  
  for (int arming_time = 0; arming_time < 50 * 5; arming_time ++)
  {
    digitalWrite(motors[0].pin, HIGH);
    digitalWrite(motors[1].pin, HIGH);
    digitalWrite(motors[2].pin, HIGH);
    digitalWrite(motors[3].pin, HIGH);
    delayMicroseconds(1100);
    digitalWrite(motors[0].pin, LOW);
    digitalWrite(motors[1].pin, LOW);
    digitalWrite(motors[2].pin, LOW);
    digitalWrite(motors[3].pin, LOW);
    delay(20-(1100/1000));
  }
  
  motors[0].speed = 1100;
  motors[1].speed = 1100;
  motors[2].speed = 1100;
  motors[3].speed = 1100;
  
  speedTimer.begin(setSpeed, TIMER_REFRESH);
  Serial.println("--- Done --- ");
}

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  motors[0].pin = 20;
  motors[0].speed = 0;
  motors[1].pin = 21;
  motors[1].speed = 0;
  motors[2].pin = 22;
  motors[2].speed = 0;
  motors[3].pin = 23;
  motors[3].speed = 0;
  
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  speedTimer.begin(setSpeed, TIMER_REFRESH);
}

void loop()
{
  int new_speed = 0;
  bool read_new_speed = false;
  while(Serial.available())
  {
    char c = Serial.read();
    if (c >= '0' && c <= '9') {
      new_speed = new_speed * 10 + (c - '0');
      read_new_speed = true;
    }
    else if (c == 'a') {
      armMotors();
    }
    else if (c == 's') {
      char n[2] = {Serial.read(), '\0'};
      sindex = atoi(n) % 4;
      Serial.print("Sindex: ");
      Serial.println(sindex);
      Serial.print("Speed:  ");
      Serial.println(motors[sindex].speed);
    }
    else {
      new_speed = 1100; // The "stop rotation" speed
      read_new_speed = true;
    }
  }
  
  if (read_new_speed)
  {
    if (new_speed > 2000) {
      new_speed = 1100;
      motors[0].speed = new_speed;
      motors[1].speed = new_speed;
      motors[2].speed = new_speed;
      motors[3].speed = new_speed;
    }
    
    noInterrupts();
    motors[sindex].speed = new_speed;
    interrupts();
    
    Serial.print("Speed:  ");
    Serial.println(motors[sindex].speed);
  }
}
