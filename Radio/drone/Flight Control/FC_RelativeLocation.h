#ifndef __FC_RELATIVE_LOCATION_H__
#define __FC_RELATIVE_LOCATION_H__ 

#include "FlightControl.h"

class FC_RelativeLocation : public FlightControl
{
public:
	FC_RelativeLocation();
	~FC_RelativeLocation();
	
	void Do();
	bool CanDo();
};

#endif // __FC_RELATIVE_LOCATION_H__