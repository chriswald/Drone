#include <SPI.h>
#include <DroneRadio.h>

DroneRadio client;

typedef unsigned long UINT32;
typedef float FLOAT32;

bool SendAndWaitForResponse(Packet*, Packet*, long = 1000);
UINT32 toInt(const char* s, int slen, int& read_len);
FLOAT32 toFloat(const char* s, int slen, int& read_len);

void setup()  
{
  Serial.begin(9600);
  client.config("clie1", "serv1");
  Serial.println("Begin");
  Serial.print("> ");
}

char buffer[100];
byte index = 0;
#define BUFFER_IS(x) strcmp(buffer, x)==0
#define BUFFER_BEGINS_WITH(x) strncmp(x, buffer, strlen(x))==0

void loop()
{
  if (!Serial.available()) return;
  
  clearRadioBuffer();
  
  while (Serial.available())
  {
    buffer[index++] = Serial.read();
    delay(2);
  }
  
  buffer[index] = '\0';
  
  if (BUFFER_BEGINS_WITH("ping"))
    pingServer();
  else if (BUFFER_BEGINS_WITH("location"))
    printGPSData();
  else if (BUFFER_BEGINS_WITH("speed"))
    printGPSSpeed();
  else if (BUFFER_BEGINS_WITH("satellites"))
    printGPSSatellites();
  else if (BUFFER_BEGINS_WITH("fix"))
    printGPSFix();
  else if (BUFFER_BEGINS_WITH("orientation"))
    getOrientation();
  else if (BUFFER_BEGINS_WITH("gyroscope"))
    getGyroscope();
  else if (BUFFER_BEGINS_WITH("accelerometer"))
    getAccelerometer();
  else if (BUFFER_BEGINS_WITH("magnetometer"))
    getMagnetometer();
  else if (BUFFER_BEGINS_WITH("pressure"))
    getPressure();
  else if (BUFFER_BEGINS_WITH("temperature"))
    getTemperature();
  /*else if (BUFFER_BEGINS_WITH("add"))
    sendWaypoint();
  else if (BUFFER_BEGINS_WITH("num"))
    getNumWaypoints();
  else if (BUFFER_BEGINS_WITH("clear"))
    clearSystemRoute();
  else if (BUFFER_BEGINS_WITH("remove"))
    removeSystemWaypoint();
  else if (BUFFER_BEGINS_WITH("get"))
    getSystemWaypoint();
  else if (BUFFER_BEGINS_WITH("insert"))
    insertSystemWaypoint();*/
  else if (BUFFER_BEGINS_WITH("gmotor"))
    motorSpeed('g');
  else if (BUFFER_BEGINS_WITH("smotor"))
    motorSpeed('s');
  else if (BUFFER_BEGINS_WITH("BEGIN"))
    systemBeginRoute();
  else if (BUFFER_BEGINS_WITH("grecords"))
    showFlightRecords('g');
  else if (BUFFER_BEGINS_WITH("drecords"))
    showFlightRecords('d');
  else if (BUFFER_BEGINS_WITH("spid"))
    PIDValues('s');
  else if (BUFFER_BEGINS_WITH("gpid"))
    PIDValues('g');
  else if (BUFFER_BEGINS_WITH("carm"))
    armMotors('c');
  else if (BUFFER_BEGINS_WITH("garm"))
    armMotors('g');
  else if (BUFFER_BEGINS_WITH("rearm"))
    rearmMotors();
  else if (BUFFER_BEGINS_WITH("stime"))
    testFlightTime('s');
  else if (BUFFER_BEGINS_WITH("gtime"))
    testFlightTime('g');
  else if (BUFFER_BEGINS_WITH("test"))
    startTestFlight();
  else if (BUFFER_BEGINS_WITH("joy"))
    joystick();
  
  index = 0;
  buffer[index] = '\0';
  
  Serial.print("> ");
}

void clearRadioBuffer()
{
  if (client.dataReady())
  {
    Packet p;
    client.get(p);
  }
}

#define LAT(x) ((float*)x.data)[0]
#define LON(x) ((float*)x.data)[1]
#define ALT(x) ((float*)x.data)[2]
#define SPEED(x) ((float*)x.data)[0]

