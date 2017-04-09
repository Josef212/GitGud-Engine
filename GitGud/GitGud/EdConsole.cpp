#include "EdConsole.h"

#include "Globals.h"
#include "App.h"
#include "Console.h"



EdConsole::EdConsole(bool startEnabled) : EdWin(startEnabled)
{
}


EdConsole::~EdConsole()
{
	logs.clear();
}

void EdConsole::Draw()
{
	int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y;

	int posx = w * 0.2f;
	int posy = h * 0.8f;
	int sclx = w * 0.8f;
	int scly = h * 0.2f;

	ImGui::SetNextWindowPos(ImVec2(posx, posy));
	ImGui::SetNextWindowSize(ImVec2(sclx, scly));

	ImGui::Begin("Console", &active);
	{
		if (ImGui::Button("Clear"))
		{
			logs.clear();
		}

		ImGui::BeginChild("", ImVec2(sclx - 20, scly - 75), true);
		{
			ImGui::TextUnformatted(logs.begin());
			if (scrollDown)
			{
				ImGui::SetScrollHere(1.f);
				scrollDown = false;
			}

			ImGui::EndChild();
		}


		static char input[256];
		static bool focus = true;
		if (ImGui::InputText("", input, 100, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (focus)
			{
				ImGui::SetKeyboardFocusHere();
				app->console->OnCmdSubmision(input);
				*input = '\0';
			}
		}
		
		ImGui::End();
	}
}

void EdConsole::PushMessage(const char * str)
{
	if (str)
	{
		logs.append(str);
		scrollDown = true;
	}
}
