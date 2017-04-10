#include "EdInspector.h"

#include "imGui/imgui.h"
#include "App.h"
#include "M_GoManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

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


			for (auto cmp : selected->components)
			{
				if (cmp)
				{
					switch (cmp->GetType())
					{
					case CMP_TRANSFORM:
						DrawTrans(selected);
						break;
					case CMP_CAMERA:
						DrawCamera(selected, (Camera*)cmp);
					}
				}
			}


		}


		ImGui::End();
	}
}

void EdInspector::DrawTrans(GameObject * selected)
{
	Transform* trans = selected->transform;
	if (!trans)
		return;

	if (ImGui::CollapsingHeader("Transform"))
	{
		bool transActive = trans->IsActive();
		if (ImGui::Checkbox("Transform active", &transActive)) trans->SetActive(transActive);

		if (ImGui::Button("Remove transform"))selected->RemoveComponent(trans);

		ImGui::SameLine();

		if (ImGui::Button("Reset transform"))
		{
			trans->SetLocalPosition(float3::zero);
			trans->SetLocalScale(float3::one);
			trans->SetLocalRotation(Quat::identity);
		}
		
		float3 pos = trans->GetLocalPosition();
		float3 scl = trans->GetLocalScale();
		float3 rot = trans->GetLocalRotation();

		if (ImGui::DragFloat3("Position", (float*)&pos, 0.25f))trans->SetLocalPosition(pos);
		if (ImGui::DragFloat3("Scale", (float*)&scl))trans->SetLocalScale(scl);
		if (ImGui::SliderAngle3("Rotation", (float*)&rot, 0.5f)) trans->SetLocalRotation(rot);

		ImGui::Separator();

		ImGui::DragFloat3("Global pos", (float*)&trans->GetGlobalPosition());
		
	}
}

void EdInspector::DrawCamera(GameObject * selected, Camera * cam)
{
	if (cam)
	{
		if (ImGui::CollapsingHeader("Frustum camera"))
		{
			ImGui::ColorEdit4("Backgorund", (float*)&cam->backgorund, false);

			float nearP = cam->GetNearPlaneDist();
			float farP = cam->GetFarPlaneDist();

			if (ImGui::DragFloat("Near plane:", &nearP)) cam->SetNearPlaneDist(nearP);
			if (ImGui::DragFloat("Far plane:", &farP)) cam->SetFarPlaneDist(farP);


			CAM_TYPE type = cam->GetType();

			static int index = 0;
			if (ImGui::Combo("Type", &index, "Perspective\0Orthographic"))
			{
				if (index == 0)
				{
					cam->SetType(CAM_PERSPECTIVE);
				}
				else if (index == 1)
				{
					cam->SetType(CAM_ORTHOGRAPHIC);
				}
			}

			if (type == CAM_PERSPECTIVE)
			{
				float fov = cam->GetFOV();
				if (ImGui::DragFloat("FOV", &fov)) cam->SetFOV(fov);
			}
			else
			{
				float size = cam->GetOthogonalSize();
				if (ImGui::DragFloat("Size", &size)) cam->SetOrthoSize(size);
			}

			bool culling = cam->IsCulling();
			if (ImGui::Checkbox("Culling", &culling)) cam->SetCulling(culling);

			//TODO: Make active
		}
	}
}
