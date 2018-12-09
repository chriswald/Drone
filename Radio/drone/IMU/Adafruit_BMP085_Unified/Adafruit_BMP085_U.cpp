/***************************************************************************
  This is a library for the BMP085 pressure sensor

  Designed specifically to work with the Adafruit BMP085 or BMP180 Breakout 
  ----> http://www.adafruit.com/products/391
  ----> http://www.adafruit.com/products/1603
 
  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include <math.h>
#include <limits.h>

#include "Adafruit_BMP085_U.h"

static bmp085_calib_data _bmp085_coeffs;   // Last read accelerometer data will be available here
static uint8_t           _bmp085Mode;

#define BMP085_USE_DATASHEET_VALS (0) /* Set to 1 for sanity check */

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C
*/
/**************************************************************************/
static void writeCommand(byte reg, byte value)
{
  Wire.beginTransmission((uint8_t)BMP085_ADDRESS);
  #if ARDUINO >= 100
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
  #else
    Wire.send(reg);
    Wire.send(value);
  #endif
  Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
static void read8(byte reg, uint8_t *value)
{
  Wire.beginTransmission((uint8_t)BMP085_ADDRESS);
  #if ARDUINO >= 100
    Wire.write((uint8_t)reg);
  #else
    Wire.send(reg);
  #endif
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)BMP085_ADDRESS, (byte)1);
  #if ARDUINO >= 100
    *value = Wire.read();
  #else
    *value = Wire.receive();
  #endif  
  Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C
*/
/**************************************************************************/
static void read16(byte reg, uint16_t *value)
{
  Wire.beginTransmission((uint8_t)BMP085_ADDRESS);
  #if ARDUINO >= 100
    Wire.write((uint8_t)reg);
  #else
    Wire.send(reg);
  #endif
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)BMP085_ADDRESS, (byte)2);
  #if ARDUINO >= 100
    *value = (Wire.read() << 8) | Wire.read();
  #else
    *value = (Wire.receive() << 8) | Wire.receive();
  #endif  
  Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C
*/
/**************************************************************************/
static void readS16(byte reg, int16_t *value)
{
  uint16_t i;
  read16(reg, &i);
  *value = (int16_t)i;
}