void printGPSData()
{
  Serial.println("Contacting server for Lat, Lon, and Alt");
  Packet p;
  p.setAction(ACTION_GPS_LAT_LON_ALT);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    float data[3];
    p.getDataArray(data, sizeof(data));
    Serial.print("Latitude:  ");
    Serial.println(data[0], 8);
    Serial.print("Longitude: ");
    Serial.println(data[1], 8);
    Serial.print("Altitude:  ");
    Serial.println(data[2], 1);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void printGPSSpeed()
{
  Serial.println("Contacting server for speed");
  Packet p;
  p.setAction(ACTION_GPS_SPEED);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    float speed;
    p.getDataArray(&speed, sizeof(speed));
    Serial.print("Speed:  ");
    Serial.print(speed, 3);
    Serial.println(" knots");
  }
  else
  {
    Serial.println("Timeout");
  }
}

void printGPSSatellites()
{
  Serial.println("Contacting server for number of satellites");
  Packet p;
  p.setAction(ACTION_GPS_SATELLITES);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    uint8_t num;
    p.getDataArray(&num, sizeof(num));
    Serial.print("Satellites: ");
    Serial.println(num);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void printGPSFix()
{
  Serial.println("Contacting server for fix status");
  Packet p;
  p.setAction(ACTION_GPS_FIX);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    uint8_t data;
    p.getDataArray(&data, sizeof(data));
    Serial.print("Fix: ");
    Serial.println((data?"Yes":"No"));
  }
  else
  {
    Serial.println("Timeout");
  }
}

