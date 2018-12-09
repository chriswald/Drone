#ifndef __SENSOR_INTERFACE_H__
#define __SENSOR_INTERFACE_H__ 

#include <stdint.h>

typedef enum
{
    LOCATION = 0,
    ALTITUDE_GPS,
    ALTITUDE_BAR,
    SPEED,
    SATELLITES,
    GPS_FIX_STATUS,
    ACCELEROMETER,
    MAGNETOMETER,
    GYRO,
    ORIENTATION,
    PRESSURE,
    TEMPERATURE,
    MOTOR_SPEED_NW,
    MOTOR_SPEED_NE,
    MOTOR_SPEED_SE,
    MOTOR_SPEED_SW,
    MAX_SENSOR_TYPE
} SensorTypeEnum;

typedef enum 
{
    DEGREES = 0,
    METERS,
    MILES,
    KILOMETERS,
    FEET,
    KNOTS,
    MILES_PER_HOUR,
    METERS_PER_SECOND,
    KILOMETERS_PER_HOUR,
    COUNT,
    BOOLEAN,
    DEGREES_PER_SECOND,
    METERS_PER_SECOND_PER_SECOND,
    GAUSS,
    HPA,
    DEGREES_CELCIUS,
    DEGREES_FAHRENHEIT,
    MICROSECONDS
} SensorValueUnitEnum;

typedef struct
{
    SensorTypeEnum SensorType;
    SensorValueUnitEnum Unit;
    float Values[3];
} SensorValue;

bool getSensorValue(SensorValue* sensorValue);
bool setSensorValue(SensorValue* sensorValue);

#endif // __SENSOR_INTERFACE_H__