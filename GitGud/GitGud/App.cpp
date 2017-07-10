#include "App.h"

#include "HrdInfo.h"
#include "Console.h"
#include "RandGen.h"

#include "JsonFile.h"

#include "AllModules.h"

#include <iostream>

//-----------------------------------------------

struct C_Quit : public Command
{
	C_Quit() : Command("quit", "exit", "Close app")
	{}

	void Function(std::vector< std::string>& args)override
	{
		if (app)app->quit = true;
	}
}c_Quit;
//-----------------------------------------------

/**
*	- App constructor.
*	- Read arguments.
*	- Create info container, console and random generator.
*	- Create all modules and add them to the vector.
*/
App::App(int argv, char** argc) : currentConfigSaveFileDir(CONFIG_PATH + (std::string("config.json")))
{
	_LOG("App: Creation  =======================");
	this->argv = argv;
	for (uint i = 0; i < argv; ++i)
	{
		this->argc.push_back(argc[i]);
	}
	ReadArgs();

	clock = new GG_Clock();
	info = new HrdInfo();
	console = new Console();
	random = new RandGen();
	//TODO: Let user pass an arg for the seed??

	console->AddCommand(&c_Quit);

	//Create modules
	editor = new M_Editor("module_editor");
	fs = new M_FileSystem("module_file_system");
	win = new M_Window("module_window");
	input = new M_Input("module_input");
	resources = new M_ResourceManager("module_resource_manager");
	goManager = new M_GoManager("module_go_manager");
	camera = new M_Camera3D("module_camera_editor");



	renderer = new M_Renderer("module_renderer");


	modules.push_back(editor);
	modules.push_back(fs);
	modules.push_back(win);
	modules.push_back(input);
	modules.push_back(resources);
	modules.push_back(goManager);
	modules.push_back(camera);


	modules.push_back(renderer);
}

/**
*	- App destructor.
*	- Release all the memory allocated in the class.
*/
App::~App()
{
	_LOG("App: Destruction  =======================");
	for (std::vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); ++it)
		RELEASE(*it);

	RELEASE(info);
	RELEASE(console);
	RELEASE(random);
	RELEASE(clock);
}

/**
*	- App Init.
*	- Read configuration.
*	- Call all modules inits passing the configuration.
*	- Call all modules starts.
*/
bool App::Init()
{
	bool ret = true;
	_LOG("App: Init  =======================");

	char* buffer = nullptr;
	fs->Load(currentConfigSaveFileDir.c_str(), &buffer);

	JsonFile config(buffer);
	ReadConfig(&config.GetSection("app"));

	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		ret = (*it)->Init(&config.GetSection((*it)->name.c_str()));
	}

	_LOG("App: Start  =======================");
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnable())
			ret = (*it)->Start();
	}

	RELEASE_ARRAY(buffer);

	if (ret)
		info->SetInfo();

	return ret;
}

/**
*	- App update.
*	- Call modules preupdate, update and postupdate.
*	- Check for exit.
*/
UPDATE_RETURN App::Update()
{
	UPDATE_RETURN ret = UPDT_CONTINUE;

	PrepareUpdate();

	std::vector<Module*>::iterator it;
	for (it = modules.begin(); it != modules.end() && ret == UPDT_CONTINUE; ++it)
	{
		if ((*it)->IsEnable())
			ret = (*it)->PreUpdate(clock->DT()); //TODO: Dont pass dt, let each module get each dt
	}

	if (ret == UPDT_ERROR)
		_LOG("ERROR: Exit preupdate with errors.");

	for (it = modules.begin(); it != modules.end() && ret == UPDT_CONTINUE; ++it)
	{
		if ((*it)->IsEnable())
			ret = (*it)->Update(clock->DT()); //TODO: Dont pass dt, let each module get each dt
	}

	if (ret == UPDT_ERROR)
		_LOG("ERROR: Exit update with errors.");

	for (it = modules.begin(); it != modules.end() && ret == UPDT_CONTINUE; ++it)
	{
		if ((*it)->IsEnable())
			ret = (*it)->PostUpdate(clock->DT()); //TODO: Dont pass dt, let each module get each dt
	}

	if (ret == UPDT_ERROR)
		_LOG("ERROR: Exit postupdate with errors.");

	FinishUpdate();

	if (quit)
		ret = UPDT_STOP;

	return ret;
}

/**
*	- App CleanUp.
*	- Call all modules cleanup.
*/
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

/**
*	- App DrawDebug.
*	- Call all active modules drawdebug.
*/
void App::DrawDebug()
{
	for (std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		if((*it)->IsEnable())
			(*it)->DrawDebug();
	}
}

/**
*	- GetTitle: Return the app title.
*/
const char * App::GetTitle() const
{
	return title.c_str();
}

/*
*	- GetOrganitzation: Return app organitzation name.
*/
const char * App::GetOrganitzation() const
{
	return organitzation.c_str();
}

/**
*	- SetTitle: Set the app title.
*/
void App::SetTitle(const char * str)
{
	if (str)
	{
		title = str;
		if (win) win->SetTitle(str);
	}
}

