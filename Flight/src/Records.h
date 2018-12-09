#ifndef __RECORDS_H__
#define __RECORDS_H__ 

#define RECORDS_MIN_ADDR 0

#define TIME_ADDR (RECORDS_MIN_ADDR)
#define TIME_TYPE long

#define SPEED_ADDR (TIME_ADDR + sizeof(TIME_TYPE))
#define SPEED_TYPE float

#define ALT_ADDR (SPEED_ADDR + sizeof(SPEED_TYPE))
#define ALT_TYPE float

#define AOG_ADDR (ALT_ADDR + sizeof(ALT_TYPE))
#define AOG_TYPE float

#define RECORDS_MAX_ADDR (AOG_ADDR + sizeof(AOG_TYPE))
#define RECORDS_STORAGE_SIZE (RECORDS_MAX_ADDR - RECORDS_MIN_ADDR)

class Records
{
public:
    void recordStartTime();
    void recordEndTime();
    long getFlightTimeMillis();

    void recordSpeed(float speed);
    float getMaxSpeed();

    void setBaseAltitude(float altitude);
    void recordAltitude(float altitude);
    float getMaxAltitude();
    float getMaxAltitudeOffGround();

    void saveRecords();
    void loadRecords();

    void clear();
private:
    long startTime;
    long endTime;
    float max_speed;
    float base_altitude;
    float max_altitude;
};

extern Records RECORDS;

#endif // __RECORDS_H__