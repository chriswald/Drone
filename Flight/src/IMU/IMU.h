#ifndef __IMU_H__
#define __IMU_H__

#include <Arduino.h>

#include <Wire.h>
#include "Adafruit_Sensor/Adafruit_Sensor.h"
#include "Adafruit_L3GD20_U/Adafruit_L3GD20_U.h"
#include "Adafruit_LSM303DLHC/Adafruit_LSM303_U.h"
#include "Adafruit_BMP085_Unified/Adafruit_BMP085_U.h"
#include "Adafruit_10DOF/Adafruit_10DOF.h"

#define SEA_LEVEL_PRESSURE SENSORS_PRESSURE_SEALEVELHPA

class IMU
{
public:
    IMU();

    bool begin();

    void getRPH(float& roll, float& pitch, float& heading);
    void getRPH_raw(float& roll, float& pitch, float& heading);
    void getGyroscope(float& x, float& y, float& z);
    void getGyroscope_raw(float& x, float& y, float& z);
    void getAccelerometer(float& x, float& y, float& z);
    void getAccelerometer_raw(float& x, float& y, float& z);
    void getMagnetometer(float& x, float& y, float& z);
    void getMagnetometer_raw(float& x, float& y, float& z);
    void getTemperature(float& temperature);
    void getAltitude(float& altitude);
    void getPressure(float& pressure);

    void calibrateAltitude(float altitude);

    bool isCalibrated();

private:
    float sea_level_pressure;
    bool calibrated;

    Adafruit_10DOF dof;
    Adafruit_L3GD20_Unified gyro;
    Adafruit_LSM303_Accel_Unified accel;
    Adafruit_LSM303_Mag_Unified mag;
    Adafruit_BMP085_Unified bmp;
    sensors_event_t event;
};

#endif // __IMU_H__