#ifndef __EDCONSOLE_H__
#define __EDCONSOLE_H__

#include "EdWin.h"
#include "imGui\imgui.h"
#include <vector>
#include <string>

class EdConsole : public EdWin
{
public:
	EdConsole(bool startEnabled = false);
	virtual ~EdConsole();

	void Draw()override;

	void PushMessage(const char* str, LOG_TYPE type);

private:
	ImGuiTextBuffer logs;
	bool scrollDown = true;
	ImColor lastColor;
	std::vector<ImColor> colors;
	std::vector<std::string> log;
};

#endif //!__EDCONSOLE_H__