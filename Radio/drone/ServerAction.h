#ifndef __SERVER_ACTION_H__
#define __SERVER_ACTION_H__

#include "DroneRadio.h"

class ServerAction
{
public:
    virtual PacketActionType actionType() = 0;
    virtual bool acceptsMethod(PacketMethodType method) = 0;
    virtual void execute(Packet& p) = 0;
};


class PingAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class ArmAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class RearmAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class BeginAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class TestAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class GPSLatLonAltAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class GPSSpeedAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class GPSSatellitesAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class GPSFixAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class IMUAccelAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class IMUMagAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class IMUGyroAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class IMUOrientationAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class IMUPressureAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class IMUTemperatureAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class RouteWaypointAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class FlightRecordsAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class MotorSpeedAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class PIDAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

class TestDurationAction : public ServerAction
{
public:
    PacketActionType actionType();
    bool acceptsMethod(PacketMethodType method);
    void execute(Packet& p);
};

#endif // __SERVER_ACTION_H__