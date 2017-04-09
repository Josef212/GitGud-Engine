#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"

class Timer
{
public:
	Timer();
	virtual ~Timer();

	void Start();
	void Stop();

	uint32 Read();
	float ReadSec();

private:
	bool running;
	uint32 startedAt;
	uint32 stopedAt;
};

#endif // !__TIMER_H__