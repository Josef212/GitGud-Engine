#include "EdTimeDisplay.h"
#include "App.h"
#include "GG_Clock.h"

#include "imGUI\imgui.h"

EdTimeDisplay::EdTimeDisplay(bool startEnabled) : EdWin(startEnabled)
{
}


EdTimeDisplay::~EdTimeDisplay()
{
}

void EdTimeDisplay::Draw()
{
	ImGui::Begin("Time", &active);
	{
		AppState state = app->GetState();

		ImGui::Text("Game state: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), GetAppStateStr(state));

		ImGui::Text("Real time clock ----------");
		ImGui::Separator();

		ImGui::Text("Time since app start: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.3f", app->clock->TimeSinceStart());

		ImGui::Text("Time since scene loaded: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.3f", app->clock->TimeSinceSceneLoaded());

		ImGui::Text("Dt: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.4f", app->clock->DT());

		ImGui::SameLine();

		ImGui::Text("Frame count: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", app->clock->RealFrameCount());

		ImGui::Text("FPS: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", app->clock->LastFPS());

		ImGui::SameLine();

		ImGui::Text("FPS counter: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", app->clock->FPS());

		ImGui::Text("Game time clock ----------");
		ImGui::Separator();

		ImGui::Text("Game time since scene loaded: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.3f", app->clock->GameTimeSinceLevelLoaded());

		ImGui::Text("Game dt: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.4f", app->clock->GameDT());

		ImGui::SameLine();

		ImGui::Text("Game frame count: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", app->clock->GameFrameCounter());

		float scl = app->clock->GetScale();
		if (ImGui::DragFloat("Time scale", &scl, 0.05f, 0.0f, 20.0f, "%.2f"))
			app->clock->SetScale(scl);

		ImGui::End();
	}
}
