#include "M_Input.h"

#include "App.h"
#include "M_Window.h"
#include "M_Editor.h"
#include "M_ResourceManager.h"

#include <SDL.h>

#define MAX_KEYS 300

M_Input::M_Input(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG("Input: Creation.");

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}


M_Input::~M_Input()
{
	_LOG("Input: Destroying.");
	RELEASE_ARRAY(keyboard);
}

bool M_Input::Init(JsonFile* file)
{
	_LOG("Input: Init.");

	bool ret = true;

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		_LOG("INPUT_ERROR: Could not init sdl events! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

UPDATE_RETURN M_Input::PreUpdate(float dt)
{
	UPDATE_RETURN ret = UPDT_CONTINUE;

	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

	mouseX /= app->win->GetWinScale();
	mouseY /= app->win->GetWinScale();
	wheelY = 0;
	mouseMotionX = mouseMotionY = 0;

	for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse[i] == KEY_IDLE)
				mouse[i] = KEY_DOWN;
			else
				mouse[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse[i] == KEY_REPEAT || mouse[i] == KEY_DOWN)
				mouse[i] = KEY_UP;
			else
				mouse[i] = KEY_IDLE;
		}
	}

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		app->editor->PassInput(&e);

		switch (e.type)
		{
		case SDL_MOUSEWHEEL:
			wheelY = e.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			mouseX = e.motion.x / app->win->GetWinScale();
			mouseY = e.motion.y / app->win->GetWinScale();
			mouseMotionX = e.motion.xrel / app->win->GetWinScale();
			mouseMotionY = e.motion.yrel / app->win->GetWinScale();
			break;

		case SDL_QUIT:
			ret = UPDT_STOP;
			break;

		case SDL_WINDOWEVENT:
		{
			switch (e.window.type)
			{
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				//TODO:Send event 
				;
			}
		}
		break;

		case SDL_DROPFILE:
		{
			char* dropped_file = e.drop.file;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped on window: ", 
				dropped_file, app->win->GetWindow());
			//app->resources->ImportFile(dropped_file);	//TODO: Should do this by events
			SDL_free(dropped_file);
		}
		break;

		}

	}

	SDL_PollEvent(&e);

	return ret;
}

bool M_Input::CleanUp()
{
	_LOG("Input: CleanUp.");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}
