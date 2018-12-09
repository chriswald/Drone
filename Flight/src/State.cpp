#include "State.h"

State SYSTEM_STATE;

#define PROPERTY_MASK 0xFF

void initSystemState()
{
    SYSTEM_STATE = STATE_INIT;
}

State getSystemState()
{
    return SYSTEM_STATE & 0xFFFFFF00;
}

bool inSystemState(State state)
{
    return SYSTEM_STATE & state;
}

void maintainRadioContact(bool b)
{
    if (b)
        SYSTEM_STATE |= PROPERTY_MAINTAIN_RADIO;
    else
        SYSTEM_STATE &= ~PROPERTY_MAINTAIN_RADIO;
}

void signalReadyForTakeoff(bool b)
{
    if (b)
        SYSTEM_STATE |= PROPERTY_READY_FOR_TAKEOFF;
    else
        SYSTEM_STATE &= ~PROPERTY_READY_FOR_TAKEOFF;
}

void setSystemState(State state)
{
    SYSTEM_STATE = (state) | (SYSTEM_STATE & PROPERTY_MASK);
}

bool mustMaintainRadioContact()
{
    return SYSTEM_STATE & PROPERTY_MAINTAIN_RADIO;
}

bool readyForTakeoff()
{
    return SYSTEM_STATE & PROPERTY_READY_FOR_TAKEOFF;
}

void setProperty(State property)
{
    SYSTEM_STATE |= property;
}

void resetProperty(State property)
{
    SYSTEM_STATE &= ~property;
}

bool isPropertySet(State property)
{
    return SYSTEM_STATE & property;
}
