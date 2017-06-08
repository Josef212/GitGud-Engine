#include "EdMaterialCreator.h"

#include "App.h"
#include "M_ResourceManager.h"
#include "M_GoManager.h"
#include "GameObject.h"
#include "Material.h"
#include "ResourceMaterial.h"

#include "imGUI\imgui.h"


EdMaterialCreator::EdMaterialCreator(bool startEnabled) : EdWin(startEnabled)
{
}


EdMaterialCreator::~EdMaterialCreator()
{
}

void EdMaterialCreator::OnEnable()
{
	currentMaterial = (ResourceMaterial*)app->resources->CreateResource(RES_MATERIAL);
}

void EdMaterialCreator::OnDisable()
{
	if (currentMaterial) currentMaterial = nullptr;
}

void EdMaterialCreator::Draw()
{
	ImGui::Begin("Material creator", &active);
	{
		if (currentMaterial)
		{
			static char resName[128];
			strcpy_s(resName, 128, currentMaterial->name.c_str());
			if (ImGui::InputText("Name", resName, 128)) currentMaterial->name.assign(resName);

			ImGui::SameLine();

			ImGui::Text("UUID: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", currentMaterial->GetUID());

		}

		ImGui::End();
	}
}
