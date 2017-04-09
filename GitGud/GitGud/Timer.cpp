#include "Timer.h"
#include <SDL_timer.h>


Timer::Timer()
{
	Start();
}


Timer::~Timer()
{
}

void Timer::Start()
{
	running = true;
	startedAt = SDL_GetTicks();
}

void Timer::Stop()
{
	running = false;
	stopedAt = SDL_GetTicks();
}

uint32 Timer::Read()
{
	if (running)
	{
		return SDL_GetTicks() - startedAt;
	}
	else
	{
		return stopedAt - startedAt;
	}
}

float Timer::ReadSec()
{
	return (float)Read() / 1000.0f;
}
