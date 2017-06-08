#include "EdResources.h"

#include "App.h"
#include "M_ResourceManager.h"
#include "Resource.h"

#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"

#include "imGUI\imgui.h"


EdResources::EdResources(bool startEnabled) : EdWin(startEnabled)
{
}


EdResources::~EdResources()
{
}

void EdResources::Draw()
{
	int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y;

	ImGui::SetNextWindowPos(ImVec2(0, h*0.5 + 10));
	ImGui::SetNextWindowSize(ImVec2(w*0.2, (h - 20)*0.5));

	ImGui::Begin("Resources", &active);
	{
		std::vector<Resource*> resources;

		if (ImGui::CollapsingHeader("Meshes"))
		{
			resources.clear();
			app->resources->GetResourcesOfType(resources, RES_MESH);

			MeshResource(resources);
		}

		if (ImGui::CollapsingHeader("Materials"))
		{
			resources.clear();
			app->resources->GetResourcesOfType(resources, RES_MATERIAL);

			MaterialResource(resources);
		}


		ImGui::End();
	}
}

void EdResources::MeshResource(std::vector<Resource*> meshes)
{
	static int mS = -1;

	for (uint i = 0; i < meshes.size(); ++i)
	{
		ResourceMesh* mesh = (ResourceMesh*)meshes[i];
		if (mesh)
		{
			ImGuiTreeNodeFlags nodeFlags = 0;
			if (mS == i)
			{
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
				nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
				nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
			}
			else
			{
				nodeFlags |= ImGuiTreeNodeFlags_Leaf;
			}

			if (ImGui::TreeNodeEx(mesh->name.c_str(), nodeFlags))
			{
				if (mS == i)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);

					ImGui::BeginChild("meshes", ImVec2(infoW, infoH));
					{
						//---- Should be generic??

						ImGui::Text("Original file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), mesh->originalFile.c_str());

						ImGui::Text("Exported file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), mesh->exportedFile.c_str());

						ImGui::Text("Instances in memory: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mesh->GetUID());

						//----------------------------------

						if (mesh->IsInMemory())
						{

						}

						ImGui::Text("Num vertices:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", mesh->numVertices);

						ImGui::Text("Num indices:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", mesh->numIndices);

						ImGui::Separator();

						//-------------------

						ImGui::Text("Indices GL buffer id: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", mesh->idIndices);

						ImGui::Text("Vertices GL buffer id: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", mesh->idVertices);

						ImGui::Text("Normals GL buffer id: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", mesh->idNormals);

						ImGui::Text("Colors GL buffer id: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", mesh->idColors);

						ImGui::Text("Texture coords GL buffer id: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", mesh->idUvs);

						ImGui::Text("Container GL buffer id: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", mesh->idContainer);

						//-------------------

						//TODO: Attach etc.

					}

					ImGui::EndChild();
					ImGui::PopStyleVar();
				}

				if (ImGui::IsItemClicked())
					mS = i;

				ImGui::TreePop();
			}

		}
	}
}

void EdResources::MaterialResource(std::vector<Resource*> materials)
{
	static int maS = -1;

	for (uint i = 0; i < materials.size(); ++i)
	{
		ResourceMesh* material = (ResourceMesh*)materials[i];
		if (material)
		{
			ImGuiTreeNodeFlags nodeFlags = 0;
			if (maS == i)
			{
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
				nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
				nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
			}
			else
			{
				nodeFlags |= ImGuiTreeNodeFlags_Leaf;
			}

			if (ImGui::TreeNodeEx(material->name.c_str(), nodeFlags))
			{
				if (maS == i)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);

					ImGui::BeginChild("meshes", ImVec2(infoW, infoH));
					{
						//---- Should be generic??

						ImGui::Text("Original file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), material->originalFile.c_str());

						ImGui::Text("Exported file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), material->exportedFile.c_str());

						ImGui::Text("Instances in memory: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", material->GetUID());

						//----------------------------------

						if (material->IsInMemory())
						{

						}


						ImGui::Separator();

						//-------------------


						//-------------------

						//TODO: Attach etc.

					}

					ImGui::EndChild();
					ImGui::PopStyleVar();
				}

				if (ImGui::IsItemClicked())
					maS = i;

				ImGui::TreePop();
			}

		}
	}
}

void EdResources::TextureResource(std::vector<Resource*> textures)
{
}
