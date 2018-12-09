#ifndef __PACKET_H__
#define __PACKET_H__

#include <stdint.h>
#include <string.h>

#define PACKET_SIZE  32
#define PAYLOAD_SIZE 30

enum PacketMethodType {
    METHOD_COMMAND,
    METHOD_GET,
    METHOD_SET,
    METHOD_DELETE
};

enum PacketActionType {
    ACTION_PING,
    ACTION_ARM,
    ACTION_REARM,
    ACTION_BEGIN,
    ACTION_TEST,
    ACTION_GPS_LAT_LON_ALT,
    ACTION_GPS_SPEED,
    ACTION_GPS_SATELLITES,
    ACTION_GPS_FIX,
    ACTION_IMU_ACCEL,
    ACTION_IMU_MAG,
    ACTION_IMU_GYRO,
    ACTION_IMU_ORIENTATION,
    ACTION_IMU_PRESSURE,
    ACTION_IMU_TEMPERATURE,
    ACTION_ROUTE_WAYPOINT,
    ACTION_FLIGHT_RECORDS,
    ACTION_MOTOR_SPEED,
    ACTION_PID,
    ACTION_TEST_DURATION,
    ACTION_CHANNEL_TRANSITION, //Unimplemented
    ACTION_RECONFIGURE         //Unimplemented
};

enum PacketErrorType {
    NO_ERROR,
    ERROR_CORRUPT,
    ERROR_INVALID_REQUEST,
    ERROR_FAIL,
    ERROR_NOT_IMPLEMENTED,
    ERROR_INVALID_MOTOR_NUM,
    ERROR_INVALID_INDEX,
};

class Packet
{
public:
    Packet();
    ~Packet();
    void setMethod(PacketMethodType method);
    void setAction(PacketActionType action);
    void setError(PacketErrorType error);
    void setDataArray(void* data, uint8_t size, uint8_t offset=0);
    PacketMethodType getMethod();
    PacketActionType getAction();
    PacketErrorType getError();
    void getDataArray(void* data, uint8_t size, uint8_t offset=0);

    void package(uint8_t* bytes, uint8_t* size);
    void unpackage(uint8_t* bytes, uint8_t size=PACKET_SIZE);
    bool isCorrupt();

private:
    uint8_t header;
    uint8_t error;
    uint8_t payload[PAYLOAD_SIZE];

    void setParity();

    uint8_t packetParity();
    uint8_t headerParity();
    uint8_t errorParity();
    uint8_t payloadParity();
};

#endif //__PACKET_H__
