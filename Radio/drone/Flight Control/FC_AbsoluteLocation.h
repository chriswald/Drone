#ifndef __FC_ABSOLUTE_LOCATION_H__
#define __FC_ABSOLUTE_LOCATION_H__ 

#include "FlightControl.h"

class FC_AbsoluteLocation : public FlightControl
{
public:
	FC_AbsoluteLocation();
	~FC_AbsoluteLocation();
	
	void Do();
	bool CanDo();
};

#endif // __FC_ABSOLUTE_LOCATION_H__