/**
*	- SetOrganitzation: Set app roganitzation.
*/
void App::SetOrganitzation(const char * str)
{
	if (str)
	{
		organitzation = str;
		//TODO: Set title to window and file system
	}
}

/**
*	- GetMaxFPS: Return app max fps.
*/
uint App::GetMaxFPS() const
{
	if (cappedMs > 0)
		return (uint)((1.0f / (float)cappedMs) * 1000.0f);
	else
		return 0;
}

/**
*	- SetMaxFPS: Set app max fps.
*/
void App::SetMaxFPS(uint _fps)
{
	if (_fps > 0)
		cappedMs = 1000 / _fps;
	else
		cappedMs = 0;
}

/**
*	- Log: Log to editor. 
*/
void App::Log(const char * str)
{
	if (editor)
		editor->Log(str);
}

/**
*	- Browse: Open a link in the browser.
*/
void App::Browse(const char * url)
{
	ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
}

/**
*	- Save: Prepare app to save and set the config save name.
*/
void App::Save(std::string saveFile)
{
	if (saveFile != currentConfigSaveFileDir)
		currentConfigSaveFileDir = saveFile;

	saveNextFrame = true;
}

/**
*	- Load: Prepare app to load and set the config file name.
*/
void App::Load(std::string saveFile)
{
	if (saveFile != currentConfigSaveFileDir)
		currentConfigSaveFileDir = saveFile;

	loadNextFrame = true;
}

/**
*	- AddCommand: Add a command to the console.
*/
void App::AddCommand(Command * cmd)
{
	if (console && cmd)
		console->AddCommand(cmd);
}

/**
*	- GetConfigSavePath: Return the config file name path.
*/
const char * App::GetConfigSavePath() const
{
	return currentConfigSaveFileDir.c_str();
}

/**
*	- SetConfigSavePath: Set the config file name path.
*/
void App::SetConfigSavePath(const char * path)
{
	if (path)
		currentConfigSaveFileDir = path;
}

/**
*	- ResetConfig: Load the default config.
*/
void App::ResetConfig()
{
	currentConfigSaveFileDir = (CONFIG_PATH + std::string("default_config.json"));
}

/**
*	- PrepareUpdate: Calculate dt.
*/
void App::PrepareUpdate()
{
	clock->OnPrepareUpdate(PLAY_STATE::STOP); //TODO: Pass the app state
}

/**
*	- FinishUpdate: Calc fps and other metrics.
*	- Delay if necessary.
*	- Do Save or Load if marked to.
*/
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


	clock->OnFinishUpdate();

	if (cappedMs > 0 && (clock->LastFrameMs() < cappedMs))
		SDL_Delay(cappedMs - clock->LastFrameMs());

	if (editor)
		editor->LogFPS((float)clock->LastFPS(), (float)clock->LastFrameMs());
}

/**
*	- ReadConfig: Read the app config.
*/
void App::ReadConfig(JsonFile* config)
{
	if (!config)return;

	SetMaxFPS(config->GetInt("fps_limit", 0));
	SetTitle(config->GetString("app_title", "GitGud"));
	SetOrganitzation(config->GetString("app_organitzation", "Josef21296"));
}

/**
*	- ReadArgs: Read args and set parameters.
*/
void App::ReadArgs()
{
	for (uint i = 0; i < argv; ++i)
	{
		std::cout << "Arg " << i << ": " << argc[i] << std::endl;
	}
}

/**
*	- SaveNow: Do save.
*/
bool App::SaveNow()
{
	bool ret = true;

	JsonFile file;
	JsonFile app = file.AddSection("app");
	app.AddInt("fps_limit", GetMaxFPS());
	app.AddString("app_title", title.c_str());
	app.AddString("app_organitzation", organitzation.c_str());

	for (auto mod : modules)
	{
		ret = mod->Save(&file.AddSection(mod->name.c_str()));
	}
	
	char* buffer = nullptr;
	uint size = file.WriteJson(&buffer);

	if (size > 0 && buffer)
	{
		if (fs->Save(currentConfigSaveFileDir.c_str(), buffer, size) != size)
		{
			_LOG("APP_ERROR: Could not save config.");
			ret = false;
		}
	}
	else
	{
		ret = false;
	}

	RELEASE_ARRAY(buffer);

	return ret;
}

/**
*	- LoadNow: Do load.
*/
bool App::LoadNow()
{
	bool ret = true;

	char* buffer = nullptr;
	uint size = fs->Load(currentConfigSaveFileDir.c_str(), &buffer);

	if (size > 0 && buffer)
	{
		JsonFile file(buffer);
		ReadConfig(&file.GetSection("app"));

		for (auto mod : modules)
		{
			ret = mod->Load(&file.AddSection(mod->name.c_str()));
		}
	}

	if (!ret)
	{
		_LOG("Could not load config from: [%s].", currentConfigSaveFileDir.c_str());
	}

	RELEASE_ARRAY(buffer);

	return ret;
}
