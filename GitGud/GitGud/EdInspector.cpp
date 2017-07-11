#include "EdInspector.h"

#include "imGui/imgui.h"
#include "App.h"
#include "M_GoManager.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentResource.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"

#include "ResourceMesh.h"

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
	ImGui::SetNextWindowSize(ImVec2(w*0.2, (h -20)*0.8 - 5));

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

					case CMP_MESH:
						DrawMesh(selected, (Mesh*)cmp);
						break;
					case CMP_CAMERA:
						DrawCamera(selected, (Camera*)cmp);
						break;
					case CMP_LIGHT:
						DrawLights(selected, (Light*)cmp);
						break;
					case CMP_MATERIAL:
						DrawMaterial(selected, (Material*)cmp);
						break;
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
		ImGui::Separator();
		ImGui::Text("Global transform");
		ImGui::DragFloat4("", (float*)&trans->GetGlobalTransform()[0]);
		ImGui::DragFloat4("", (float*)&trans->GetGlobalTransform()[1]);
		ImGui::DragFloat4("", (float*)&trans->GetGlobalTransform()[2]);
		ImGui::DragFloat4("", (float*)&trans->GetGlobalTransform()[3]);
		ImGui::Text("Local transform");
		ImGui::DragFloat4("", (float*)&trans->GetLocalTransform()[0]);
		ImGui::DragFloat4("", (float*)&trans->GetLocalTransform()[1]);
		ImGui::DragFloat4("", (float*)&trans->GetLocalTransform()[2]);
		ImGui::DragFloat4("", (float*)&trans->GetLocalTransform()[3]);
		
	}
}

void EdInspector::DrawMesh(GameObject * selected, Mesh * mesh)
{
	if (mesh)
	{
		if (ImGui::CollapsingHeader("Mesh"))
		{
			bool meshActive = mesh->IsActive();
			if (ImGui::Checkbox("Mesh active", &meshActive)) mesh->SetActive(meshActive);

			if (ImGui::Button("Remove mesh"))selected->RemoveComponent(mesh);

			ResourceMesh* r = (ResourceMesh*)mesh->GetResource();

			ImGui::Text("Resource mesh ID: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", mesh->GetResourceUID());

			if (r)
			{
				ImGui::Text("Original file: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), r->GetOriginalFile());

				ImGui::Text("Exported file: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), r->GetExportedFile());

				ImGui::Separator();

				ImGui::Text("Num indices: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", r->numIndices);

				ImGui::Text("Num vertices: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", r->numVertices);
			}

			ImGui::Checkbox("Wirframe", &mesh->renderWirefreame);
			ImGui::SameLine();
			ImGui::Checkbox("Normals", &mesh->renderNormals);
		}
	}
}

void EdInspector::DrawCamera(GameObject * selected, Camera * cam)
{
	if (cam)
	{
		if (ImGui::CollapsingHeader("Frustum camera"))
		{
			bool camActive = cam->IsActive();
			if (ImGui::Checkbox("Camera active", &camActive)) cam->SetActive(camActive);

			if (ImGui::Button("Remove camera"))selected->RemoveComponent(cam);

			ImGui::ColorEdit4("Backgorund", (float*)&cam->backgorund, false);

			float nearP = cam->GetNearPlaneDist();
			float farP = cam->GetFarPlaneDist();

			if (ImGui::DragFloat("Near plane:", &nearP)) cam->SetNearPlaneDist(nearP);
			if (ImGui::DragFloat("Far plane:", &farP)) cam->SetFarPlaneDist(farP);


			CAM_TYPE type = cam->GetType();

			int index = 0;
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

void EdInspector::DrawLights(GameObject * selected, Light * light)
{
	if (light)
	{
		if (ImGui::CollapsingHeader("Light"))
		{
			bool activeL = light->IsActive();
			if (ImGui::Checkbox("Light active", &activeL)) light->SetActive(activeL);
			ImGui::SameLine();
			if (ImGui::Button("Remove light")) selected->RemoveComponent(light);

			int lTypeIndex = (int)light->lightType;
			if (ImGui::Combo("Light type", &lTypeIndex, "Directional\0Point\0Spot"))
			{
				light->SetType((LIGHT_TYPE)lTypeIndex);
			}

			ImGui::ColorEdit4("Light color", (float*)&light->color);
			ImGui::DragFloat("Intensity", &light->intensity, 0.05f, 0.f);

			if (light->lightType == LIGHT_TYPE::L_POINT)
			{
				ImGui::DragFloat("Range", &light->range);
			}

			if (light->lightType == LIGHT_TYPE::L_SPOT)
			{
				ImGui::DragFloat("Range", &light->range);
				ImGui::DragFloat("Spot angle", &light->spotAngle);
			}


		}
	}
}

void EdInspector::DrawMaterial(GameObject * selected, Material * mat)
{
}
