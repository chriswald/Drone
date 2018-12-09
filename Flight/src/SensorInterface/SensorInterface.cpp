#include "SensorInterface.h"
#include <MotorController.h>
#include <Sensor.h>
#include <State.h>
#include <stdint.h>

typedef bool (*GetSensorValueFuncType)(SensorValue*);
typedef bool (*SetSensorValueFuncType)(SensorValue*);

bool invalidSensorType(SensorValue*);

bool getMotorSpeed(SensorValue*);
bool getLocation(SensorValue*);
bool getAltitudeGPS(SensorValue*);
bool getAltitudeBAR(SensorValue*);
bool getSpeed(SensorValue*);
bool getSatellites(SensorValue*);
bool getFix(SensorValue*);
bool getAccelerometer(SensorValue*);
bool getMagnetometer(SensorValue*);
bool getGyro(SensorValue*);
bool getOrientation(SensorValue*);
bool getPressure(SensorValue*);
bool getTemperature(SensorValue*);

bool setMotorSpeed(SensorValue*);

GetSensorValueFuncType GetSensorValueFuncs[MAX_SENSOR_TYPE] = {
    getLocation,
    getAltitudeGPS,
    getAltitudeBAR,
    getSpeed,
    getSatellites,
    getFix,
    getAccelerometer,
    getMagnetometer,
    getGyro,
    getOrientation,
    getPressure,
    getTemperature,
    getMotorSpeed,
    getMotorSpeed,
    getMotorSpeed,
    getMotorSpeed
};

SetSensorValueFuncType SetSensorValueFuncs[MAX_SENSOR_TYPE] = {
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    invalidSensorType,
    setMotorSpeed,
    setMotorSpeed,
    setMotorSpeed,
    setMotorSpeed
};

bool getSensorValue(SensorValue* SensorValue)
{
    GetSensorValueFuncType getSensorValueFunc;
    getSensorValueFunc = GetSensorValueFuncs[SensorValue->SensorType];
    return getSensorValueFunc(SensorValue);
}

bool setSensorValue(SensorValue* SensorValue)
{
    SetSensorValueFuncType setSensorValueFunc;
    setSensorValueFunc = SetSensorValueFuncs[SensorValue->SensorType];
    return setSensorValueFunc(SensorValue);
}

bool getLocation(SensorValue* SensorValue)
{
    SensorValue->Values[0] = gps.location.lat();
    SensorValue->Values[1] = gps.location.lng();
    SensorValue->Unit = SensorValueUnitEnum::DEGREES;
    return true;
}

bool getAltitudeGPS(SensorValue* SensorValue)
{
    switch (SensorValue->Unit)
    {
        case SensorValueUnitEnum::MILES:
            SensorValue->Values[0] = gps.altitude.miles();
            break;
        case SensorValueUnitEnum::KILOMETERS:
            SensorValue->Values[0] = gps.altitude.kilometers();
            break;
        case SensorValueUnitEnum::FEET:
            SensorValue->Values[0] = gps.altitude.feet();
            break;
        default:
            SensorValue->Values[0] = gps.altitude.meters();
            SensorValue->Unit = SensorValueUnitEnum::METERS;
            break;
    }
    
    return true;
}

bool getAltitudeBAR(SensorValue* SensorValue)
{
    imu.getAltitude(SensorValue->Values[0]);
    SensorValue->Unit = SensorValueUnitEnum::METERS;
    return true;
}

bool getSpeed(SensorValue* SensorValue)
{
    switch (SensorValue->Unit)
    {
        case SensorValueUnitEnum::MILES_PER_HOUR:
            SensorValue->Values[0] = gps.speed.mph();
            break;
        case SensorValueUnitEnum::METERS_PER_SECOND:
            SensorValue->Values[0] = gps.speed.mps();
            break;
        case SensorValueUnitEnum::KILOMETERS_PER_HOUR:
            SensorValue->Values[0] = gps.speed.kmph();
            break;
        default:
            SensorValue->Values[0] = gps.speed.knots();
            SensorValue->Unit = SensorValueUnitEnum::KNOTS;
            break;
    }
    
    return true;
}

