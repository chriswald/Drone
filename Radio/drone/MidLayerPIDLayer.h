#ifndef __MID_LAYER_PID_LAYER_H__
#define __MID_LAYER_PID_LAYER_H__

#include "PID.h"
#include "LowLayerPIDLayer.h"
#include "Sensor.h"

#define RPH_PID_BUNDLE_NUM 1
#define PID_ROLL       0
#define PID_PITCH      (PID_ROLL    + RPH_PID_BUNDLE_NUM)
#define PID_HEADING    (PID_PITCH   + RPH_PID_BUNDLE_NUM)
#define NUM_RPH_PIDS   (PID_HEADING + RPH_PID_BUNDLE_NUM)

// Transforms desired RPH values into the gyroscope readings to
// achieve those RPH values.
//
// RPH => Gyroscope
class MidLayerPIDLayer
{
public:
    MidLayerPIDLayer();
    ~MidLayerPIDLayer();

    void setNominalRPH(float r, float p, float h);
    void iteratePIDS();
    void resetPIDS();

    void getCurrentRPH(float& r, float& p, float& h);

    bool setPID(int index, float p,  float i,  float d);
    bool getPID(int index, float* p, float* i, float* d);

private:
    PID PIDS[NUM_RPH_PIDS];
    float nom_r, nom_p, nom_h;
};

extern MidLayerPIDLayer mlPIDLayer;

#endif //__MID_LAYER_PID_LAYER_H__