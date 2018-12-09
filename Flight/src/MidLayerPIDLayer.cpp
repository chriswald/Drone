#include "MidLayerPIDLayer.h"

MidLayerPIDLayer mlPIDLayer;

MidLayerPIDLayer::MidLayerPIDLayer()
{
    nom_r = 0;
    nom_p = 0;
    nom_h = 0;

    for (int i = PID_ROLL; i < PID_ROLL + RPH_PID_BUNDLE_NUM; i ++)
    {
        PIDS[i].KP = 1000;
        PIDS[i].KI = 0;
        PIDS[i].KD = 0;
        PIDS[i].IMAX = 100;
        PIDS[i].IMIN = -100;
        PIDS[i].dState_reset = 0;
    }

    for (int i = PID_PITCH; i < PID_PITCH + RPH_PID_BUNDLE_NUM; i ++)
    {
        PIDS[i].KP = 1000;
        PIDS[i].KI = 0;
        PIDS[i].KD = 0;
        PIDS[i].IMAX = 100;
        PIDS[i].IMIN = -100;
        PIDS[i].dState_reset = 0;
    }

    for (int i = PID_HEADING; i < PID_HEADING + RPH_PID_BUNDLE_NUM; i ++)
    {
        PIDS[i].KP = 1000;
        PIDS[i].KI = 0;
        PIDS[i].KD = 0;
        PIDS[i].IMAX = 100;
        PIDS[i].IMIN = -100;
        PIDS[i].dState_reset = 0;
    }
}

MidLayerPIDLayer::~MidLayerPIDLayer() {}

void MidLayerPIDLayer::setNominalRPH(float r, float p, float h)
{
    nom_r = r;
    nom_p = p;
    nom_h = h;
}

void MidLayerPIDLayer::iteratePIDS()
{
    float r, p, h, gx, gy, gz;
    float xerr, yerr, zerr;

    imu.getRPH(r, p, h);
    llPIDLayer.getCurrentGyro(gx, gy, gz);

    xerr = PIDS[PID_ROLL].updatePID(r, nom_r, gx);
    yerr = PIDS[PID_PITCH].updatePID(p, nom_p, gy);
    zerr = PIDS[PID_HEADING].updatePID(h, nom_h, gz);

    llPIDLayer.setNominalGyro(xerr, yerr, zerr);
    llPIDLayer.iteratePIDS();
}

void MidLayerPIDLayer::resetPIDS()
{
    for (int i = PID_ROLL; i < PID_ROLL + NUM_RPH_PIDS; i ++)
    {
        PIDS[i].resetPID();
    }
}

void MidLayerPIDLayer::getCurrentRPH(float& r, float& p, float& h)
{
    imu.getRPH(r, p, h);
}

bool MidLayerPIDLayer::setPID(int index, float p=-1, float i=-1, float d=-1)
{
    if (index >= NUM_RPH_PIDS || index % RPH_PID_BUNDLE_NUM != 0)
        return false;

    for (int j = index; j < index + RPH_PID_BUNDLE_NUM; j ++)
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

bool MidLayerPIDLayer::getPID(int index, float* p, float* i, float* d)
{
    if (index >= NUM_RPH_PIDS || index % RPH_PID_BUNDLE_NUM != 0)
        return false;

    if (p != NULL)
        *p = PIDS[index].KP;
    if (i != NULL)
        *i = PIDS[index].KI;
    if (d != NULL)
        *d = PIDS[index].KD;

    return true;
}