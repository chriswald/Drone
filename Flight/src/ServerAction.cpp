#include "ServerAction.h"
#include "MotorController.h"
#include "LowLayerPIDLayer.h"
#include "MidLayerPIDLayer.h"
#include "Sensor.h"
#include "State.h"
#include "Records.h"
#include "Route.h"

PacketActionType PingAction::actionType()
{
    return ACTION_PING;
}

bool PingAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_COMMAND:
            return true;
        default:
            return false;
    }
}

void PingAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_COMMAND)
    {
        Serial.println("Ping Action");
    }
}

PacketActionType ArmAction::actionType()
{
    return ACTION_ARM;
}

bool ArmAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_COMMAND:
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void ArmAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_COMMAND)
    {
        Serial.println("Arming motors");
        motorController.armMotors();
        bool isReady = motorController.getMotorsArmState();
        p.setDataArray((uint8_t*)&isReady, sizeof(isReady));
    }
    if (p.getMethod() == METHOD_GET)
    {
        bool isReady = motorController.getMotorsArmState();
        p.setDataArray((uint8_t*)&isReady, sizeof(isReady));
    }
}

PacketActionType RearmAction::actionType()
{
    return ACTION_REARM;
}

bool RearmAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_COMMAND:
            return true;
        default:
            return false;
    }
}

void RearmAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_COMMAND)
    {
        Serial.println("Rearming Motors");
        motorController.armMotors();
        bool isReady = motorController.getMotorsArmState();
        p.setDataArray((uint8_t*)&isReady, sizeof(isReady));
    }
}

PacketActionType BeginAction::actionType()
{
    return ACTION_BEGIN;
}

bool BeginAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_COMMAND:
            return true;
        default:
            return false;
    }
}

void BeginAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_COMMAND)
    {
        Serial.print("Preparing for PREFLIGHT ...");

        if (inSystemState(STATE_ON_GROUND) &&
            isPropertySet(PROPERTY_READY_FOR_TAKEOFF))
        {
            Serial.println("DONE");
            setSystemState(STATE_PREFLIGHT);
        }
        else
        {
            Serial.println("FAIL");
            p.setError(ERROR_FAIL);
        }
    }
}

PacketActionType TestAction::actionType()
{
    return ACTION_TEST;
}

bool TestAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_COMMAND:
            return true;
        default:
            return false;
    }
}

void TestAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_COMMAND)
    {
        if (inSystemState(STATE_ON_GROUND))
        {
            uint32_t y, nom;
            p.getDataArray(&y, sizeof(y));
            p.getDataArray(&nom, sizeof(nom), sizeof(y));
            
            Serial.println("Initiating mock preflight routine.");
            Serial.print("Nominal gyro (y) = ");
            Serial.println(((float)y)/20.0);
            Serial.print("Nominal motor speed = ");
            Serial.println(nom);
            
            // FOR TESTING
            motorController.setNominalSpeed(nom);
            llPIDLayer.setNominalGyro(0, ((float)y)/20.0, 0);
            llPIDLayer.resetPIDS();

            // Enter perflight state.
            setSystemState(STATE_MOCK_PREFLIGHT);
        }
    }
}

PacketActionType GPSLatLonAltAction::actionType()
{
    return ACTION_GPS_LAT_LON_ALT;
}

bool GPSLatLonAltAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void GPSLatLonAltAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for Lat, Lon, and Alt");
        float data[4];
        data[0] = gps.location.lat();
        data[1] = gps.location.lng();
        imu.getAltitude(data[2]);
        data[3] = gps.altitude.meters();
        p.setDataArray(data, sizeof(data));
    }
}

PacketActionType GPSSpeedAction::actionType()
{
    return ACTION_GPS_SPEED;
}

bool GPSSpeedAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void GPSSpeedAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for speed");
        float data = gps.speed.knots();
        p.setDataArray(&data, sizeof(data));
    }
}

PacketActionType GPSSatellitesAction::actionType()
{
    return ACTION_GPS_SATELLITES;
}

