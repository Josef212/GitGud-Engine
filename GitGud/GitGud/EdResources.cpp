#include "EdResources.h"

#include "App.h"
#include "M_Editor.h"
#include "EdShaderEditor.h"
#include "M_ResourceManager.h"
#include "Resource.h"

#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"
#include "ResourceShader.h"
#include "ResourceScene.h"

#include "imGUI\imgui.h"

#include "OpenGL.h"


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

		if (ImGui::CollapsingHeader("Textures"))
		{
			resources.clear();
			app->resources->GetResourcesOfType(resources, RES_TEXTURE);

			TextureResource(resources);
		}

		if (ImGui::CollapsingHeader("Scenes"))
		{
			resources.clear();
			app->resources->GetResourcesOfType(resources, RES_SCENE);

			SceneResource(resources);
		}

		if (ImGui::CollapsingHeader("Shader"))
		{
			resources.clear();
			app->resources->GetResourcesOfType(resources, RES_SHADER);

			ShaderResource(resources);
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
						ImGui::TextColored(ImVec4(1, 1, 0, 1), mesh->originalFile.GetFile());

						ImGui::Text("Exported file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), mesh->exportedFile.GetFile());

						ImGui::Text("Instances in memory: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", mesh->Count());

						ImGui::Text("ID: ");
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

					ImGui::BeginChild("materials", ImVec2(infoW, infoH));
					{
						//---- Should be generic??

						ImGui::Text("Original file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), material->originalFile.GetFile());

						ImGui::Text("Exported file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), material->exportedFile.GetFile());

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
	static int tS = -1;

	for (uint i = 0; i < textures.size(); ++i)
	{
		ResourceTexture* text = (ResourceTexture*)textures[i];
		if (text)
		{
			ImGuiTreeNodeFlags nodeFlags = 0;
			if (tS == i)
				nodeFlags |= ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			else
				nodeFlags |= ImGuiTreeNodeFlags_Leaf;

			if (ImGui::TreeNodeEx(text->name.c_str(), nodeFlags))
			{
				if (tS == i)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);

					ImGui::BeginChild("textures", ImVec2(infoW, infoH));
					{
						ImGui::Text("Original file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), text->originalFile.GetFile());

						ImGui::Text("Exported file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), text->exportedFile.GetFile());

						ImGui::Text("Instances in memory: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", text->Count());

						ImGui::Text("ID: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", text->GetUID());

						//------------------------------------------------------------------------

						if (text->IsInMemory())
						{
							ImGui::Separator();

							ImGui::Text("Width:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", text->width);

							ImGui::Text("Height:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", text->height);

							ImGui::Text("Depth:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", text->depth);

							ImGui::Text("Bpp:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", text->bpp);

							ImGui::Text("Mips:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", text->mips);

							ImGui::Text("Bytes:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", text->bytes);

							ImGui::Text("Format:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), text->GetTextureFormatStr());

							ImGui::Text("Type:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), text->GetTextureTypeStr());

							uint texIndex = text->texID;
							glBindTexture(GL_TEXTURE_2D, texIndex);
							ImTextureID texId = (void*)texIndex;
							ImVec2 texSize(64, 64);
							ImGui::Image(texId, texSize, ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255));
							
							if (ImGui::IsItemHovered())
							{
								//TODO: This tool only zoom full size image.

								ImGui::BeginTooltip();

								ImVec2 texScreenPos = ImGui::GetCursorScreenPos();
								float focusSZ = 64.0f;
								float focusX = ImGui::GetMousePos().x - texScreenPos.x - focusSZ * 0.5f;
								if (focusX < 0.0f) focusX = 0.0f;
								else if (focusX > texSize.x - focusSZ) focusX = texSize.x - focusSZ;
								float focusY = ImGui::GetMousePos().y - texScreenPos.y - focusSZ * 0.5f;
								if (focusY < 0.0f) focusY = 0.0f;
								else if (focusY > texSize.y - focusSZ) focusY = texSize.y - focusSZ;

								ImVec2 uv0 = ImVec2((focusX) / texSize.x, (focusY) / texSize.y);
								ImVec2 uv1 = ImVec2((focusX + focusSZ) / texSize.x, (focusY + focusSZ) / texSize.y);

								ImGui::Image(texId, ImVec2(256, 256), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255));

								ImGui::EndTooltip();
							}
							
							glBindTexture(GL_TEXTURE_2D, 0);
						}
					}

					ImGui::EndChild();
					ImGui::PopStyleVar();
				}

				if (ImGui::IsItemClicked()) tS = i;

				ImGui::TreePop();
			}
		}
	}
}

void EdResources::SceneResource(std::vector<Resource*> textures)
{
}

void EdResources::ShaderResource(std::vector<Resource*> shaders)
{
	static int shS = -1;

	for (uint i = 0; i < shaders.size(); ++i)
	{
		ResourceShader* sh = (ResourceShader*)shaders[i];
		if (sh)
		{
			ImGuiTreeNodeFlags nodeFlags = 0;
			if (shS == i)
				nodeFlags |= ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			else
				nodeFlags |= ImGuiTreeNodeFlags_Leaf;

			if (ImGui::TreeNodeEx(sh->name.c_str(), nodeFlags))
			{
				if (shS == i)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);

					ImGui::BeginChild("shaders", ImVec2(infoW, infoH));
					{
						ImGui::Text("Original file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), sh->originalFile.GetFile());

						ImGui::Text("Exported file: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), sh->exportedFile.GetFile());

						ImGui::Text("Instances in memory: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", sh->Count());

						ImGui::Text("ID: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", sh->GetUID());

						//--------------------------------------------------------------------------

						ImGui::Separator();

						if (ImGui::Button("Edit shader")) app->editor->shaderEditor->SetShader(sh);

						//TODO: Shader editor and shader attach
					}

					ImGui::EndChild();
					ImGui::PopStyleVar;
				}

				if(ImGui::IsItemClicked()) shS = i;

				ImGui::TreePop();
			}
		}
	}
}
