#ifndef __FC_TIMED_H__
#define __FC_TIMED_H__ 

#include "FlightControl.h"

class FC_Timed : public FlightControl
{
public:
	FC_Timed();
	~FC_Timed();
	
	void Do();
	bool CanDo();
};

#endif // __FC_TIMED_H__