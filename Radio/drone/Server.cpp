#include "Server.h"
#include "Sensor.h"
#include "State.h"
#include "Route.h"
#include "config.h"
#include "Records.h"
#include "MessageQueue.h"
#include "ServerAction.h"
#include "DroneRadio.h"

#define INVALID_SENSOR_NUM 0xFE

DroneRadio server;

void srvMessageReceived_ISR();
void loopServer();
void processPacket(Packet& p);
void sendGPSLatLonAlt();
void addWaypoint(Packet p);
void sendNumWaypoints();
void clearSystemRoute();
void removeSystemWaypoint(Packet p);
void sendWaypoint(Packet p);
void insertWaypoint(Packet p);

#define NUM_SERVER_ACTIONS 20
ServerAction* serverAction[NUM_SERVER_ACTIONS];

bool initServer()
{
    byte val;

    serverAction[ACTION_PING] = new PingAction();
    serverAction[ACTION_ARM] = new ArmAction();
    serverAction[ACTION_REARM] = new RearmAction();
    serverAction[ACTION_BEGIN] = new BeginAction();
    serverAction[ACTION_TEST] = new TestAction();
    serverAction[ACTION_GPS_LAT_LON_ALT] = new GPSLatLonAltAction();
    serverAction[ACTION_GPS_SPEED] = new GPSSpeedAction();
    serverAction[ACTION_GPS_SATELLITES] = new GPSSatellitesAction();
    serverAction[ACTION_GPS_FIX] = new GPSFixAction();
    serverAction[ACTION_IMU_ACCEL] = new IMUAccelAction();
    serverAction[ACTION_IMU_MAG] = new IMUMagAction();
    serverAction[ACTION_IMU_GYRO] = new IMUGyroAction();
    serverAction[ACTION_IMU_ORIENTATION] = new IMUOrientationAction();
    serverAction[ACTION_IMU_PRESSURE] = new IMUPressureAction();
    serverAction[ACTION_IMU_TEMPERATURE] = new IMUTemperatureAction();
    serverAction[ACTION_ROUTE_WAYPOINT] = new RouteWaypointAction();
    serverAction[ACTION_FLIGHT_RECORDS] = new FlightRecordsAction();
    serverAction[ACTION_MOTOR_SPEED] = new MotorSpeedAction();
    serverAction[ACTION_PID] = new PIDAction();
    serverAction[ACTION_TEST_DURATION] = new TestDurationAction();

    server.config("serv1", "clie1", RADIO_CE, RADIO_CSN);
    server.configInterrupt(srvMessageReceived_ISR, RADIO_IRQ, true, false, false);

    // Check radio configuration
    server.readRegister(CONFIG, &val, sizeof(byte));
    return (val == 0x3B);
}

void loopServer()
{
    if (!qisEmpty())
    {
        Packet p;
        qpop(p);
        processPacket(p);
    }
}

void srvMessageReceived_ISR()
{
    while (server.dataReady())
    {
        Packet p;
        server.get(p);
        qpush(p);
    }
    digitalWrite(RADIO_IRQ, HIGH);
}

void processPacket(Packet& p)
{
    if (p.isCorrupt())
    {
        p.setError(ERROR_CORRUPT);
        server.send(p);
        return;
    }

    p.setError(NO_ERROR);

    PacketActionType pAct = p.getAction();
    if (serverAction[pAct]->acceptsMethod(p.getMethod()))
    {
        serverAction[pAct]->execute(p);
    }
    else
    {
        p.setError(ERROR_INVALID_REQUEST);
    }

    server.send(p);
}

/*void addWaypoint(Packet p)
{
    if (inSystemState(STATE_ON_GROUND))
    {
        Serial.println("Adding waypoint to the system route");
        Location l;
        l.lat = ((float*)p.data)[0];
        l.lon = ((float*)p.data)[1];
        l.alt = ((float*)p.data)[2];
        bool success = ROUTE.addLocation(l);
        if (success)
        {
            *(float*)p.data = (float)ROUTE.totalDistanceMeters();
            server.send(p);
        }
    }
}

void sendNumWaypoints()
{
    Serial.println("Sending the number of waypoints in route");
    Packet p;
    p.typeID = PACKET_GET_NUM_WAYPOINTS;
    p.data[0] = ROUTE.numWaypoints();
    server.send(p);
}

void clearSystemRoute()
{
    Serial.println("Clearing the system route");
    ROUTE.clearRoute();
    Packet p;
    p.typeID = PACKET_CLEAR_ROUTE;
    server.send(p);
}

void removeSystemWaypoint(Packet p)
{
    byte index = p.data[0];
    bool success = ROUTE.removeLocation((int)index);
    *(int*)p.data = success;

    Serial.print("Removing waypoint ");
    Serial.print(index);
    Serial.print(" ...");
    if (success)
        Serial.println("DONE");
    else
        Serial.println("FAIL");

    server.send(p);
}

void sendWaypoint(Packet p)
{
    Serial.println("Sending waypoint to caller");
    int index = (int) p.data[0];
    Location l = ROUTE.getLocation(index);
    p.typeID = PACKET_GET_WAYPOINT;
    ((float*)p.data)[0] = l.lat;
    ((float*)p.data)[1] = l.lon;
    ((float*)p.data)[2] = l.alt;
    server.send(p);
}

void insertWaypoint(Packet p)
{
    if (inSystemState(STATE_ON_GROUND))
    {
        Serial.println("Inserting waypoint into the system route");
        Location l;
        int index;
        l.lat = ((float*)p.data)[0];
        l.lon = ((float*)p.data)[1];
        l.alt = ((float*)p.data)[2];
        index = (int)((float*)p.data)[3];
        bool success = ROUTE.insertLocation(l, index);
        if (success)
        {
            *(float*)p.data = (float)ROUTE.totalDistanceMeters();
            server.send(p);
        }
    }
}*/