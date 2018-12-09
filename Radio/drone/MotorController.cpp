#include "MotorController.h"

MotorController motorController;

#define ARMING_DELAY 1100 //us
#define ARMING_TIME 5 //sec
#define TIMER_REFRESH 5000 //us

// Pairs a motor's control signal pin with its speed (1100-2000);
typedef struct {
    unsigned speed;
    unsigned char pin;
} Motor;

// This has to be a global - rather than part of MotorController
// -- so the IntervalTimer functions can access it.
static Motor motors[4];

IntervalTimer speedTimer;
IntervalTimer stopSpeedTimer;
unsigned mindex = 0;

void startSendMotorSpeed();
void stopSendMotorSpeed();

void stopSendMotorSpeed()
{
    stopSpeedTimer.end();
    digitalWrite(motors[mindex].pin, LOW);
    mindex = (mindex + 1) % NUM_MOTORS;
}

void startSendMotorSpeed()
{
    digitalWrite(motors[mindex].pin, HIGH);
    if (motors[mindex].speed != 0)
        stopSpeedTimer.begin(stopSendMotorSpeed, motors[mindex].speed);
    else
        mindex = (mindex + 1) % NUM_MOTORS;
}

MotorController::MotorController()
{
    armedState = false;
    motors[MOTOR_NW].pin = MOTOR_NW_PIN;
    motors[MOTOR_NE].pin = MOTOR_NE_PIN;
    motors[MOTOR_SE].pin = MOTOR_SE_PIN;
    motors[MOTOR_SW].pin = MOTOR_SW_PIN;

    motors[MOTOR_NW].speed = MOTOR_NULL;
    motors[MOTOR_NE].speed = MOTOR_NULL;
    motors[MOTOR_SE].speed = MOTOR_NULL;
    motors[MOTOR_SW].speed = MOTOR_NULL;
}

MotorController::~MotorController() {}

void MotorController::armMotors()
{
    Serial.println("*** Arming");
    speedTimer.end();

    pinMode(motors[MOTOR_NW].pin, OUTPUT);
    pinMode(motors[MOTOR_NE].pin, OUTPUT);
    pinMode(motors[MOTOR_SE].pin, OUTPUT);
    pinMode(motors[MOTOR_SW].pin, OUTPUT);

    for (int arming_time = 0; arming_time < 50 * ARMING_TIME; arming_time ++)
    {
        digitalWrite(motors[0].pin, HIGH);
        digitalWrite(motors[1].pin, HIGH);
        digitalWrite(motors[2].pin, HIGH);
        digitalWrite(motors[3].pin, HIGH);
        delayMicroseconds(ARMING_DELAY);
        digitalWrite(motors[0].pin, LOW);
        digitalWrite(motors[1].pin, LOW);
        digitalWrite(motors[2].pin, LOW);
        digitalWrite(motors[3].pin, LOW);
        delayMicroseconds(20000 - ARMING_DELAY);
    }

    if (motors[0].speed == MOTOR_NULL)
    {
        motors[0].speed = MOTOR_ZERO_SPEED;
        motors[1].speed = MOTOR_ZERO_SPEED;
        motors[2].speed = MOTOR_ZERO_SPEED;
        motors[3].speed = MOTOR_ZERO_SPEED;
    }

    speedTimer.begin(startSendMotorSpeed, TIMER_REFRESH);

    armedState = true;
    Serial.println("*** Arming Complete");
}

bool MotorController::getMotorsArmState()
{
    return armedState;
}

bool MotorController::setMotorSpeed(int motor, unsigned speed)
{
    if (motor >= NUM_MOTORS)
        return false;

    if (speed > MOTOR_MAX_ROT_SPEED)
        speed = MOTOR_MAX_ROT_SPEED;
    if (speed < MOTOR_ZERO_SPEED)
        speed = MOTOR_ZERO_SPEED;

    noInterrupts();
    motors[motor].speed = speed;
    interrupts();

    return true;
}

unsigned MotorController::getMotorSpeed(int motor)
{
    if (motor >= NUM_MOTORS)
        return -1;
    return motors[motor].speed;
}

void MotorController::setNominalSpeed(unsigned nominal)
{
    if (nominal > MOTOR_MAX_ROT_SPEED)
        nominal = MOTOR_MAX_ROT_SPEED;
    else if (nominal < MOTOR_ZERO_SPEED)
        nominal = MOTOR_ZERO_SPEED;

    nominalSpeed = nominal;
}

unsigned MotorController::getNominalSpeed()
{
    return nominalSpeed;
}

bool MotorController::setOffsetFromNominalSpeed(int motor, int delta)
{
    return setMotorSpeed(motor, getNominalSpeed() + delta);
}

bool MotorController::setMotorSpeedDelta(int motor, int delta)
{
    if (motor >= NUM_MOTORS)
        return false;

    return setMotorSpeed(motor, motors[motor].speed + delta);
}