bool GPSSatellitesAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void GPSSatellitesAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for satellites");
        uint8_t data = gps.satellites.value();
        p.setDataArray(&data, sizeof(data));
    }
}

PacketActionType GPSFixAction::actionType()
{
    return ACTION_GPS_FIX;
}

bool GPSFixAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void GPSFixAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for fix status");
        uint8_t data = gps.fix();
        p.setDataArray(&data, sizeof(data));
    }
}

PacketActionType IMUAccelAction::actionType()
{
    return ACTION_IMU_ACCEL;
}

bool IMUAccelAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void IMUAccelAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for accelerometer");
        float data[3];
        imu.getAccelerometer(data[0], data[1], data[2]);
        p.setDataArray(data, sizeof(data));
    }
}

PacketActionType IMUMagAction::actionType()
{
    return ACTION_IMU_MAG;
}

bool IMUMagAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void IMUMagAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for magnetometer");
        float data[3];
        imu.getMagnetometer(data[0], data[1], data[2]);
        p.setDataArray(data, sizeof(data));
    }
}

PacketActionType IMUGyroAction::actionType()
{
    return ACTION_IMU_GYRO;
}

bool IMUGyroAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void IMUGyroAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for gyroscope");
        float data[3];
        imu.getGyroscope(data[0], data[1], data[2]);
        p.setDataArray(data, sizeof(data));
    }
}

PacketActionType IMUOrientationAction::actionType()
{
    return ACTION_IMU_ORIENTATION;
}

bool IMUOrientationAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void IMUOrientationAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for orientation");
        float data[3];
        imu.getRPH(data[0], data[1], data[2]);
        p.setDataArray(data, sizeof(data));
    }
}

PacketActionType IMUPressureAction::actionType()
{
    return ACTION_IMU_PRESSURE;
}

bool IMUPressureAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void IMUPressureAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for pressure");
        float data;
        imu.getPressure(data);
        p.setDataArray(&data, sizeof(data));
    }
}

PacketActionType IMUTemperatureAction::actionType()
{
    return ACTION_IMU_TEMPERATURE;
}

bool IMUTemperatureAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
            return true;
        default:
            return false;
    }
}

void IMUTemperatureAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        Serial.println("Processing request for temperature");
        float data;
        imu.getTemperature(data);
        p.setDataArray(&data, sizeof(data));
    }
}

PacketActionType RouteWaypointAction::actionType()
{
    return ACTION_ROUTE_WAYPOINT;
}

bool RouteWaypointAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
        case METHOD_SET:
        case METHOD_DELETE:
            return true;
        default:
            return false;
    }
}

void RouteWaypointAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        p.setError(ERROR_NOT_IMPLEMENTED);
    }
    else if (p.getMethod() == METHOD_SET)
    {
        p.setError(ERROR_NOT_IMPLEMENTED);
    }
    else if (p.getMethod() == METHOD_DELETE)
    {
        p.setError(ERROR_NOT_IMPLEMENTED);
    }
}

PacketActionType FlightRecordsAction::actionType()
{
    return ACTION_FLIGHT_RECORDS;
}

bool FlightRecordsAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
        case METHOD_DELETE:
            return true;
        default:
            return false;
    }
}

void FlightRecordsAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        long time;
        float data[3];
        time    = RECORDS.getFlightTimeMillis();
        data[0] = RECORDS.getMaxSpeed();
        data[1] = RECORDS.getMaxAltitude();
        data[2] = RECORDS.getMaxAltitudeOffGround();

        p.setDataArray(&time, sizeof(time));
        p.setDataArray(data, sizeof(data), sizeof(time));
    }
    else if (p.getMethod() == METHOD_DELETE)
    {
        RECORDS.clear();
    }
}

PacketActionType MotorSpeedAction::actionType()
{
    return ACTION_MOTOR_SPEED;
}

bool MotorSpeedAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
        case METHOD_SET:
            return true;
        default:
            return false;
    }
}

void MotorSpeedAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        uint32_t motorNum;
        p.getDataArray(&motorNum, sizeof(motorNum));
        if (motorNum < 4)
        {
            uint32_t speed = motorController.getMotorSpeed(motorNum);
            Serial.print("Retrieving motors[");
            Serial.print(motorNum);
            Serial.print("].speed = ");
            Serial.print(speed);
            Serial.println();
            
            p.setDataArray(&speed, sizeof(speed), sizeof(motorNum));
        }
        else
        {
            p.setError(ERROR_INVALID_MOTOR_NUM);
        }
    }
    else if (p.getMethod() == METHOD_SET)
    {
        if (inSystemState(STATE_ON_GROUND))
        {
            uint32_t data[2];
            p.getDataArray(data, sizeof(data));
            uint32_t motorNum = data[0];
            uint32_t speed = data[1];

            if (motorNum < 4)
            {
                if (motorController.getMotorsArmState())
                {
                    Serial.print("Updating motors[");
                    Serial.print(motorNum);
                    Serial.print("].speed = ");
                    Serial.print(speed);
                    Serial.println();

                    // Set individual motor speed.
                    motorController.setMotorSpeed(motorNum, speed);
                }
                else
                {
                    Serial.print("Failed to set motors[");
                    Serial.print(motorNum);
                    Serial.print("].speed: Motors not armed");
                    Serial.println();
                    speed = motorController.getMotorSpeed(motorNum);
                }
                
                p.setDataArray(&motorNum, sizeof(motorNum));
                p.setDataArray(&speed, sizeof(speed), sizeof(motorNum));
            }
            else
            {
                p.setError(ERROR_INVALID_MOTOR_NUM);
            }
        }
    }
}

PacketActionType PIDAction::actionType()
{
    return ACTION_PID;
}

bool PIDAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
        case METHOD_SET:
            return true;
        default:
            return false;
    }
}

void PIDAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        uint32_t index;
        float data[3];
        p.getDataArray(&index, sizeof(index));

        bool success = llPIDLayer.getPID(index, &(data[0]), &(data[1]), &(data[2]));
        if (!success)
        {
            p.setError(ERROR_INVALID_INDEX);
            return;
        }

        Serial.print("Retrieving PID[");
        Serial.print(index);
        Serial.print("] is P:");
        Serial.print(data[0]);
        Serial.print(" I:");
        Serial.print(data[1]);
        Serial.print(" D:");
        Serial.print(data[2]);
        Serial.println();

        p.setDataArray(&index, sizeof(index));
        p.setDataArray(data, sizeof(data), sizeof(index));
    }
    else if (p.getMethod() == METHOD_SET)
    {
        uint32_t index;
        float data[3];
        p.getDataArray(&index, sizeof(index));
        p.getDataArray(data, sizeof(data), sizeof(index));

        Serial.print("Updating PID[");
        Serial.print(index);
        Serial.print("] to P:");
        Serial.print(data[0]);
        Serial.print(" I:");
        Serial.print(data[1]);
        Serial.print(" D:");
        Serial.print(data[2]);
        Serial.println();

        bool success = llPIDLayer.setPID(index, data[0], data[1], data[2]);
        if (!success)
        {
            p.setError(ERROR_INVALID_INDEX);
            return;
        }
    }
}

PacketActionType TestDurationAction::actionType()
{
    return ACTION_TEST_DURATION;
}

bool TestDurationAction::acceptsMethod(PacketMethodType method)
{
    switch(method)
    {
        case METHOD_GET:
        case METHOD_SET:
            return true;
        default:
            return false;
    }
}

extern int test_flight_duration;
void TestDurationAction::execute(Packet& p)
{
    if (p.getMethod() == METHOD_GET)
    {
        p.setDataArray(&test_flight_duration, sizeof(test_flight_duration));
        Serial.print("Retrieving test flight duration: ");
        Serial.print(test_flight_duration);
        Serial.println("ms");
    }
    else if (p.getMethod() == METHOD_SET)
    {
        p.getDataArray(&test_flight_duration, sizeof(test_flight_duration));
        Serial.print("Updated test flight duration to ");
        Serial.print(test_flight_duration);
        Serial.println("ms");
    }
}
