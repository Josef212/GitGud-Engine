#ifndef __APP_H__
#define __APP_H__

#include "Globals.h"

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
	UPDATE_RETURN Update();
	bool CleanUp();

	void DrawDebug();

	const char* GetTitle()const;
	const char* GetOrganitzation()const;
	void SetTitle(const char* str);
	void SetOrganitzation(const char* str);

	uint GetMaxFPS()const;
	void SetMaxFPS(uint _fps);

	void Log(const char* str);
	void Browse(const char* url);

	void Save(std::string saveFile);
	void Load(std::string saveFile);

	void AddCommand(Command* cmd);

	const char* GetConfigSavePath()const;
	void SetConfigSavePath(const char* path);
	void ResetConfig();


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

	Timer	msTimer;
	Timer	fpsTimer;
	uint64		frames = 0;
	float		dt = 0.f;
	int			fpsCounter = 0;
	uint32		lastFrameMs = -1;
	int			lastFps = -1;
	uint32	cappedMs = 0;


};
extern App* app;


#endif // !__APP_H__