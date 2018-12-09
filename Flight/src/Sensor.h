#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "BattMon/BattMon.h"
#include <IMU.h>
#include "TinyGPSPlus/TinyGPS++.h"

#include "SensorPins.h"
#include "SensorComm.h"

extern TinyGPSPlus gps;
extern IMU imu;
extern BattMon battmon;

bool initSensors();
void loopGPS();

#endif // __SENSOR_H__
