#include "Sensor.h"
#include "Server.h"
#include "State.h"
#include "Records.h"
#include "Route.h"
#include "LowLayerPIDLayer.h"
#include "MidLayerPIDLayer.h"
#include "HighLayerPIDLayer.h"
#include <stdint.h>
#include <Arduino.h>

//#define WAIT_ON_INPUT

void setFlightControllerNominal();
void storeHomeLocation();
void calibrateAltimiter();
void initSystem();

void updateRecords();
void startRecordsTimer();
void endRecordsTimer();
void stopTakeoff();
IntervalTimer recordsUpdater;
IntervalTimer takeoffTimer;
bool timerRunning = false;

//REMOVE THESE
uint64_t takeoffMillis;
bool isTrackingTakeoff;
uint32_t test_flight_duration = 2000; //millis
bool mock_preflight = false;
//^^^^^^^^^^^^

void setup()
{
    Serial.begin(9600);
    #ifdef WAIT_ON_INPUT
    while (!Serial.available()) yield();
    while (Serial.available()) Serial.read();
    Serial.println("Done waiting");
    #endif

    initSystem();
}

void loop()
{
    loopGPS();
    loopServer();

    switch(getSystemState())
    {
        case STATE_ON_GROUND:
            if (gps.fix() && gps.location.lat() != 0)
            {
                storeHomeLocation();
                if (!imu.isCalibrated())
                    calibrateAltimiter();
                if (ROUTE.numWaypoints() > 0)
                {
                    signalReadyForTakeoff(true);
                }
                else
                {
                    signalReadyForTakeoff(false);
                }
            }
            break;
        case STATE_MOCK_PREFLIGHT:
            Serial.println("State Mock-Preflight");
            mock_preflight = true;
        case STATE_PREFLIGHT:
            // Do any preflight checks here such as checking the battery levels,
            // Spinning up the props individually, and making sure the GPS has a
            // fix one last time. Also get readings from all of the distance
            // sensors.
            Serial.println("State Preflight");
            if (!motorController.getMotorsArmState())
                motorController.armMotors();

            //startRecordsTimer();
            
            setSystemState(STATE_TAKEOFF);
            break;
        case STATE_TAKEOFF:
            // Rise a few feet off of the ground and hover, using a combination
            // of the altimiter and downward facing distance sensor.
            if (!isTrackingTakeoff)
            {
                Serial.println("State Takeoff");
                isTrackingTakeoff = true;
                takeoffMillis = millis();
            }

            // Start with just testing the low layer for now
            llPIDLayer.iteratePIDS();

            if (mock_preflight && (millis() - takeoffMillis > test_flight_duration))
            {
                isTrackingTakeoff = false;
                stopTakeoff();
            }
            break;
        case STATE_NAVIGATING:
            hlPIDLayer.iteratePIDS();
            break;
        case STATE_LANDING:
            endRecordsTimer();
            break;
        case STATE_E_FAILED_INIT:
            delay(1000);
            initSystem(); // Try initialization again.
            break;
        default:
            break;
    }
}

void initSystem()
{
    initSystemState();

    bool success = true;
    if (!initServer()) {
        Serial.println("Failed to initialize Radio");
        success = false;
    }

    if (!initSensors()) {
        Serial.println("Failed to initialize the sensors");
        success = false;
    }

    if (!success)
    {
        setSystemState(STATE_E_FAILED_INIT);
    }
    else
    {
        delay(100);
        setSystemState(STATE_ON_GROUND);
    }
}

void storeHomeLocation()
{
    Location loc = {(float)gps.location.lat(),
                    (float)gps.location.lng(),
                    (float)gps.altitude.meters()};
    ROUTE.setHomeLocation(loc);
}

void calibrateAltimiter()
{
    float alt = gps.altitude.meters();
    imu.calibrateAltitude(alt);
    imu.getAltitude(alt);
    RECORDS.setBaseAltitude(alt);
}

void startRecordsTimer()
{
    if (!timerRunning)
    {
        noInterrupts();
        RECORDS.recordStartTime();
        recordsUpdater.begin(updateRecords, 1000000); //1sec
        timerRunning = true;
        interrupts();
    }
}

void endRecordsTimer()
{
    if (timerRunning)
    {
        noInterrupts();
        recordsUpdater.end();
        timerRunning = false;
        RECORDS.recordEndTime();
        interrupts();
    }
}

void updateRecords()
{
    if (getSystemState() == STATE_NAVIGATING)
    {
        float altitude;
        imu.getAltitude(altitude);
        RECORDS.recordSpeed(gps.speed.knots());
        RECORDS.recordAltitude(altitude);
    }
}

void stopTakeoff()
{
    uint64_t endMillis;
    Serial.print("Start = ");
    Serial.print((int)takeoffMillis);
    Serial.print(" End = ");
    Serial.print((int)(endMillis = millis()));
    Serial.print(" Delta = ");
    Serial.println((int)(endMillis - takeoffMillis));
    Serial.println("Ending Takeoff");

    takeoffTimer.end();
    motorController.setMotorSpeed(0, MOTOR_ZERO_SPEED);
    motorController.setMotorSpeed(1, MOTOR_ZERO_SPEED);
    motorController.setMotorSpeed(2, MOTOR_ZERO_SPEED);
    motorController.setMotorSpeed(3, MOTOR_ZERO_SPEED);

    mock_preflight = false;
    setSystemState(STATE_ON_GROUND);
}
