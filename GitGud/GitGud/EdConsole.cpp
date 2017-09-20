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
	log.clear();
	colors.clear();
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
			//logs.clear();
			log.clear();
			colors.clear();
		}

		ImGui::BeginChild("", ImVec2(sclx - 20, scly - 75), true);
		{
			//ImGui::TextUnformatted(logs.begin());

			for (uint i = 0; i < colors.size(); ++i)
			{
				ImGui::TextColored(colors[i], log[i].c_str());
			}

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

void EdConsole::PushMessage(const char * str, LOG_TYPE type)
{
	if (str)
	{
		//logs.append(str);
		scrollDown = true;

		switch (type)
		{
		case LOG_INFO:
			lastColor = ImColor(15, 149, 0);
			break;
		case LOG_WARN:
			lastColor = ImColor(255, 255, 0);
			break;
		case LOG_ERROR:
			lastColor = ImColor(255, 0, 0);
			break;
		case LOG_CMD:
			lastColor = ImColor(51, 153, 255);
			break;
		default:
			lastColor = ImColor(255, 255, 255);
			break;
		}

		log.push_back(str);
		colors.push_back(lastColor);
	}
}
