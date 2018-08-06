#include "Globals.h"
#include <SDL.h>
#include "App.h"
#include <assert.h>
#include "MathGeoLib\include\Time\Clock.h"

void _log(LogType type, const char file[], int line, const char* format, ...)
{
	static char tmpString[4096];
	static char tmpString2[4096];
	static char timeStr[1024];
	static va_list ap;

	va_start(ap, format);
	vsprintf_s(tmpString, 4096, format, ap);
	va_end(ap);

	sprintf_s(timeStr, 1024, "(%d-%d-%d %d:%d:%d)", Clock::Day(), Clock::Month(), Clock::Year(), Clock::Hour(), Clock::Min(), Clock::Sec());

	sprintf_s(tmpString2, 4096, "\n%s %s::%s(%d): %s", timeStr, GetLogTypeStr(type), file, line, tmpString);

	//OutputDebugString(tmpString2);
	SDL_Log(tmpString2);

	if (app)
	{
		sprintf_s(tmpString2, 4096, "%s %s::%s\n", timeStr, GetLogTypeStr(type), tmpString);
		app->Log(tmpString2, type);
	}
}

const char* GetLogTypeStr(LogType type)
{
	static const char* types[] = {
		"INFO", "WARN", "ERROR", "CMD"
	};

	assert(type >= 0 && type < LOG_MAX);

	return types[type];
}

const char* GetAppStateStr(AppState state)
{
	static const char* states[] = {
		"PLAY", "STOP", "PAUSE", "WAITING_TO_PLAY", "WAITING_TO_STOP", "WAITING_TO_PAUSE", "WAITING_TO_UNPAUSE"
	};

	assert(state >= 0 && state <= WAITING_TO_UNPAUSE);

	return states[state];
}