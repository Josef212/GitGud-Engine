#include "App.h"

#include "HrdInfo.h"

#include "Module.h"
#include "M_FileSystem.h"
#include "M_Window.h"
#include "M_Input.h"
#include "M_Editor.h"



#include "M_Renderer.h"

#include <iostream>


App::App(int argv, char** argc)
{
	_LOG("App: Creation  =======================");
	this->argv = argv;
	for (uint i = 0; i < argv; ++i)
	{
		this->argc.push_back(argc[i]);
	}
	ReadArgs();

	info = new HrdInfo();

	//Create modules
	editor = new M_Editor("module_editor");
	fs = new M_FileSystem("module_file_system");
	win = new M_Window("module_window");
	input = new M_Input("module_input");


	renderer = new M_Renderer("module_renderer");


	modules.push_back(editor);
	modules.push_back(fs);
	modules.push_back(win);
	modules.push_back(input);


	modules.push_back(renderer);
}


App::~App()
{
	_LOG("App: Destruction  =======================");
	for (std::vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); ++it)
		RELEASE(*it);
}

bool App::Init()
{
	bool ret = true;
	_LOG("App: Init  =======================");

	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		ret = (*it)->Init();
	}

	_LOG("App: Start  =======================");
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnable())
			ret = (*it)->Start();
	}

	if (ret)
		info->SetInfo();

	return ret;
}

UPDATE_RETURN App::Update()
{
	UPDATE_RETURN ret = UPDT_CONTINUE;

	PrepareUpdate();

	std::vector<Module*>::iterator it;
	for (it = modules.begin(); it != modules.end() && ret == UPDT_CONTINUE; ++it)
	{
		if ((*it)->IsEnable())
			ret = (*it)->PreUpdate(dt);
	}

	if (ret == UPDT_ERROR)
		_LOG("ERROR: Exit preupdate with errors.");

	for (it = modules.begin(); it != modules.end() && ret == UPDT_CONTINUE; ++it)
	{
		if ((*it)->IsEnable())
			ret = (*it)->Update(dt);
	}

	if (ret == UPDT_ERROR)
		_LOG("ERROR: Exit update with errors.");

	for (it = modules.begin(); it != modules.end() && ret == UPDT_CONTINUE; ++it)
	{
		if ((*it)->IsEnable())
			ret = (*it)->PostUpdate(dt);
	}

	if (ret == UPDT_ERROR)
		_LOG("ERROR: Exit postupdate with errors.");

	FinishUpdate();

	if (quit)
		ret = UPDT_STOP;

	return ret;
}

bool App::CleanUp()
{
	bool ret = true;
	_LOG("App: CleanUp  =======================");

	for (std::vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); ++it)
	{
		ret = (*it)->CleanUp();
	}

	return ret;
}

void App::DrawDebug()
{
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		if((*it)->IsEnable())
			(*it)->DrawDebug();
	}
}

const char * App::GetTitle() const
{
	return title.c_str();
}

const char * App::GetOrganitzation() const
{
	return organitzation.c_str();
}

void App::SetTitle(const char * str)
{
	if (str)
	{
		title = str;
		if (win) win->SetTitle(str);
	}
}

void App::SetOrganitzation(const char * str)
{
	if (str)
	{
		organitzation = str;
		//TODO: Set title to window and file
	}
}

uint App::GetMaxFPS() const
{
	if (cappedMs > 0)
		return (uint)((1.0f / (float)cappedMs) * 1000.0f);
	else
		return 0;
}

void App::SetMaxFPS(uint _fps)
{
	if (_fps > 0)
		cappedMs = 1000 / _fps;
	else
		cappedMs = 0;
}

void App::Log(const char * str)
{
	if (editor)
		editor->Log(str);
}

void App::Browse(const char * url)
{
	ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
}

void App::Save(std::string saveFile)
{
	if (saveFile != currentConfigSaveFileDir)
		currentConfigSaveFileDir = saveFile;

	saveNextFrame = true;
}

void App::Load(std::string saveFile)
{
	if (saveFile != currentConfigSaveFileDir)
		currentConfigSaveFileDir = saveFile;

	loadNextFrame = true;
}

void App::PrepareUpdate()
{
	dt = (float)msTimer.ReadSec();
	msTimer.Start();
}

void App::FinishUpdate()
{
	if (saveNextFrame)
	{
		SaveNow();
		saveNextFrame = false;
	}

	if (loadNextFrame)
	{
		LoadNow();
		loadNextFrame = false;
	}


	++frames;
	++fpsCounter;

	if (fpsTimer.Read() >= 1000)
	{
		lastFps = fpsCounter;
		fpsCounter = 0;
		fpsTimer.Start();
	}

	lastFrameMs = msTimer.Read();

	if (cappedMs > 0 && (lastFrameMs < cappedMs))
		SDL_Delay(cappedMs - lastFrameMs);

	if (editor)
		editor->LogFPS((float)lastFps, (float)lastFrameMs);
}

void App::ReadConfig()
{
}

void App::ReadArgs()
{
	for (uint i = 0; i < argv; ++i)
	{
		std::cout << "Arg " << i << ": " << argc[i] << std::endl;
	}
}

bool App::SaveNow()
{
	return false;
}

bool App::LoadNow()
{
	return false;
}
