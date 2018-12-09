#include "PID.h"

float PID::updatePID(float current, float desired, float position)
{
    float pterm,iterm,dterm;
    float error = desired - current;
    pterm = KP * error;
    
    iSum += error;
    if( iSum > IMAX)
        iSum = IMAX;
    else if( iSum < IMIN)
        iSum = IMIN;
    iterm = KI * iSum;
    
    dterm = KD * (dState - position);
    dState = position;
    
    return (pterm + iterm + dterm);
}

void PID::resetPID()
{
    iSum = 0;
    dState = dState_reset;
}