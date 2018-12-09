#ifndef __FLIGHT_CONTROL_H__
#define __FLIGHT_CONTROL_H__ 

class FlightControl
{
public:
	FlightControl();
	~FlightControl();
	
	virtual void Do() = 0;
	virtual bool CanDo() = 0;
};

#endif // __FLIGHT_CONTROL_H__