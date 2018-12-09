#include <SoftwareSerial.h>
#include <SPI.h>
#include <BattMon.h>
#include <Radio.h>
#include <TinyGPS++.h>
#include <RangeSensor.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_10DOF.h>
#include <IMU.h>

#define INT_PIN 14

#define TRIG_PIN 8
#define ECHO_PIN0 3
#define ECHO_PIN1 4
#define ECHO_PIN2 5
#define ECHO_PIN3 6
#define ECHO_PIN4 7
#define NUM_RANGE_SENSORS 5
#define INVALID_SENSOR_NUM 0xFE
RangeSensor* range_sensor[NUM_RANGE_SENSORS];

#define SERIAL Serial1
TinyGPSPlus GPS;
Radio server;
IMU   imu;

void setup()
{
  Serial.begin(9600);
  configGPS();
  
  server.config("serv1", "clie1", 10, 9);
  
  // Set up an interrupt to listen
  // for incoming packets.
  server.configInterrupt(handleMessage, INT_PIN, true, false, false);
  
  range_sensor[0] = new RangeSensor(TRIG_PIN, ECHO_PIN0);
  range_sensor[1] = new RangeSensor(TRIG_PIN, ECHO_PIN1);
  range_sensor[2] = new RangeSensor(TRIG_PIN, ECHO_PIN2);
  range_sensor[3] = new RangeSensor(TRIG_PIN, ECHO_PIN3);
  range_sensor[4] = new RangeSensor(TRIG_PIN, ECHO_PIN4);
  
  imu.begin();
  
  delay(100);
}

void loop()
{
  while(SERIAL.available())
    GPS.encode(SERIAL.read());
}

void configGPS()
{
  SERIAL.begin(9600);
  SERIAL.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  SERIAL.println(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  SERIAL.println(PGCMD_ANTENNA);
}

void handleMessage()
{
  Packet p;
  server.get(p);
  
  if (p.typeID == PACKET_GPS_LAT_LON_ALT)
    sendGPSLatLonAlt();
    
  else if (p.typeID == PACKET_GPS_SPEED)
    sendGPSSpeed();
    
  else if (p.typeID == PACKET_GPS_SATELLITES)
    sendGPSSatellites();
  
  else if (p.typeID == PACKET_GPS_FIX)
    sendGPSFix();
    
  else if (p.typeID == PACKET_PING)
    server.send(p);
    
  else if (p.typeID == PACKET_DISTANCE)
    sendDistance(p);
  
  else if (p.typeID == PACKET_ORIENTATION)
    sendOrientation();
    
  else if (p.typeID == PACKET_GYROSCOPE)
    sendGyroscope();
    
  else if (p.typeID == PACKET_ACCELEROMETER)
    sendAccelerometer();
    
  else if (p.typeID == PACKET_MAGNETOMETER)
    sendMagnetometer();
    
  else if (p.typeID == PACKET_PRESSURE)
    sendPressure();
  
  else if (p.typeID == PACKET_TEMPERATURE)
    sendTemperature();
    
  // I know that the pin is input,
  // but set it high to clear the
  // interrupt
  digitalWrite(INT_PIN,HIGH);
}

void sendGPSLatLonAlt()
{
  Serial.println("Processing request for Lat, Lon, and Alt");
  Packet p;
  p.typeID = PACKET_GPS_LAT_LON_ALT;
  float altitude;
  imu.getAltitude(altitude);
  
  ((float*)p.data)[0] = GPS.location.lat();
  ((float*)p.data)[1] = GPS.location.lng();
  ((float*)p.data)[2] = GPS.altitude.meters();
  ((float*)p.data)[3] = altitude;
  server.send(p);
}

void sendGPSSpeed()
{
  Serial.println("Processing request for speed");
  Packet p;
  p.typeID = PACKET_GPS_SPEED;
  *(float*)p.data = GPS.speed.knots();
  server.send(p);
}

void sendGPSSatellites()
{
  Serial.println("Processing request for satellites");
  Packet p;
  p.typeID = PACKET_GPS_SATELLITES;
  p.data[0] = GPS.satellites.value();
  server.send(p);
}

void sendGPSFix()
{
  Serial.println("Processing request for fix status");
  Packet p;
  p.typeID = PACKET_GPS_FIX;
  p.data[0] = GPS.fix();
  server.send(p);
}

void sendDistance(Packet packet)
{
  Serial.println("Processing request for sensor distance");
  byte sensor_num = packet.data[0];
  Packet p;
  if (sensor_num < NUM_RANGE_SENSORS)
  {
    *(float*)p.data = range_sensor[sensor_num]->getDistanceCM();
  }
  else
  {
    p.data[0] = INVALID_SENSOR_NUM;
    p.data[1] = sensor_num;
  }
  server.send(p);
}

void sendOrientation()
{
  Serial.println("Processing request for orientation");
  Packet p;
  p.typeID = PACKET_ORIENTATION;
  float roll, pitch, heading;
  imu.getRPH(roll, pitch, heading);
  ((float*)p.data)[0] = roll;
  ((float*)p.data)[1] = pitch;
  ((float*)p.data)[2] = heading;
  server.send(p);
}

void sendGyroscope()
{
  Serial.println("Processing request for gyroscope");
  Packet p;
  p.typeID = PACKET_GYROSCOPE;
  float x, y, z;
  imu.getGyroscope(x, y, z);
  ((float*)p.data)[0] = x;
  ((float*)p.data)[1] = y;
  ((float*)p.data)[2] = z;
  server.send(p);
}

void sendAccelerometer()
{
  Serial.println("Processing request for accelerometer");
  Packet p;
  p.typeID = PACKET_ACCELEROMETER;
  float x, y, z;
  imu.getAccelerometer(x, y, z);
  ((float*)p.data)[0] = x;
  ((float*)p.data)[1] = y;
  ((float*)p.data)[2] = z;
  server.send(p);
}

void sendMagnetometer()
{
  Serial.println("Processing request for magnetometer");
  Packet p;
  p.typeID = PACKET_MAGNETOMETER;
  float x, y, z;
  imu.getMagnetometer(x, y, z);
  ((float*)p.data)[0] = x;
  ((float*)p.data)[1] = y;
  ((float*)p.data)[2] = z;
  server.send(p);
}

void sendPressure()
{
  Serial.println("Processing request for pressure");
  Packet p;
  p.typeID = PACKET_PRESSURE;
  float pressure;
  imu.getPressure(pressure);
  ((float*)p.data)[0] = pressure;
  server.send(p);
}

void sendTemperature()
{
  Serial.println("Processing request for temperature");
  Packet p;
  p.typeID = PACKET_TEMPERATURE;
  float temperature;
  imu.getTemperature(temperature);
  ((float*)p.data)[0] = temperature;
  server.send(p);
}
