#ifndef __STATE_H__
#define __STATE_H__

typedef unsigned int UINT32;
typedef UINT32 State;

// Error states (High byte)
#define STATE_E_FAILSAFE            0x80000000
#define STATE_E_LAND                0x40000000
#define STATE_E_RETURN_HOME         0x20000000
#define STATE_E_ABORT               0x10000000
#define STATE_E_FAILED_INIT         0x08000000

// Normal states (Middle two bytes)
#define STATE_INIT                  0x00800000
#define STATE_ON_GROUND             0x00400000
#define STATE_TAKEOFF               0x00200000
#define STATE_NAVIGATING            0x00100000
#define STATE_AVOID                 0x00080000
#define STATE_LAND                  0x00040000
#define STATE_LANDING               0x00020000
#define STATE_RETURN_HOME           0x00010000
#define STATE_RESET                 0x00008000
#define STATE_VERIFY_ROUTE          0x00004000
#define STATE_PREFLIGHT             0x00002000
#define STATE_MOCK_PREFLIGHT        0x00001000

// System properties (Low byte)
#define PROPERTY_READY_FOR_TAKEOFF  0x00000002
#define PROPERTY_MAINTAIN_RADIO     0x00000001

void initSystemState();
void setSystemState(State state);
State getSystemState();
bool inSystemState(State state);

void maintainRadioContact(bool b);
void signalReadyForTakeoff(bool b);

bool mustMaintainRadioContact();
bool readyForTakeoff();

void setProperty(State property);
void resetProperty(State property);
bool isPropertySet(State property);

#endif // __STATE_H__
