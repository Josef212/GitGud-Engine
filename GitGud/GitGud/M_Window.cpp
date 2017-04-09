#include "M_Window.h"

#include "App.h"



M_Window::M_Window(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG("Window: Creation.");
}


M_Window::~M_Window()
{
	_LOG("Window: Destroying.");
}

bool M_Window::Init()
{
	_LOG("Window: Init");
	bool ret = false;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		_LOG("WIN_Error: SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//TODO: Config
		winScale = 1; 
		width = 1920;
		height = 1080;

		fullscreen = false;
		resizable = false;
		borderless = false;
		fullscreenDesktop = false;

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		if (fullscreen)
			flags |= SDL_WINDOW_FULLSCREEN;
		if (resizable)
			flags |= SDL_WINDOW_RESIZABLE;
		if (borderless)
			flags |= SDL_WINDOW_BORDERLESS;
		if (fullscreenDesktop && !fullscreen)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (!window)
		{
			_LOG("WIN_Error: Could not create window! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
			ret = true;
		}
	}

	return ret;
}

bool M_Window::CleanUp()
{
	_LOG("Window: CleanUp");

	if (window)
		SDL_DestroyWindow(window);

	SDL_Quit();

	return true;
}

void M_Window::SetTitle(const char * title)
{
	if(title && window)
		SDL_SetWindowTitle(window, title);
}

void M_Window::HideWindow()
{
	if (window)SDL_HideWindow(window);
}

void M_Window::MaximizeWindow()
{
	if (window)SDL_MaximizeWindow(window);
}

void M_Window::MinimizaWindow()
{
	if (window)SDL_MinimizeWindow(window);
}

void M_Window::RestoreWindow()
{
	if (window)SDL_RestoreWindow(window);
}

void M_Window::ShowWindow()
{
	if (window)SDL_ShowWindow(window);
}

void M_Window::RaiseWindow()
{
	if (window)SDL_RaiseWindow(window);
}

void M_Window::SetScreenSaver(bool set)
{
	if (set)
		SDL_EnableScreenSaver();
	else
		SDL_DisableScreenSaver();
}

void M_Window::SetFullScreen(bool set)
{
	if (window)
	{
		if (set != fullscreen)
		{
			fullscreen = set;
			if (fullscreen == true)
			{
				if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
					_LOG("Could not switch to fullscreen: %s\n", SDL_GetError());
				fullscreenDesktop = false;
			}
			else
			{
				if (SDL_SetWindowFullscreen(window, 0) != 0)
					_LOG("Could not switch to windowed: %s\n", SDL_GetError());
			}
		}
	}
	else
	{
		_LOG("Error setting fullscreen, window pointer is nullptr");
	}
}

void M_Window::SetResizable(bool set)
{
	resizable = set; //cant change this, save it to load it on next exe
}

void M_Window::SetBorderless(bool set)
{
	if (set != borderless && !fullscreen && !fullscreenDesktop)
	{
		borderless = set;
		SDL_SetWindowBordered(window, (SDL_bool)!borderless);
	}
}

void M_Window::SetFullScreenDesktop(bool set)
{
	if (set != fullscreenDesktop)
	{
		fullscreenDesktop = set;
		if (fullscreenDesktop)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				_LOG("Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				_LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

void M_Window::SetBorder(bool set)
{
	if (SDL_GetWindowFlags(window) != SDL_WINDOW_FULLSCREEN)
	{
		if (set)
			SDL_SetWindowBordered(window, SDL_TRUE);
		else
			SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else
		_LOG("Could not set border, because of fullscreen.");
}

void M_Window::SetGrab(bool set)
{
	if (window)
	{
		if (set)
			SDL_SetWindowGrab(window, SDL_TRUE);
		else
			SDL_SetWindowGrab(window, SDL_FALSE);
	}
}

void M_Window::SetPosition(int x, int y)
{
	if (window)
	{
		SDL_SetWindowPosition(window, x, y);
	}
}

void M_Window::SetSize(int w, int h)
{
	if (window && w >= 0 && h >= 0)
	{
		width = w;
		height = h;
		SDL_SetWindowSize(window, w, h);
	}
}

void M_Window::SetWidth(int width)
{
	if (width > 0)
	{
		this->width = width;
		SetSize(width, height);
	}
}

void M_Window::SetHeight(int height)
{
	if (height > 0)
	{
		this->height = height;
		SetSize(width, height);
	}
}

bool M_Window::SetBrightness(float bright)
{
	bool ret = true;

	if (bright < 0.0f)
		bright = 0.0f;
	if (bright > 1.0f)
		bright = 1.0f;
	if (window)
	{
		if (SDL_SetWindowBrightness(window, bright) < 0)
		{
			ret = false;
			_LOG("Error setting brightness, SDL_Error: %s.\n", SDL_GetError());
		}
	}
	else
		_LOG("Error setting brightness, window pointer is nullptr");

	return ret;
}

bool M_Window::IsScreenSaver() const
{
	return (bool)SDL_IsScreenSaverEnabled();
}

void M_Window::GetPosition(int & w, int & h) const
{
	if (window)
	{
		SDL_GetWindowPosition(window, &w, &h);
	}
}

float M_Window::GetBrightness() const
{
	return SDL_GetWindowBrightness(window);
}

void M_Window::GetRange(int & minW, int & minH, int & maxW, int & maxH) const
{
	minW = 640;
	minH = 480;
	maxW = 3000;
	maxH = 2000;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		_LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else
	{
		maxW = dm.w;
		maxH = dm.h;
	}
}

int M_Window::GetRefresh() const
{
	uint ret = 0;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		_LOG("Error getting refresh rate: %s", SDL_GetError());
	}
	else
		ret = dm.refresh_rate;

	return ret;
}
