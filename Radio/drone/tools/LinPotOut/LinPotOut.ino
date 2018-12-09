#include <Esplora.h>
int LEDpin = 13;
int brightness = 0;
int direction = 1;

void setup() {
  pinMode(LEDpin, OUTPUT);
}

void loop() {
  analogWrite(LEDpin, 
    map(brightness, 0, 1023, 0, 255));
  brightness += direction;
  if (brightness < 0)
  {
    brightness = 0;
    direction *= -1;
  }
  if (brightness > 1023)
  {
    brightness = 255;
    direction *= -1;
  }
  
  int slideval = 1023 - Esplora.readSlider();
  Serial.print(",");
  Serial.print(slideval);
}
