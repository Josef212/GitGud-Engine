#include "GG_Clock.h"
#include "PerfTimer.h"


GG_Clock::GG_Clock()
{
	msTimer = new PerfTimer();
	msGameTimer = new PerfTimer();
	fpsTimer = new PerfTimer();
}

GG_Clock::~GG_Clock()
{
	RELEASE(msTimer);
	RELEASE(msGameTimer);
	RELEASE(fpsTimer);
}

/**
*	- OnPrepareUpdate: Calculate all attributes at the begging of each frame
*		- Add the elapsed time.
*		- Recal real dt.
*		- Add one frame to counter.
*		- If app state is PLAY do the same with the game timer.
*/
void GG_Clock::OnPrepareUpdate(AppState appState)
{
	//1. Add time
	timeSinceAppStart += realDt;
	timeSinceLevelLoaded += realDt;
	//2. Calc dt
	realDt = (float)(msTimer->ReadMs() / 1000.0f);
	if (realDt > maximumDT) realDt = 1 / 30.0f;
	msTimer->Start();
	//3. Add a frame
	++realFrameCount;

	if (appState == AppState::PLAY)
	{
		gameTimeSinceLevelLoaded += gameDt;

		gameDt = ((float)(msGameTimer->ReadMs() / 1000.0f)) * scale;
		msGameTimer->Start();

		++gameFrameCount;
	}
}

/**
*	- OnFinishUpdate: Calculate all attributes at the ending of the frame.
*		- Add one to fps counter.
*		- Check if one second has passed to calculate fps.
*		- Calculate last frame ms.
*/
void GG_Clock::OnFinishUpdate()
{
	++fpsCounter;

	if (fpsTimer->ReadMs() > 1000.0f)
	{
		lastFps = fpsCounter;
		fpsCounter = 0;
		fpsTimer->Start();
	}

	lastFrameMs = msTimer->ReadMs();
}

void GG_Clock::OnSceneLoaded()
{
	timeSinceLevelLoaded = 0.0;
}

void GG_Clock::Pause()
{
	//TODO: Check this, make sense to stop the time even if u pause?? (Scene since level started should continue even if u pause)
	//msGameTimer->Stop();
}

void GG_Clock::UnPause()
{
	msGameTimer->Start();
}

void GG_Clock::Stop()
{
	msGameTimer->Stop();
	gameFrameCount = 0;
	gameDt = 0.0f;
}

void GG_Clock::Play()
{
	msGameTimer->Start();
}

/**
*	- DT: Return real dt.
*/
float GG_Clock::DT() const
{
	return realDt;
}

/**
*	- RealFrameCount: Return the real frames passed.
*/
uint GG_Clock::RealFrameCount() const
{
	return realFrameCount;
}

/**
*	- FPS: Return the fps counter.
*/
uint GG_Clock::FPS() const
{
	return fpsCounter;
}

/**
*	- LastFPS: Return the last frame fps counter.
*/
uint GG_Clock::LastFPS() const
{
	return lastFps;
}

/**
*	- TimeSinceStart: Return the time in seconds since app started.
*/
float GG_Clock::TimeSinceStart() const
{
	return timeSinceAppStart;
}

/**
*	- TimeSinceSceneLoaded: Return the time in seconds since scene was loaded.
*/
float GG_Clock::TimeSinceSceneLoaded() const
{
	return timeSinceLevelLoaded;
}

/**
*	- GameDT: Return the game dt.
*/
float GG_Clock::GameDT() const
{
	return gameDt;
}

/**
*	- GameFrameCounter: Return the game frames passed.
*/
uint GG_Clock::GameFrameCounter() const
{
	return gameFrameCount;
}

/**
*	- TimeSinceSceneLoaded: Return the time in seconds since scene was loaded in game scale.
*/
float GG_Clock::GameTimeSinceLevelLoaded() const
{
	return gameTimeSinceLevelLoaded;
}

/**
*	- GetScale: Return the game time scale.
*/
float GG_Clock::GetScale() const
{
	return scale;
}

/**
*	- LastFrameMs: Return the last frame ms.
*/
float GG_Clock::LastFrameMs() const
{
	return lastFrameMs;
}

/**
*	- SetScale: Set the game time scale.
*/
void GG_Clock::SetScale(float scl)
{
	if (scl >= 0.0f)
		scale = scl;
	else
		scale = 0.0f;
}
