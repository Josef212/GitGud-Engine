#include <stdlib.h>
#include "Globals.h"
#include "App.h"


#ifdef _DEBUG
	#include "mmgr\mmgr.h"
#endif // DEBUG

#include "SDL.h"

enum MainState
{
	MAIN_CREATION = 0,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

App* app = nullptr;

int main(int argc, char** argv)
{
	_LOG(LOG_INFO, "Starting engine %s from 'Josef21296'.", APP_TITLE);

	int mainRet = EXIT_FAILURE;
	MainState state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			_LOG(LOG_INFO, "--------------- APP Creation ---------------");
			app = new App(argc, argv);
			state = MAIN_START;

			break;

		case MAIN_START:

			_LOG(LOG_INFO, "--------------- APP Init ---------------");
			if (!app->Init())
			{
				_LOG(LOG_ERROR, "App exits on init with errors!");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				_LOG(LOG_INFO, "--------------- APP Update ---------------");
			}

			break;

		case MAIN_UPDATE:
		{
			UpdateReturn tmp = app->Update();

			if (tmp == UPDT_ERROR)
			{
				_LOG(LOG_ERROR, "App exits on update with errors!");
				state = MAIN_EXIT;
			}

			if (tmp == UPDT_STOP)
				state = MAIN_FINISH;
		}			
			break;

		case MAIN_FINISH:
			_LOG(LOG_INFO, "--------------- APP CleanUp ---------------");
			if (!app->CleanUp())
			{
				_LOG(LOG_ERROR, "App exits on cleanup with errors!");
			}
			else
			{
				mainRet = EXIT_SUCCESS;
			}

			state = MAIN_EXIT;

			break;
			
		}
	}

	RELEASE(app);


#ifdef _DEBUG
	int leaks = MAX(0, m_getMemoryStatistics().totalAllocUnitCount - 23);
	_LOG(LOG_WARN, "Exiting with %d memory leaks!\n", (leaks > 0) ? leaks : 0);
#else
	_LOG(LOG_INFO, "Exiting game engine! GITGUD :)");
#endif // _DEBUG


	return mainRet;
}