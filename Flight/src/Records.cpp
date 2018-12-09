#include "Records.h"
#include <Arduino.h>
#include "Storage.h"

Records RECORDS;

void Records::recordStartTime()
{
    startTime = millis();
}

void Records::recordEndTime()
{
    endTime = millis();
}

long Records::getFlightTimeMillis()
{
    if (startTime == 0)
        return 0;
    if (endTime == 0)
        return millis() - startTime;
    else
        return endTime - startTime;
}

void Records::recordSpeed(float speed)
{
    if (speed > max_speed)
    {
        max_speed = speed;
    }
}

float Records::getMaxSpeed()
{
    return max_speed;
}

void Records::recordAltitude(float altitude)
{
    if (altitude > max_altitude)
    {
        max_altitude = altitude;
    }
}

void Records::setBaseAltitude(float altitude)
{
    base_altitude = altitude;
}

float Records::getMaxAltitude()
{
    return max_altitude;
}

float Records::getMaxAltitudeOffGround()
{
    return max_altitude - base_altitude;
}

void Records::saveRecords()
{
    long duration = getFlightTimeMillis();
    storage_write(TIME_ADDR, &duration, sizeof(duration));

    float speed = getMaxSpeed();
    storage_write(SPEED_ADDR, &speed, sizeof(speed));

    float alt = getMaxAltitude();
    storage_write(ALT_ADDR, &alt, sizeof(alt));

    float aog = getMaxAltitudeOffGround();
    storage_write(AOG_ADDR, &aog, sizeof(aog));
}

void Records::loadRecords()
{
    storage_read(TIME_ADDR, &endTime, sizeof(endTime));
    endTime += startTime;

    storage_read(SPEED_ADDR, &max_speed, sizeof(max_speed));

    storage_read(ALT_ADDR, &max_altitude, sizeof(max_altitude));

    storage_read(AOG_ADDR, &base_altitude, sizeof(base_altitude));
    base_altitude = max_altitude - base_altitude;
}

void Records::clear()
{
    storage_erase(RECORDS_MIN_ADDR, RECORDS_STORAGE_SIZE);
    loadRecords();
}