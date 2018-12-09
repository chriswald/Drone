#ifndef __LOW_LAYER_PID_LAYER__
#define __LOW_LAYER_PID_LAYER__ 

#include "PID.h"
#include "MotorController.h"
#include "Sensor.h"
#include <SensorInterface.h>
#include "State.h" // Remove this when done tuning PIDS

#define GYRO_PID_BUNDLE_NUM 4
#define PID_GYRO_X     0
#define PID_GYRO_Y     (PID_GYRO_X + GYRO_PID_BUNDLE_NUM) // 4
#define PID_GYRO_Z     (PID_GYRO_Y + GYRO_PID_BUNDLE_NUM) // 8
#define NUM_GYRO_PIDS  (PID_GYRO_Z + GYRO_PID_BUNDLE_NUM) // 12

// Transforms desired gyroscope readings into the motor speeds needed
// to achieve those readings.
//
// Gyroscope => Motor speed
class LowLayerPIDLayer
{
public:
    LowLayerPIDLayer();
    ~LowLayerPIDLayer();

    void setNominalGyro(float gx, float gy, float gz);
    void iteratePIDS();
    void resetPIDS();

    void getCurrentGyro(float& gx, float& gy, float& gz);

    bool setPID(int index, float p,  float i,  float d);
    bool getPID(int index, float* p, float* i, float* d);

private:
    PID PIDS[NUM_GYRO_PIDS];
    float nom_gx, nom_gy, nom_gz;
};

extern LowLayerPIDLayer llPIDLayer;

#endif //__LOW_LAYER_PID_LAYER__