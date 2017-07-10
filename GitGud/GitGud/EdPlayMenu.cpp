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

	APP_STATE state = app->GetState();

	if (state != APP_STATE::PLAY && state != APP_STATE::PAUSE)
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

	if (state == APP_STATE::PLAY)
	{
		if (ImGui::Button("PAUSE"))
			app->Pause();
	}
	else if (state == APP_STATE::PAUSE)
	{
		if (ImGui::Button("CONTINUE"))
			app->UnPause();
	}

	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &app->renderer->showGrid);
	ImGui::SameLine();
	ImGui::Checkbox("Draw debug", &app->debugMode);

	//ImGui::SameLine();
	float scl = app->clock->GetScale();
	if (ImGui::DragFloat("Time scale", &scl, 0.05f, 0.0f, 20.0f, "%.2f"))
		app->clock->SetScale(scl);

	ImGui::End();
}