void pingServer()
{
  Serial.println("Pinging server");
  Packet p;
  p.setAction(ACTION_PING);
  p.setMethod(METHOD_COMMAND);
  unsigned long data = millis();
  p.setDataArray(&data, sizeof(data));
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    unsigned long time;
    p.getDataArray(&time, sizeof(time));
    Serial.print("Ping: ");
    Serial.println(millis()-time);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void getOrientation()
{
  Serial.println("Requesting Orientation");
  Packet p;
  p.setAction(ACTION_IMU_ORIENTATION);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    float data[3];
    p.getDataArray(data, sizeof(data));
    
    Serial.print("Roll:    ");
    Serial.println(data[0]);
    Serial.print("Pitch:   ");
    Serial.println(data[1]);
    Serial.print("Heading: ");
    Serial.println(data[2]);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void getGyroscope()
{
  Serial.println("Requesting Gyroscope");
  Packet p;
  p.setAction(ACTION_IMU_GYRO);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    float data[3];
    p.getDataArray(data, sizeof(data));
    
    Serial.print("X: ");
    Serial.print(data[0]);
    Serial.print(" Y: ");
    Serial.print(data[1]);
    Serial.print(" Z: ");
    Serial.println(data[2]);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void getAccelerometer()
{
  Serial.println("Requesting Accelerometer");
  Packet p;
  p.setAction(ACTION_IMU_ACCEL);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    float data[3];
    p.getDataArray(data, sizeof(data));
    
    Serial.print("X: ");
    Serial.print(data[0]);
    Serial.print(" Y: ");
    Serial.print(data[1]);
    Serial.print(" Z: ");
    Serial.println(data[2]);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void getMagnetometer()
{
  Serial.println("Requesting Magnetometer");
  Packet p;
  p.setAction(ACTION_IMU_MAG);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    float data[3];
    p.getDataArray(data, sizeof(data));
    
    Serial.print("X: ");
    Serial.print(data[0]);
    Serial.print(" Y: ");
    Serial.print(data[1]);
    Serial.print(" Z: ");
    Serial.println(data[2]);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void getPressure()
{
  Serial.println("Requesting Pressure");
  Packet p;
  p.setAction(ACTION_IMU_PRESSURE);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    float pressure;
    p.getDataArray(&pressure, sizeof(pressure));
    
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");
  }
  else
  {
    Serial.println("Timeout");
  }
}

void getTemperature()
{
  Serial.println("Requesting Temperature");
  Packet p;
  p.setAction(ACTION_IMU_TEMPERATURE);
  p.setMethod(METHOD_GET);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    float temperature;
    p.getDataArray(&temperature, sizeof(temperature));
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C, ");
    Serial.print(temperature * 9/5 + 32);
    Serial.println(" F");
  }
  else
  {
    Serial.println("Timeout");
  }
}

/*void sendWaypoint()
{
  Serial.println("Sending a waypoint to the system.");
  Packet p;
  p.typeID = PACKET_ADD_WAYPOINT;
  
  int read_len;
  index = strlen("add");
  
  float lat = toFloat(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  float lon = toFloat(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  float alt = toFloat(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  
  ((float*)p.data)[0] = lat;
  ((float*)p.data)[1] = lon;
  ((float*)p.data)[2] = alt;
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    double distance = ((double*)p.data)[0];
    
    Serial.print("Total route distance: ");
    Serial.print(distance);
    Serial.println("m");
  }
  else
  {
    Serial.println("Timeout");
  }
}

void getNumWaypoints()
{
  Serial.println("Getting number of waypoints.");
  Packet p;
  p.typeID = PACKET_GET_NUM_WAYPOINTS;
 
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    byte num = p.data[0];
    
    Serial.print("Number of waypoints: ");
    Serial.println(num);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void clearSystemRoute()
{
  Serial.println("Clearing the system route.");
  Packet p;
  p.typeID = PACKET_CLEAR_ROUTE;
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    if (p.typeID == PACKET_CLEAR_ROUTE)
    {
      Serial.println("Route Cleared.");
    }
  }
  else
  {
    Serial.println("Timeout");
  }
}

void removeSystemWaypoint()
{
  Serial.println("Removing a waypoint.");
  int index = atoi(&buffer[sizeof("remove")]);  
  Packet p;
  p.typeID = PACKET_REMOVE_WAYPOINT;
  p.data[0] = (byte)index;
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    if (*(int*)p.data)
      Serial.println("Waypoint removed.");
    else
      Serial.println("Waypoint NOT removed.");
  }
  else
  {
    Serial.println("Timeout");
  }
}

void getSystemWaypoint()
{
  Serial.println("Getting system waypoint.");
  int index = atoi(&buffer[sizeof("get")]);
  Packet p;
  p.typeID = PACKET_GET_WAYPOINT;
  p.data[0] = (byte)index;
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    float lat = ((float*)p.data)[0];
    float lon = ((float*)p.data)[1];
    float alt = ((float*)p.data)[2];
    
    Serial.print("Waypoint ");
    Serial.print(index);
    Serial.println(":");
    Serial.print("Latitude:  ");
    Serial.println(lat, 8);
    Serial.print("Longitude: ");
    Serial.println(lon, 8);
    Serial.print("Altitude:  ");
    Serial.println(alt, 1);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void insertSystemWaypoint()
{
  Serial.println("Inserting system waypoint.");
  Packet p;
  p.typeID = PACKET_INSERT_WAYPOINT;
  
  int read_len;
  index = strlen("insert");
  
  float lat = toFloat(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  float lon = toFloat(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  float alt = toFloat(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  int ind = toInt(&buffer[index], strlen(&buffer[index]), read_len);
  
  ((float*)p.data)[0] = lat;
  ((float*)p.data)[1] = lon;
  ((float*)p.data)[2] = alt;
  ((float*)p.data)[3] = (float)ind;
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    double distance = ((double*)p.data)[0];
    
    Serial.print("Total route distance: ");
    Serial.print(distance);
    Serial.println("m");
  }
  else
  {
    Serial.println("Timeout");
  }
}*/

void systemBeginRoute()
{
  Packet p;
  p.setAction(ACTION_BEGIN);
  p.setMethod(METHOD_COMMAND);
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    uint8_t data;
    p.getDataArray(&data, sizeof(data));
    if (data)
    {
      Serial.println();
      Serial.println("-----------------------------------------------------");
      Serial.println("--- SYSTEM BEGINNING PREFLIGHT ROUTINE            ---");
      Serial.println("--- PLEASE STAND BACK, TAKEOFF WILL BEGIN SHORTLY ---");
      Serial.println("-----------------------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("System is not ready for preflight.");
    }
  }
  else
  {
    Serial.println("Timeout");
  }
}

void showFlightRecords(char c)
{
  Serial.println("Retrieving flight records.");
  Packet p;
  p.setAction(ACTION_FLIGHT_RECORDS);
  if (c == 'g')
    p.setMethod(METHOD_GET);
  else if (c == 'd')
    p.setMethod(METHOD_DELETE);
  else
    return;
    
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    long time;
    float data[3];
    p.getDataArray(&time, sizeof(time));
    p.getDataArray(data, sizeof(data), sizeof(time));
    
    Serial.print("Flight Time: ");
    Serial.print(time/1000.0);
    Serial.println(" s");
    
    Serial.print("Max Speed:   ");
    Serial.print(data[0]);
    Serial.println(" knots");
    
    Serial.print("Max Alt:     ");
    Serial.print(data[1]);
    Serial.println(" m");
    
    Serial.print("Max AOG:     ");
    Serial.print(data[2]);
    Serial.println(" m");
  }
  else
  {
    Serial.println("Timeout");
  }
}

void motorSpeed(char c)
{
  Serial.println("Motor speed");
  Packet p;
  p.setAction(ACTION_MOTOR_SPEED);
  if (c == 'g')
    p.setMethod(METHOD_GET);
  else if (c == 's')
    p.setMethod(METHOD_SET);
  else
    return;

  uint32_t data[2] = {0, 0};

  int read_len;
  index = strlen("xmotor");
  data[0] = toInt(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;

  if (c =='s') {
    data[1] = toInt(&buffer[index], strlen(&buffer[index]), read_len);
  }

  p.setDataArray(data, sizeof(data));
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    uint32_t num, speed;
    p.getDataArray(&num, sizeof(num));
    p.getDataArray(&speed, sizeof(speed), sizeof(num));
    Serial.print("motor[");
    Serial.print(num);
    Serial.print("] = ");
    Serial.println(speed);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void PIDValues(char c)
{
  Packet p;
  p.setAction(ACTION_PID);
  if (c == 's')
    p.setMethod(METHOD_SET);
  else if (c == 'g')
    p.setMethod(METHOD_GET);
  else
    return;
  
  uint32_t pindex = 0;
  float data[3] = {0, 0, 0};
  
  index = strlen("xpid");
  int read_len = 0;
  pindex = toInt(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  
  if (c == 's') {
    data[0] = toFloat(&buffer[index], strlen(&buffer[index]), read_len);
    index += read_len;
    data[1] = toFloat(&buffer[index], strlen(&buffer[index]), read_len);
    index += read_len;
    data[2] = toFloat(&buffer[index], strlen(&buffer[index]), read_len);
  }
  
  p.setDataArray(&pindex, sizeof(pindex));
  p.setDataArray(data, sizeof(data), sizeof(pindex));
  
  if (c == 's')
    Serial.print("Updating PID");
  else
    Serial.print("Retreiving PID");
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    if (p.getError() == NO_ERROR)
    {
      p.getDataArray(&pindex, sizeof(pindex));
      p.getDataArray(data, sizeof(data), sizeof(pindex));
      
      Serial.print("[");
      Serial.print(pindex);
      Serial.print("] to P:");
      Serial.print(data[0]);
      Serial.print(" I:");
      Serial.print(data[1]);
      Serial.print(" D:");
      Serial.print(data[2]);
      Serial.println();
    }
    else if (p.getError() == ERROR_INVALID_INDEX)
    {
      Serial.print("\nERROR_INVALID_INDEX: ");
      Serial.println(pindex);
    }
  }
  else
  {
    Serial.println(" ...Timeout");
  }
}

void armMotors(char c)
{
  Serial.println("Arming motors.");
  Packet p;
  p.setAction(ACTION_ARM);
  if (c == 'c')
    p.setMethod(METHOD_COMMAND);
  else if (c == 'g')
    p.setMethod(METHOD_GET);
  else
    return;
  
  bool timeout = SendAndWaitForResponse(&p, &p, 8000);
  
  if (!timeout)
  {
    uint8_t ready;
    p.getDataArray(&ready, sizeof(ready));
    if (ready)
      Serial.println("Motors armed.");
    else
      Serial.println("Motors NOT armed.");
  }
  else
  {
    Serial.println("Timeout");
  }
}

void rearmMotors()
{
  Serial.println("Re-Arming motors.");
  Packet p;
  p.setAction(ACTION_REARM);
  p.setMethod(METHOD_COMMAND);
  
  bool timeout = SendAndWaitForResponse(&p, &p, 8000);

  if (!timeout)
  {
    uint8_t ready;
    p.getDataArray(&ready, sizeof(ready));
    if (ready)
      Serial.println("Motors armed.");
    else
      Serial.println("Motors NOT armed.");
  }
  else
  {
    Serial.println("Timeout");
  }
}

void testFlightTime(char c)
{
  Serial.println("Setting new value for test flight duration.");
  Packet p;
  p.setAction(ACTION_TEST_DURATION);
  if (c == 's')
    p.setMethod(METHOD_SET);
  else if (c == 'g')
    p.setMethod(METHOD_GET);
  else
    return;
  
  uint32_t time = 0;
  
  if (c == 's') {
    index = strlen("xtime");
    int read_len = 0;
    time = toInt(&buffer[index], strlen(&buffer[index]), read_len);
    index += read_len;
  }
  
  p.setDataArray(&time, sizeof(time));
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    p.getDataArray(&time, sizeof(time));
    Serial.print("Test flight duration = ");
    Serial.println(time);
  }
  else
  {
    Serial.println("Timeout");
  }
}

void startTestFlight()
{
  Serial.println("Starting new test flight.");
  Packet p;
  p.setAction(ACTION_TEST);
  p.setMethod(METHOD_COMMAND);
  
  index = strlen("test");
  int read_len = 0;
  UINT32 dspeed = toInt(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  UINT32 nomspeed = toInt(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  
  p.setDataArray(&dspeed, sizeof(dspeed));
  p.setDataArray(&nomspeed, sizeof(nomspeed), sizeof(dspeed));
  
  bool timeout = SendAndWaitForResponse(&p, &p);
  
  if (!timeout)
  {
    Serial.println("Test flight started.");
  }
  else
  {
    Serial.println("Timeout");
  }
}

void joystick()
{
  pinMode(10, OUTPUT);
  
  digitalWrite(10,HIGH);
  index = strlen("joy");
  int read_len = 0;
  UINT32 val = toInt(&buffer[index], strlen(&buffer[index]), read_len);
  index += read_len;
  
  Packet p;
  p.setAction(ACTION_MOTOR_SPEED);
  p.setMethod(METHOD_SET);
  
  uint32_t data[2] = {0, 0};
  data[1] = val;
  
  data[0] = 0;
  p.setDataArray(data, sizeof(data));
  client.send(p);
  
  data[0] = 1;
  p.setDataArray(data, sizeof(data));
  client.send(p);
  
  data[0] = 2;
  p.setDataArray(data, sizeof(data));
  client.send(p);
  
  data[0] = 3;
  p.setDataArray(data, sizeof(data));
  client.send(p);
  
  digitalWrite(10,LOW);
}

/*
 * Sends send_packet, then waits for the number of milliseconds specified by wait.
 * If a reply was received before the timeout it gets stored in read_packet and
 * false is returned. Otherwise nothing is stored in read_packet and true is
 * returned.
 */
bool SendAndWaitForResponse(Packet* send_packet, Packet* read_packet, long wait)
{
  client.send(*send_packet);
  unsigned long time = millis();
  bool timeout = false;
  while(!client.dataReady() && !timeout)
    timeout = millis()-time > wait;
  
  if (!timeout)
  {
    client.get(*read_packet);
    return false;
  }
  else
  {
    return true;
  }
}

FLOAT32 toFloat(const char* s, int slen, int& read_len)
{
  char snum[slen];
  int si = 0, ni = 0;
  while (s[si] == ' ') si ++;

  while (si < slen && s[si] != ' ' &&
         s[si] != '\0' && s[si] != '\n')
  {
    snum[ni++] = s[si++];
  }
  
  snum[ni] = '\0';
  
  FLOAT32 f = atof(snum);
  read_len = si;
  return f;
}

UINT32 toInt(const char* s, int slen, int& read_len)
{
  char snum[slen];
  int si = 0, ni = 0;
  while (s[si] == ' ') si ++;

  while (si < slen && s[si] != ' ' &&
         s[si] != '\0' && s[si] != '\n')
  {
    snum[ni++] = s[si++];
  }
  
  snum[ni] = '\0';
  
  UINT32 i = atol(snum);
  read_len = si;
  return i;
}
