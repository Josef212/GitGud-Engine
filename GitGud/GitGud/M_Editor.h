#ifndef __M_EDITOR_H__
#define __M_EDITOR_H__

#include "Module.h"
#include "JsonFile.h"
#include <list>
#include <string>

#define FILE_MAX_SIZE 250

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
class EdPlayMenu;
class EdTimeDisplay;

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

	void Log(const char* str, LOG_TYPE type);
	void LogFPS(float fps, float ms);

	bool UsingKeyboard()const { return isUsingKeyboard; }
	bool UsingMouse()const { return isUsingMouse; }

	bool FileDialog(const char* extension = nullptr, const char* fromFolder = nullptr);
	const char* CloseFileDialog();


private:
	void SetStyle(const char* filename);
	void SaveStyle(ImGuiStyle* style);
	void SetStyleEditorWin();

	void LoadFile(const char* filterExt = nullptr, const char* fromDir = nullptr);
	void DrawDirRec(const char* directory, const char* filterExt);

public:
	EdConfig* config = nullptr;
	EdConsole* console = nullptr;
	EdHierarchy* hierarchy = nullptr;
	EdInspector* inspector = nullptr;
	EdResources* resources = nullptr;
	EdMaterialCreator* materialCreator = nullptr;
	EdShaderEditor* shaderEditor = nullptr;
	EdPlayMenu* playMenu = nullptr;
	EdTimeDisplay* timeDisplay = nullptr;

private:
	std::list<EdWin*> editorWins;

	enum
	{
		CLOSED,
		OPENED,
		READY_TO_CLOSE
	} fileDialog = CLOSED;

	std::string fileDialogFilter;
	std::string fileDialogOrigin;

	bool inModal = false;
	char selectedFile[FILE_MAX_SIZE];

	bool isUsingMouse = false;
	bool isUsingKeyboard = false;

	bool showImGuiDemo = false;
	bool styleEditor = false;
	
};

#endif //!__M_EDITOR_H__