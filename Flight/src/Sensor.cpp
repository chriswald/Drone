#include "Sensor.h"

bool initGPS();
bool initIMU();
bool initBattMon();

TinyGPSPlus gps;
IMU imu;
BattMon battmon;

bool initSensors()
{
    bool success = true;
    success = success && initGPS();
    success = success && initIMU();
    success = success && initBattMon();
    return success;
}

bool initGPS()
{
    GPS_SERIAL.begin(9600);
    GPS_SERIAL.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS_SERIAL.println(PMTK_SET_NMEA_UPDATE_1HZ);
    GPS_SERIAL.println(PGCMD_ANTENNA);
    return true;
}

bool initIMU()
{
    return imu.begin();
}

bool initBattMon()
{
    battmon.config(BATT_MON1, 5.0);
    return true;
}

void loopGPS()
{
    while (GPS_SERIAL.available())
        gps.encode(GPS_SERIAL.read());
}
