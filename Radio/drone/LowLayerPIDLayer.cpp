#include "LowLayerPIDLayer.h"

LowLayerPIDLayer llPIDLayer;

// Remove when done testing PID Loops
#include "Logger.h"
Logger actlog('a');
Logger gxlog('x');
Logger gylog('y');
Logger gzlog('z');
/////////////////////////////////////

LowLayerPIDLayer::LowLayerPIDLayer()
{
    nom_gx = 0;
    nom_gy = 0;
    nom_gz = 0;

    for (int i = PID_GYRO_X; i < PID_GYRO_X + GYRO_PID_BUNDLE_NUM; i ++)
    {
        PIDS[i].KP = 1000;
        PIDS[i].KI = 0;
        PIDS[i].KD = 0;
        PIDS[i].IMAX = 100;
        PIDS[i].IMIN = -100;
        PIDS[i].dState_reset = MOTOR_MIN_ROT_SPEED;
    }

    for (int i = PID_GYRO_Y; i < PID_GYRO_Y + GYRO_PID_BUNDLE_NUM; i ++)
    {
        PIDS[i].KP = 1000;
        PIDS[i].KI = 0;
        PIDS[i].KD = 0;
        PIDS[i].IMAX = 100;
        PIDS[i].IMIN = -100;
        PIDS[i].dState_reset = MOTOR_MIN_ROT_SPEED;
    }

    for (int i = PID_GYRO_Z; i < PID_GYRO_Z + GYRO_PID_BUNDLE_NUM; i ++)
    {
        PIDS[i].KP = 1000;
        PIDS[i].KI = 0;
        PIDS[i].KD = 0;
        PIDS[i].IMAX = 100;
        PIDS[i].IMIN = -100;
        PIDS[i].dState_reset = MOTOR_MIN_ROT_SPEED;
    }
}

LowLayerPIDLayer::~LowLayerPIDLayer() {}

void LowLayerPIDLayer::setNominalGyro(float gx, float gy, float gz)
{
    nom_gx = gx;
    nom_gy = gy;
    nom_gz = gz;
}

void LowLayerPIDLayer::iteratePIDS()
{
    float gx, gy, gz;
    float xerr[4], yerr[4], zerr[4];
    MotorController* mc = &motorController;

    SensorValue gyros = {GYRO, DEGREE};
    getSensorValue(&gyros);
    gx = gyros.Value[0];
    gy = gyros.Value[1];
    gz = gyros.Value[2];

    (void) xerr, (void) yerr, (void) zerr;

    // This block has not been configured and is not being used
    xerr[0] = PIDS[PID_GYRO_X + 0].updatePID(gx, nom_gx, mc->getMotorSpeed(MOTOR_NW + 0));
    xerr[1] = PIDS[PID_GYRO_X + 1].updatePID(gx, nom_gx, mc->getMotorSpeed(MOTOR_NW + 1));
    xerr[2] = PIDS[PID_GYRO_X + 2].updatePID(gx, nom_gx, mc->getMotorSpeed(MOTOR_NW + 2));
    xerr[3] = PIDS[PID_GYRO_X + 3].updatePID(gx, nom_gx, mc->getMotorSpeed(MOTOR_NW + 3));

    // This block HAS BEEN configured and IS BEING USED
    yerr[0] = PIDS[PID_GYRO_Y + 0].updatePID(gy, -nom_gy, mc->getMotorSpeed(MOTOR_NW + 0));
    yerr[1] = PIDS[PID_GYRO_Y + 1].updatePID(gy, -nom_gy, mc->getMotorSpeed(MOTOR_NW + 1));
    yerr[2] = PIDS[PID_GYRO_Y + 2].updatePID(gy,  nom_gy, mc->getMotorSpeed(MOTOR_NW + 2));
    yerr[3] = PIDS[PID_GYRO_Y + 3].updatePID(gy,  nom_gy, mc->getMotorSpeed(MOTOR_NW + 3));

    // This block has not been configured and is not being used
    zerr[0] = PIDS[PID_GYRO_Z + 0].updatePID(gz, nom_gz, mc->getMotorSpeed(MOTOR_NW + 0));
    zerr[1] = PIDS[PID_GYRO_Z + 1].updatePID(gz, nom_gz, mc->getMotorSpeed(MOTOR_NW + 1));
    zerr[2] = PIDS[PID_GYRO_Z + 2].updatePID(gz, nom_gz, mc->getMotorSpeed(MOTOR_NW + 2));
    zerr[3] = PIDS[PID_GYRO_Z + 3].updatePID(gz, nom_gz, mc->getMotorSpeed(MOTOR_NW + 3));

    // USED FOR TESTING GYRO Y-AXIS LEVELING
    mc->setOffsetFromNominalSpeed(MOTOR_NW + 0, yerr[0]);
    mc->setOffsetFromNominalSpeed(MOTOR_NW + 1, yerr[0]);
    mc->setOffsetFromNominalSpeed(MOTOR_NW + 2, yerr[0]);
    mc->setOffsetFromNominalSpeed(MOTOR_NW + 3, yerr[0]);

    #if 1
    if (getSystemState() == STATE_TAKEOFF)
    {
        #if 0
        Serial.print(mc->getMotorSpeed(MOTOR_NW));
        Serial.print("\t");
        Serial.print(mc->getMotorSpeed(MOTOR_NE));
        Serial.print("\t");
        Serial.print(mc->getMotorSpeed(MOTOR_SE));
        Serial.print("\t");
        Serial.print(mc->getMotorSpeed(MOTOR_SW));
        Serial.print("\t");

        Serial.print(nom_gy);
        Serial.print("\t");
        Serial.println(gy);
        #endif

        #if 1
        // Log the gyroscope readings
        // REMOVE WHEN DONE TESTING PID LOOPS
        actlog.log(nom_gy);
        gylog.log(gy);
        //////////////////////////////////////
        #endif
    }
    #endif
}

void LowLayerPIDLayer::resetPIDS()
{
    for (int i = PID_GYRO_X; i < PID_GYRO_X + NUM_GYRO_PIDS; i ++)
    {
        PIDS[i].resetPID();
    }
}

void LowLayerPIDLayer::getCurrentGyro(float& gx, float& gy, float& gz)
{
    imu.getGyroscope(gx, gy, gz);
}

bool LowLayerPIDLayer::setPID(int index, float p=-1, float i=-1, float d=-1)
{
    if (index >= NUM_GYRO_PIDS || index % GYRO_PID_BUNDLE_NUM != 0)
        return false;

    for (int j = index; j < index + GYRO_PID_BUNDLE_NUM; j ++)
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

bool LowLayerPIDLayer::getPID(int index, float* p, float* i, float* d)
{
    if (index >= NUM_GYRO_PIDS || index % GYRO_PID_BUNDLE_NUM != 0)
        return false;

    if (p != NULL)
        *p = PIDS[index].KP;
    if (i != NULL)
        *i = PIDS[index].KI;
    if (d != NULL)
        *d = PIDS[index].KD;

    return true;
}