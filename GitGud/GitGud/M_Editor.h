#ifndef __M_EDITOR_H__
#define __M_EDITOR_H__

#include "Module.h"
#include "JsonFile.h"
#include <list>
#include <string>

class EdWin;
union SDL_Event;

struct ImGuiStyle;

class EdConfig;
class EdConsole;
class EdHierarchy;
class EdInspector;
class EdMaterialCreator;
class EdResources;
class EdShaderEditor;

class M_Editor : public Module
{
public:
	M_Editor(const char* name, bool startEnabled = true);
	virtual ~M_Editor();

	bool Init(JsonFile* file)override;
	bool Start()override;
	UPDATE_RETURN PreUpdate(float dt)override;
	UPDATE_RETURN Update(float dt)override;
	bool CleanUp()override;

	void DrawEditor();
	void PassInput(SDL_Event* ev);

	void Log(const char* str);
	void LogFPS(float fps, float ms);

	bool UsingKeyboard()const { return isUsingKeyboard; }
	bool UsingMouse()const { return isUsingMouse; }


private:
	void SetStyle(const char* filename);
	void SaveStyle(ImGuiStyle* style);
	void SetStyleEditorWin();

public:
	EdConfig* config = nullptr;
	EdConsole* console = nullptr;
	EdHierarchy* hierarchy = nullptr;
	EdInspector* inspector = nullptr;
	EdResources* resources = nullptr;
	EdMaterialCreator* materialCreator = nullptr;
	EdShaderEditor* shaderEditor = nullptr;

private:
	std::list<EdWin*> editorWins;

	bool isUsingMouse = false;
	bool isUsingKeyboard = false;

	bool showImGuiDemo = false;
	bool styleEditor = false;
	
};

#endif //!__M_EDITOR_H__