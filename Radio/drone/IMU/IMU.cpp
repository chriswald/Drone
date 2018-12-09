#include "IMU.h"

#define RPH_dr      0.138
#define RPH_dp      2.04
#define RPH_dh      0
#define GYRO_dx     -0.00851
#define GYRO_dy     0.01368
#define GYRO_dz     -0.0093
#define ACCEL_dx    0.2225
#define ACCEL_dy    0.0040
#define ACCEL_dz    0.125
#define MAG_dx      0
#define MAG_dy      0
#define MAG_dz      0

IMU::IMU() : sea_level_pressure(SEA_LEVEL_PRESSURE), calibrated(false), dof(), gyro(20), accel(30301), mag(30302), bmp(18001)
{
}

bool IMU::begin()
{
    return gyro.begin() && accel.begin() && mag.begin() && bmp.begin();
}

void IMU::getRPH(float& roll, float& pitch, float& heading)
{
    getRPH_raw(roll, pitch, heading);
    roll    += RPH_dr;
    pitch   += RPH_dp;
    heading += RPH_dh;
}

void IMU::getRPH_raw(float& roll, float& pitch, float& heading)
{
    accel.getEvent(&event);
    sensors_vec_t orientation;
    dof.accelGetOrientation(&event, &orientation);
    roll = orientation.roll;
    pitch = orientation.pitch;
    mag.getEvent(&event);
    dof.magGetOrientation(SENSOR_AXIS_Z, &event, &orientation);
    heading = orientation.heading;
}

void IMU::getGyroscope(float& x, float& y, float& z)
{
    getGyroscope_raw(x, y, z);
    x += GYRO_dx;
    y += GYRO_dy;
    z += GYRO_dz;
}

void IMU::getGyroscope_raw(float& x, float& y, float& z)
{
    gyro.getEvent(&event);
    x = event.gyro.x;
    y = event.gyro.y;
    z = event.gyro.z;
}

void IMU::getAccelerometer(float& x, float& y, float& z)
{
    getAccelerometer_raw(x, y, z);
    x += ACCEL_dx;
    y += ACCEL_dy;
    z += ACCEL_dz;
}

void IMU::getAccelerometer_raw(float& x, float& y, float& z)
{
    accel.getEvent(&event);
    x = event.acceleration.x;
    y = event.acceleration.y;
    z = event.acceleration.z;
}

void IMU::getMagnetometer(float& x, float& y, float& z)
{
    getMagnetometer_raw(x, y, z);
    x += MAG_dx;
    y += MAG_dy;
    z += MAG_dz;
}

void IMU::getMagnetometer_raw(float& x, float& y, float& z)
{
    mag.getEvent(&event);
    x = event.magnetic.x;
    y = event.magnetic.y;
    z = event.magnetic.z;
}

void IMU::getTemperature(float& temperature)
{
    bmp.getTemperature(&temperature);
}

void IMU::getAltitude(float& altitude)
{
    noInterrupts();
    bmp.getEvent(&event);
    float temp;
    getTemperature(temp);
    altitude = bmp.pressureToAltitude(sea_level_pressure, event.pressure, temp);
    interrupts();
}

void IMU::getPressure(float& pressure)
{
    bmp.getEvent(&event);
    pressure = event.pressure;
}

void IMU::calibrateAltitude(float altitude)
{
    noInterrupts();
    float cur_pres, cur_temp;
    getPressure(cur_pres);
    getTemperature(cur_temp);
    sea_level_pressure = bmp.seaLevelForAltitude(altitude, cur_pres, cur_temp);
    calibrated = true;
    interrupts();
}

bool IMU::isCalibrated()
{
    return calibrated;
}
