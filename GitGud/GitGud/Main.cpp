#include <stdlib.h>
#include "Globals.h"
#include "App.h"

#ifdef TEST_MEMORY//_DEBUG
	#include "mmgr\mmgr.h"
#endif // DEBUG

#include "SDL.h"

enum MAIN_STATE
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
	_LOG("Starting engine %s from 'Josef21296'.", APP_TITLE);

	int mainRet = EXIT_FAILURE;
	MAIN_STATE state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			_LOG("--------------- APP Creation ---------------");
			app = new App(argc, argv);
			state = MAIN_START;

			break;

		case MAIN_START:

			_LOG("--------------- APP Init ---------------");
			if (!app->Init())
			{
				_LOG("App exits on init with errors!");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				_LOG("--------------- APP Update ---------------");
			}

			break;

		case MAIN_UPDATE:
		{
			UPDATE_RETURN tmp = app->Update();

			if (tmp == UPDT_ERROR)
			{
				_LOG("App exits on update with errors!");
				state = MAIN_EXIT;
			}

			if (tmp == UPDT_STOP)
				state = MAIN_FINISH;
		}			
			break;

		case MAIN_FINISH:
			_LOG("--------------- APP CleanUp ---------------");
			if (!app->CleanUp())
			{
				_LOG("App exits on cleanup with errors!");
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


#ifdef TEST_MEMORY
	int leaks = MAX(0, m_getMemoryStatistics().totalAllocUnitCount - 23);
	_LOG("Exiting with %d memory leaks!\n", (leaks > 0) ? leaks : 0);
#else
	_LOG("Exiting game engine! GITGUD :)");
#endif // _DEBUG


	return mainRet;
}