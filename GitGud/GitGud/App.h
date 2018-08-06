#ifndef __APP_H__
#define __APP_H__

#include "Globals.h"

#include "GG_Clock.h"
#include "Timer.h"
#include "PerfTimer.h"

#include <string>
#include <vector>

struct Command;

class HrdInfo;
class Console;
class RandGen;
class JsonFile;

class Module;
class M_Window;
class M_Input;
class M_FileSystem;
class M_Editor;
class M_GoManager;
class M_Camera3D;
class M_ResourceManager;
class M_Renderer;


class App
{
public:
	App(int argv, char** argc);
	virtual ~App();

	bool Init();
	UpdateReturn Update();
	bool CleanUp();

	void DrawDebug();

	const char* GetTitle()const;
	const char* GetOrganitzation()const;
	void SetTitle(const char* str);
	void SetOrganitzation(const char* str);

	uint GetMaxFPS()const;
	void SetMaxFPS(uint _fps);

	void Log(const char* str, LogType type);
	void Browse(const char* url);

	void Save(std::string saveFile);
	void Load(std::string saveFile);

	void AddCommand(Command* cmd);

	const char* GetConfigSavePath()const;
	void SetConfigSavePath(const char* path);
	void ResetConfig();

	//----------------

	AppState GetState()const;
	bool IsPlay()const;
	bool IsPause()const;
	bool IsStop()const;

	void Play();
	void Pause();
	void Stop();
	void UnPause();

	//----------------


private:
	void PrepareUpdate();
	void FinishUpdate();

	void ReadConfig(JsonFile* config);

	void ReadArgs();

	bool SaveNow();
	bool LoadNow();

public:
	HrdInfo* info = nullptr;
	Console* console = nullptr;
	RandGen* random = nullptr;

	M_FileSystem* fs = nullptr;
	M_Window* win = nullptr;
	M_Input* input = nullptr;
	M_Editor* editor = nullptr;
	M_ResourceManager* resources = nullptr;
	M_GoManager* goManager = nullptr;
	M_Camera3D* camera = nullptr;

	M_Renderer* renderer = nullptr;

	GG_Clock* clock = nullptr;

	bool debugMode = false;
	bool quit = false;
	bool forceEditor = false;

private:
	std::vector<Module*>	modules;
	std::string				title = APP_TITLE;
	std::string				organitzation = APP_ORGANITZATION;

	std::string				currentConfigSaveFileDir;

	int argv = 0;
	std::vector<std::string> argc;

	bool saveNextFrame = false;
	bool loadNextFrame = false;

	AppState state = AppState::STOP; //TODO: Args??
	uint32	cappedMs = 0;


};
extern App* app;


#endif // !__APP_H__