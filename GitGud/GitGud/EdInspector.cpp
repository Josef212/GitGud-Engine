#include "EdInspector.h"

#include "imgui-1.49\imgui.h"
#include "App.h"
#include "M_GoManager.h"
#include "GameObject.h"

EdInspector::EdInspector(bool startEnabled) : EdWin(startEnabled)
{
}


EdInspector::~EdInspector()
{
}

void EdInspector::Draw()
{
	int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y;

	ImGui::SetNextWindowPos(ImVec2(w * 0.8, 20));
	ImGui::SetNextWindowSize(ImVec2(w*0.2, (h -20)*0.8 - 55));

	GameObject* selected = app->goManager->GetSelected();

	ImGui::Begin("Inspector", &active);
	{
		if (selected)
		{
			bool ac = selected->IsActive();
			if (ImGui::Checkbox("Active", &ac))selected->SetActive(ac);
			char name[128];
			sprintf_s(name, 128, selected->GetName());

			ImGui::SameLine();

			if (ImGui::InputText("Name", name, 128)) selected->SetName(name);

			ImGui::Separator();

			if (ImGui::Button("Delete"))selected->Destroy();

			ImGui::SameLine();

			bool stat = selected->IsStatic();
			if (ImGui::Checkbox("Static", &stat)) selected->SetStatic(stat);

			ImGui::Separator();


			//TODO


		}


		ImGui::End();
	}
}

void EdInspector::DrawTrans(GameObject * selected)
{
}
