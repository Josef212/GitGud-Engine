#ifndef __M_WINDOW_H__
#define __M_WINDOW_H__


#include "Module.h"
#include "Console.h"
#include <SDL.h>

class M_Window : public Module
{
public:
	M_Window(const char* name, bool startEnabled = true);
	virtual ~M_Window();

	bool Init(JsonFile* file)override;
	bool CleanUp()override;

	void SetTitle(const char* title);

	SDL_Window* GetWindow()const { return window; }
	SDL_Surface* GetSurface()const { return screenSurface; }
	
	void HideWindow();
	void MaximizeWindow();
	void MinimizaWindow();
	void RestoreWindow();
	void ShowWindow();
	void RaiseWindow();

	void SetScreenSaver(bool set);
	void SetFullScreen(bool set);
	void SetResizable(bool set);
	void SetBorderless(bool set);
	void SetFullScreenDesktop(bool set);
	void SetBorder(bool set);
	void SetGrab(bool set);
	void SetPosition(int x, int y);

	void SetSize(int w, int h);
	void SetWidth(int width);
	void SetHeight(int height);
	bool SetBrightness(float bright);


	bool IsScreenSaver()const;
	bool IsFullScreen()const { return fullscreen; }
	bool IsResizable() const { return resizable; }
	bool IsBorderless() const { return borderless; }
	bool IsFullscreenDesktop() const { return fullscreenDesktop; }
	
	void GetPosition(int& w, int& h)const;

	uint GetWinScale()const { return winScale; }
	void GetWindowSize(int& width, int& height) const { width = this->width; height = this->height; }
	uint GetWidth()const { return width; }
	uint GetHeight()const { return height; }

	float GetBrightness()const;

	void GetRange(int& minW, int& minH, int& maxW, int& maxH)const;
	int GetRefresh()const;

private:

public:

private:
	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;

	uint winScale = 1;
	uint width = 0, height = 0;
	bool fullscreen, resizable, borderless, fullscreenDesktop;

	struct CResize : public Command
	{
		CResize() : Command("Resize window", "resize_win", "Resize the window: -w: width -h height")
		{}
		void Function(std::vector<std::string>& args)override;
	}cResize;

	struct CSetFlags : public Command
	{
		CSetFlags() : Command("Set win flags", "set_win_flags", "Set window flags: -f: fullscreen(1/0) -r: resizable(1/0) -b: borderless(1/0) -fd: fullscreen desktop(1/0)")
		{}
		void Function(std::vector<std::string>& args)override;
	}cSetFlags;

};


#endif // !__M_WINDOW_H__