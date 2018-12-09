#ifndef __PID_H__
#define __PID_H__

class PID {
public:
    float updatePID(float current, float desired, float position);
    void  resetPID();

    float KP, KI, KD;
    float IMAX, IMIN;
    float iSum;
    float dState;
    float dState_reset;
};

#endif // __PID_H__