/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
static void readCoefficients(void)
{
  #if BMP085_USE_DATASHEET_VALS
    _bmp085_coeffs.ac1 = 408;
    _bmp085_coeffs.ac2 = -72;
    _bmp085_coeffs.ac3 = -14383;
    _bmp085_coeffs.ac4 = 32741;
    _bmp085_coeffs.ac5 = 32757;
    _bmp085_coeffs.ac6 = 23153;
    _bmp085_coeffs.b1  = 6190;
    _bmp085_coeffs.b2  = 4;
    _bmp085_coeffs.mb  = -32768;
    _bmp085_coeffs.mc  = -8711;
    _bmp085_coeffs.md  = 2868;
    _bmp085Mode        = 0;
  #else
    readS16(BMP085_REGISTER_CAL_AC1, &_bmp085_coeffs.ac1);
    readS16(BMP085_REGISTER_CAL_AC2, &_bmp085_coeffs.ac2);
    readS16(BMP085_REGISTER_CAL_AC3, &_bmp085_coeffs.ac3);
    read16(BMP085_REGISTER_CAL_AC4, &_bmp085_coeffs.ac4);
    read16(BMP085_REGISTER_CAL_AC5, &_bmp085_coeffs.ac5);
    read16(BMP085_REGISTER_CAL_AC6, &_bmp085_coeffs.ac6);
    readS16(BMP085_REGISTER_CAL_B1, &_bmp085_coeffs.b1);
    readS16(BMP085_REGISTER_CAL_B2, &_bmp085_coeffs.b2);
    readS16(BMP085_REGISTER_CAL_MB, &_bmp085_coeffs.mb);
    readS16(BMP085_REGISTER_CAL_MC, &_bmp085_coeffs.mc);
    readS16(BMP085_REGISTER_CAL_MD, &_bmp085_coeffs.md);
  #endif
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
static void readRawTemperature(int32_t *temperature)
{
  #if BMP085_USE_DATASHEET_VALS
    *temperature = 27898;
  #else
    uint16_t t;
    writeCommand(BMP085_REGISTER_CONTROL, BMP085_REGISTER_READTEMPCMD);
    delay(5);
    read16(BMP085_REGISTER_TEMPDATA, &t);
    *temperature = t;
  #endif
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
static void readRawPressure(int32_t *pressure)
{
  #if BMP085_USE_DATASHEET_VALS
    *pressure = 23843;
  #else
    uint8_t  p8;
    uint16_t p16;
    int32_t  p32;

    writeCommand(BMP085_REGISTER_CONTROL, BMP085_REGISTER_READPRESSURECMD + (_bmp085Mode << 6));
    switch(_bmp085Mode)
    {
      case BMP085_MODE_ULTRALOWPOWER:
        delay(5);
        break;
      case BMP085_MODE_STANDARD:
        delay(8);
        break;
      case BMP085_MODE_HIGHRES:
        delay(14);
        break;
      case BMP085_MODE_ULTRAHIGHRES:
      default:
        delay(26);
        break;
    }

    read16(BMP085_REGISTER_PRESSUREDATA, &p16);
    p32 = (uint32_t)p16 << 8;
    read8(BMP085_REGISTER_PRESSUREDATA+2, &p8);
    p32 += p8;
    p32 >>= (8 - _bmp085Mode);
    
    *pressure = p32;
  #endif
}

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/
 
/**************************************************************************/
/*!
    @brief  Instantiates a new Adafruit_BMP085_Unified class
*/
/**************************************************************************/
Adafruit_BMP085_Unified::Adafruit_BMP085_Unified(int32_t sensorID) {
  _sensorID = sensorID;
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/
 
/**************************************************************************/
/*!
    @brief  Setups the HW
*/
/**************************************************************************/
bool Adafruit_BMP085_Unified::begin(bmp085_mode_t mode)
{
  // Enable I2C
  Wire.begin();

  /* Mode boundary check */
  if ((mode > BMP085_MODE_ULTRAHIGHRES) || (mode < 0))
  {
    mode = BMP085_MODE_ULTRAHIGHRES;
  }

  /* Make sure we have the right device */
  uint8_t id;
  read8(BMP085_REGISTER_CHIPID, &id);
  if(id != 0x55)
  {
    return false;
  }

  /* Set the mode indicator */
  _bmp085Mode = mode;

  /* Coefficients need to be read once */
  readCoefficients();
    
  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the compensated pressure level in kPa
*/
/**************************************************************************/
void Adafruit_BMP085_Unified::getPressure(float *pressure)
{
  int32_t  ut = 0, up = 0, compp = 0;
  int32_t  x1, x2, b5, b6, x3, b3, p;
  uint32_t b4, b7;

  /* Get the raw pressure and temperature values */
  readRawTemperature(&ut);
  readRawPressure(&up);

  /* Temperature compensation */
  x1 = (ut - (int32_t)(_bmp085_coeffs.ac6))*((int32_t)(_bmp085_coeffs.ac5))/pow(2,15);
  x2 = ((int32_t)(_bmp085_coeffs.mc*pow(2,11)))/(x1+(int32_t)(_bmp085_coeffs.md));
  b5 = x1 + x2;

  /* Pressure compensation */
  b6 = b5 - 4000;
  x1 = (_bmp085_coeffs.b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = (_bmp085_coeffs.ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((int32_t) _bmp085_coeffs.ac1) * 4 + x3) << _bmp085Mode) + 2) >> 2;
  x1 = (_bmp085_coeffs.ac3 * b6) >> 13;
  x2 = (_bmp085_coeffs.b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (_bmp085_coeffs.ac4 * (uint32_t) (x3 + 32768)) >> 15;
  b7 = ((uint32_t) (up - b3) * (50000 >> _bmp085Mode));

  if (b7 < 0x80000000)
  {
    p = (b7 << 1) / b4;
  }
  else
  {
    p = (b7 / b4) << 1;
  }

  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  compp = p + ((x1 + x2 + 3791) >> 4);

  /* Assign compensated pressure value */
  *pressure = compp;
}

/**************************************************************************/
/*!
    @brief  Reads the temperatures in degrees Celsius
*/
/**************************************************************************/
void Adafruit_BMP085_Unified::getTemperature(float *temp)
{
  int32_t UT, X1, X2, B5;     // following ds convention
  float t;

  readRawTemperature(&UT);

  #if BMP085_USE_DATASHEET_VALS
    // use datasheet numbers!
    UT = 27898;
    _bmp085_coeffs.ac6 = 23153;
    _bmp085_coeffs.ac5 = 32757;
    _bmp085_coeffs.mc = -8711;
    _bmp085_coeffs.md = 2868;
  #endif

  // step 1
  X1 = (UT - (int32_t)_bmp085_coeffs.ac6) * ((int32_t)_bmp085_coeffs.ac5) / pow(2,15);
  X2 = ((int32_t)_bmp085_coeffs.mc * pow(2,11)) / (X1+(int32_t)_bmp085_coeffs.md);
  B5 = X1 + X2;
  t = (B5+8)/pow(2,4);
  t /= 10;

  *temp = t;
}

/**************************************************************************/
/*!
    Calculates the altitude (in meters) from the specified atmospheric
    pressure (in hPa), sea-level pressure (in hPa), and temperature (in �C)

    @param  seaLevel      Sea-level pressure in hPa
    @param  atmospheric   Atmospheric pressure in hPa
    @param  temp          Temperature in degrees Celsius
*/
/**************************************************************************/
float Adafruit_BMP085_Unified::pressureToAltitude(float seaLevel, float atmospheric, float temp)
{
  /* Hyposometric formula:                      */
  /*                                            */
  /*     ((P0/P)^(1/5.257) - 1) * (T + 273.15)  */
  /* h = -------------------------------------  */
  /*                   0.0065                   */
  /*                                            */
  /* where: h   = height (in meters)            */
  /*        P0  = sea-level pressure (in hPa)   */
  /*        P   = atmospheric pressure (in hPa) */
  /*        T   = temperature (in �C)           */

  return (((float)pow((seaLevel/atmospheric), 0.190223F) - 1.0F)
         * (temp + 273.15F)) / 0.0065F;
}

float Adafruit_BMP085_Unified::seaLevelForAltitude(float altitude, float atmospheric, float temp)
{
  /* Hyposometric formula:                      */
  /*                                            */
  /* P0=((((h*0.0065)/(temp + 273.15F))+1)^(1.0/0.190223F))*P */
  /*                                            */
  /* where: h   = height (in meters)            */
  /*        P0  = sea-level pressure (in hPa)   */
  /*        P   = atmospheric pressure (in hPa) */
  /*        T   = temperature (in �C)           */
  
  return (float)pow((((altitude*0.0065)/(temp + 273.15F))+1), (1.0/.190223F))*atmospheric;
}



/**************************************************************************/
/*!
    @brief  Provides the sensor_t data for this sensor
*/
/**************************************************************************/
void Adafruit_BMP085_Unified::getSensor(sensor_t *sensor)
{
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "BMP085", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _sensorID;
  sensor->type        = SENSOR_TYPE_PRESSURE;
  sensor->min_delay   = 0;
  sensor->max_value   = 300.0F;               // 300..1100 hPa
  sensor->min_value   = 1100.0F;
  sensor->resolution  = 0.01F;                // Datasheet states 0.01 hPa resolution
}

/**************************************************************************/
/*!
    @brief  Reads the sensor and returns the data as a sensors_event_t
*/
/**************************************************************************/
void Adafruit_BMP085_Unified::getEvent(sensors_event_t *event)
{
  float pressure_kPa;

  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type      = SENSOR_TYPE_PRESSURE;
  event->timestamp = 0;
  getPressure(&pressure_kPa);
  event->pressure = pressure_kPa / 100.0F;
}
