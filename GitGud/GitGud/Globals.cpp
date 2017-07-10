#include "Globals.h"
#include <SDL.h>
#include "App.h"
#include <assert.h>

void _log(const char file[], int line, const char* format, ...)
{
	static char tmpString[4096];
	static char tmpString2[4096];
	static va_list ap;

	va_start(ap, format);
	vsprintf_s(tmpString, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmpString2, 4096, "\n%s(%d): %s", file, line, tmpString);

	//OutputDebugString(tmpString2);
	SDL_Log(tmpString2);

	if (app)
	{
		sprintf_s(tmpString2, 4096, "%s\n", tmpString);
		app->Log(tmpString2);
	}
}

const char* GetAppStateStr(APP_STATE state)
{
	static const char* states[] = {
		"PLAY", "STOP", "PAUSE", "WAITING_TO_PLAY", "WAITING_TO_STOP", "WAITING_TO_PAUSE", "WAITING_TO_UNPAUSE"
	};

	assert(state >= 0 && state <= WAITING_TO_UNPAUSE);

	return states[state];
}