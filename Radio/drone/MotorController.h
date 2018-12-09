#ifndef __MOTOR_CONTROLLER_H__
#define __MOTOR_CONTROLLER_H__

#include <Arduino.h>
#include "SensorPins.h"

#define MOTOR_NULL          0
#define MOTOR_ZERO_SPEED    1100
#define MOTOR_MIN_ROT_SPEED 1135
#define MOTOR_MAX_ROT_SPEED 1500 //2000

// How far off the nominal speed is a single motor allowed to be?
// This is just a guess.
#define MOTOR_NOMINAL_BOUND 50

class MotorController
{
public:
    MotorController();
    ~MotorController();

    void armMotors();
    bool getMotorsArmState();

    bool setMotorSpeed(int motor, unsigned speed);
    unsigned getMotorSpeed(int motor);
    void setNominalSpeed(unsigned nominal);
    unsigned getNominalSpeed();
    bool setOffsetFromNominalSpeed(int motor, int delta);

private:
    bool armedState;
    unsigned nominalSpeed;

    bool setMotorSpeedDelta(int motor, int delta);
};

extern MotorController motorController;

#endif //__MOTOR_CONTROLLER_H__