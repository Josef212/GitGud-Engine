#include "EdPlayMenu.h"
#include "App.h"
#include "M_Renderer.h"

#include "imGUI\imgui.h"

EdPlayMenu::EdPlayMenu(bool startEnabled) : EdWin(startEnabled)
{
}


EdPlayMenu::~EdPlayMenu()
{
}

void EdPlayMenu::Draw()
{
	int w = ImGui::GetIO().DisplaySize.x;
	int winSize = 300;
	ImGui::SetNextWindowPos(ImVec2((w / 2) - (winSize / 2), 20));
	ImGui::SetNextWindowSize(ImVec2(winSize, 60));

	ImGui::Begin("Play bar", &active, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	AppState state = app->GetState();

	if (state != AppState::PLAY && state != AppState::PAUSE)
	{
		if (ImGui::Button("PLAY"))
			app->Play();
	}
	else
	{
		if (ImGui::Button("STOP"))
			app->Stop();
	}

	ImGui::SameLine();

	if (state == AppState::PLAY)
	{
		if (ImGui::Button("PAUSE"))
			app->Pause();
	}
	else if (state == AppState::PAUSE)
	{
		if (ImGui::Button("CONTINUE"))
			app->UnPause();
	}

	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &app->renderer->showGrid);
	ImGui::SameLine();
	ImGui::Checkbox("Draw debug", &app->debugMode);

	float scl = app->clock->GetScale();
	if (ImGui::DragFloat("Time scale", &scl, 0.05f, 0.0f, 20.0f, "%.2f"))
		app->clock->SetScale(scl);

	ImGui::End();
}
