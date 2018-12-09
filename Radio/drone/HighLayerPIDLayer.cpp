#include "HighLayerPIDLayer.h"
#include "MotorController.h"

HighLayerPIDLayer hlPIDLayer;

HighLayerPIDLayer::HighLayerPIDLayer()
{
    nom_lat = 0;
    nom_lon = 0;
    nom_alt = 0;

    for (int i = PID_LAT; i < PID_LAT + GPS_PID_BUNDLE_NUM; i ++)
    {
        PIDS[i].KP = 1000;
        PIDS[i].KI = 0;
        PIDS[i].KD = 0;
        PIDS[i].IMAX = 100;
        PIDS[i].IMIN = -100;
        PIDS[i].dState_reset = 0;
    }

    for (int i = PID_LON; i < PID_LON + GPS_PID_BUNDLE_NUM; i ++)
    {
        PIDS[i].KP = 1000;
        PIDS[i].KI = 0;
        PIDS[i].KD = 0;
        PIDS[i].IMAX = 100;
        PIDS[i].IMIN = -100;
        PIDS[i].dState_reset = 0;
    }

    for (int i = PID_ALT; i < PID_ALT + GPS_PID_BUNDLE_NUM; i ++)
    {
        PIDS[i].KP = 1000;
        PIDS[i].KI = 0;
        PIDS[i].KD = 0;
        PIDS[i].IMAX = 100;
        PIDS[i].IMIN = -100;
        PIDS[i].dState_reset = 0;
    }
}

HighLayerPIDLayer::~HighLayerPIDLayer() {}

void HighLayerPIDLayer::setNominalGPS(float lat, float lon, float alt)
{
    nom_lat = lat;
    nom_lon = lon;
    nom_alt = alt;
}

void HighLayerPIDLayer::iteratePIDS()
{
    // TODO: figure this section out
    // I need to figure out proper constraints for RPH, eg how many
    // degrees tilt is the maximum.
    // For now just pass this call through.

    mlPIDLayer.iteratePIDS();


    float altitude;
    imu.getAltitude(altitude);
    unsigned currNomSpeed = motorController.getNominalSpeed();
    PIDS[PID_ALT].updatePID(altitude, nom_alt, currNomSpeed);
}

void HighLayerPIDLayer::resetPIDS()
{
    for (int i = PID_LAT; i < PID_LAT + NUM_GPS_PIDS; i ++)
    {
        PIDS[i].resetPID();
    }
}

void HighLayerPIDLayer::getCurrentGPS(float& lat, float& lon, float& alt)
{
    lat = gps.location.lat();
    lon = gps.location.lng();
    imu.getAltitude(alt);
}

bool HighLayerPIDLayer::setPID(int index, float p=-1, float i=-1, float d=-1)
{
    if (index >= NUM_GPS_PIDS || index % GPS_PID_BUNDLE_NUM != 0)
        return false;

    for (int j = index; j < index + GPS_PID_BUNDLE_NUM; j ++)
    {
        if (p != -1)
            PIDS[j].KP = p;
        if (i != -1)
            PIDS[j].KI = i;
        if (d != -1)
            PIDS[j].KD = d;
    }

    return true;
}

bool HighLayerPIDLayer::getPID(int index, float* p, float* i, float* d)
{
    if (index >= NUM_GPS_PIDS || index % GPS_PID_BUNDLE_NUM != 0)
        return false;

    if (p != NULL)
        *p = PIDS[index].KP;
    if (i != NULL)
        *i = PIDS[index].KI;
    if (d != NULL)
        *d = PIDS[index].KD;

    return true;
}