bool getSatellites(SensorValue* SensorValue)
{
    SensorValue->Values[0] = gps.satellites.value();
    SensorValue->Unit = SensorValueUnitEnum::COUNT;
    return true;
}

bool getFix(SensorValue* SensorValue)
{
    SensorValue->Values[0] = (double)gps.fix();
    SensorValue->Unit = SensorValueUnitEnum::BOOLEAN;
    return true;
}

bool getAccelerometer(SensorValue* SensorValue)
{
    float x,y,z;
    imu.getAccelerometer(x, y, z);
    SensorValue->Values[0] = x;
    SensorValue->Values[1] = y;
    SensorValue->Values[2] = z;
    SensorValue->Unit = SensorValueUnitEnum::METERS_PER_SECOND_PER_SECOND;

    return true;
}

bool getMagnetometer(SensorValue* SensorValue)
{
    float x,y,z;
    imu.getMagnetometer(x, y, z);
    SensorValue->Values[0] = x;
    SensorValue->Values[1] = y;
    SensorValue->Values[2] = z;
    SensorValue->Unit = SensorValueUnitEnum::GAUSS;

    return true;
}

bool getGyro(SensorValue* SensorValue)
{
    float x,y,z;
    imu.getGyroscope(x, y, z);
    SensorValue->Values[0] = x;
    SensorValue->Values[1] = y;
    SensorValue->Values[2] = z;
    SensorValue->Unit = SensorValueUnitEnum::DEGREES_PER_SECOND;

    return true;
}

bool getOrientation(SensorValue* SensorValue)
{
    float r,p,h;
    imu.getRPH(r, p, h);
    SensorValue->Values[0] = r;
    SensorValue->Values[1] = p;
    SensorValue->Values[2] = h;
    SensorValue->Unit = SensorValueUnitEnum::DEGREES;

    return true;
}

bool getPressure(SensorValue* SensorValue)
{
    imu.getPressure(SensorValue->Values[0]);
    SensorValue->Unit = SensorValueUnitEnum::HPA;
    return true;
}

bool getTemperature(SensorValue* SensorValue)
{
    float temp;
    imu.getTemperature(temp);

    switch (SensorValue->Unit)
    {
        case SensorValueUnitEnum::DEGREES_FAHRENHEIT:
            SensorValue->Values[0] = temp * (9.0/5.0) + 32;
            break;
        default:
            SensorValue->Values[0] = temp;
            SensorValue->Unit = SensorValueUnitEnum::DEGREES_CELCIUS;
            break;
    }

    return true;
}

bool getMotorSpeed(SensorValue* SensorValue)
{
    int motorNum = SensorValue->SensorType - SensorTypeEnum::MOTOR_SPEED_NW;
    if (motorNum >= 0 && motorNum < 4)
    {
        uint32_t speed = motorController.getMotorSpeed(motorNum);
        SensorValue->Values[0] = (double) speed;
        SensorValue->Unit = SensorValueUnitEnum::MICROSECONDS;
        return true;
    }
    else
    {
        return false;
    }
}

bool setMotorSpeed(SensorValue* SensorValue)
{
    int motorNum = SensorValue->SensorType - SensorTypeEnum::MOTOR_SPEED_NW;
    if (inSystemState(STATE_ON_GROUND) && 
        SensorValue->Unit == SensorValueUnitEnum::MICROSECONDS &&
        motorNum >= 0 && motorNum < 4)
    {
        motorController.setMotorSpeed(motorNum, (uint32_t)SensorValue->Values[0]);
        return true;
    }
    else
    {
        return false;
    }
}

bool invalidSensorType(SensorValue* Values)
{
    return false;
}