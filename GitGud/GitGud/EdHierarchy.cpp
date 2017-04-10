#include "EdHierarchy.h"

#include "App.h"
#include "GameObject.h"
#include "M_GoManager.h"

#include "imGui/imgui.h"


EdHierarchy::EdHierarchy(bool startEnabled) : EdWin(startEnabled)
{
}


EdHierarchy::~EdHierarchy()
{
}

void EdHierarchy::Draw()
{
	int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y;

	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(w*0.2, (h - 20)*0.5));

	ImGui::Begin("Hierarchy", &active);
	{
		GameObject* root = app->goManager->GetRoot();
		GameObject* selec = app->goManager->GetSelected();

		ImGuiTreeNodeFlags nodeFlgs = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (!selec)
			nodeFlgs |= ImGuiTreeNodeFlags_Selected;

		if (ImGui::TreeNodeEx("Scene", nodeFlgs))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);

			if (ImGui::IsItemClicked())
				app->goManager->SelectGo(nullptr);

			if(root)
			{
				for (auto go : root->childs)
				{
					RecDraw(go, selec);
				}
			}

			ImGui::PopStyleVar();
			ImGui::TreePop();
		}


		ImGui::End();
	}

}

void EdHierarchy::RecDraw(GameObject * node, GameObject * selected)
{
	ImGuiTreeNodeFlags nodeFlags = 0;
	if (node == selected)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	if (node->childs.size() > 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
	}
	else
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(node->GetName(), nodeFlags))
	{
		if (ImGui::IsItemClicked())
			app->goManager->SelectGo(node);

		for (auto go : node->childs)
		{
			RecDraw(go, selected);
		}
		ImGui::TreePop();
	}
}
