#ifndef __EDCONSOLE_H__
#define __EDCONSOLE_H__

#include "EdWin.h"
#include "imGui\imgui.h"

class EdConsole : public EdWin
{
public:
	EdConsole(bool startEnabled = false);
	virtual ~EdConsole();

	void Draw()override;

	void PushMessage(const char* str);

private:
	ImGuiTextBuffer logs;
	bool scrollDown = true;
};

#endif //!__EDCONSOLE_H__