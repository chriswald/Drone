#ifndef __HIGH_LAYER_PID_LAYER_H__
#define __HIGH_LAYER_PID_LAYER_H__

#include "PID.h"
#include "MidLayerPIDLayer.h"
#include "Sensor.h"

#define GPS_PID_BUNDLE_NUM 1
#define PID_LAT        0
#define PID_LON        (PID_LAT + GPS_PID_BUNDLE_NUM)
#define PID_ALT        (PID_LON + GPS_PID_BUNDLE_NUM)
#define NUM_GPS_PIDS   (PID_ALT + GPS_PID_BUNDLE_NUM)

// Transforms desired GPS readings into the the RPH values needed
// to achieve those readings.
//
// GPS => RPH
class HighLayerPIDLayer
{
public:
    HighLayerPIDLayer();
    ~HighLayerPIDLayer();

    void setNominalGPS(float lat, float lon, float alt);
    void iteratePIDS();
    void resetPIDS();

    void getCurrentGPS(float& lat, float& lon, float& alt);

    bool setPID(int index, float p,  float i,  float d);
    bool getPID(int index, float* p, float* i, float* d);

private:
    PID PIDS[NUM_RPH_PIDS];
    float nom_lat, nom_lon, nom_alt;
};

extern HighLayerPIDLayer hlPIDLayer;

#endif //__HIGH_LAYER_PID_LAYER_H__