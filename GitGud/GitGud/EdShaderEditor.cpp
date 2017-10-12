#include "EdShaderEditor.h"

#include "imGui/imgui.h"
#include "App.h"
#include "ResourceShader.h"
#include "M_ResourceManager.h"
#include "M_FileSystem.h"

#include <string>
#include <vector>

#define BUFFER_APPEND_SIZE 256 * 2
#define SUCCES_COMPILE ImVec4(0, 1, 0, 1), "COMPILED"
#define FAILED_COMPILE ImVec4(1, 0, 0, 1), "FAILED"

EdShaderEditor::EdShaderEditor(bool startEnabled) : EdWin(startEnabled)
{
}


EdShaderEditor::~EdShaderEditor()
{
}

void EdShaderEditor::Draw()
{
	int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y;

	ImVec2 pos(0.2 * w, 20);
	ImVec2 size(0.6 * w, 0.77 * h);

	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);

	if (ImGui::Begin("ShaderViewer", &active, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Create new shader")) 
				{
					if (SaveCurrentShader())
						createMenu = true;
				}
				ImGui::MenuItem("Load shader", nullptr, &loadMenu);
				if (ImGui::MenuItem("Save shader")) SaveCurrentShader();
				if (ImGui::MenuItem("Force Compile"))
				{
					if (SaveCurrentShader()) currentShader->CompileAndLink();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Edit vertex")) editingShader = SH_VERTEX;
				if (ImGui::MenuItem("Edit fragment")) editingShader = SH_FRAGMENT;
				if (ImGui::MenuItem("Edit geometry")) editingShader = SH_GEOMETRY;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "This is only a read tool so edit shaders in a text extern editor. Save option will do nothing.");

		if (currentShader)
		{
			ImGui::Text("Shader name: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), currentShader->name.c_str());

			ImGui::SameLine(0.75 * size.x);
			ImGui::Text("UID: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", currentShader->GetUID());
			ImGui::SameLine();
			ImGui::Text("GL ID: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", currentShader->GetShaderID());

			ImGui::Text("Shader status: ");
			ImGui::SameLine();
			if (currentShader->IsUsable()) ImGui::TextColored(SUCCES_COMPILE); else ImGui::TextColored(FAILED_COMPILE);

			if (!shaderLoaded) shaderLoaded = LoadCurrentShader();

			if (shaderLoaded)
			{
				if (editingShader == SH_VERTEX)
				{
					ImGui::InputTextMultiline("###vertex_code", (char*)currentShader->GetVertexCodeStr(), currentShader->GetVertexCodeSize(), ImVec2(-1.f, -1.f), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly | 0);
				}
				else if(editingShader == SH_FRAGMENT)
				{
					ImGui::InputTextMultiline("###fragment_code", (char*)currentShader->GetFragmentCodeStr(), currentShader->GetFragmentCodeSize(), ImVec2(-1.f, -1.f), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly | 0);
				}
				else if (editingShader == SH_GEOMETRY)
				{
					ImGui::InputTextMultiline("###geometry_code", (char*)currentShader->GetGeometryCodeStr(), currentShader->GetGeometryCodeSize(), ImVec2(-1.f, -1.f), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly | 0);
				}
			}

		}

		if (loadMenu) LoadShaderMenu();
		if (createMenu) CreateShaderMenu();

		ImGui::End();
	}
}

void EdShaderEditor::SetShader(ResourceShader * shader)
{
	currentShader = shader;
	Enable();
}

ResourceShader * EdShaderEditor::GetCurrentShaderEdditing() const
{
	return currentShader;
}

bool EdShaderEditor::SaveCurrentShader()
{
	if (currentShader)
	{
		//if (currentShader->vertexFile.Empty())
		//{
		//	currentShader->vertexFile.SetFolders(SHADER_SAVE_PATH);
		//	currentShader->vertexFile.SetFileName(currentShader->name.c_str());
		//	currentShader->vertexFile.SetExtension("vertex");
		//}
		//
		//if (currentShader->fragmentFile.Empty())
		//{
		//	currentShader->fragmentFile.SetFolders(SHADER_SAVE_PATH);
		//	currentShader->fragmentFile.SetFileName(currentShader->name.c_str());
		//	currentShader->fragmentFile.SetExtension("fragment");
		//}
		//
		//if (app->fs->Save(currentShader->vertexFile.GetFullPath(), vertexFile.c_str(), vertexBufferSize) != vertexBufferSize)
		//	_LOG(LOG_ERROR, "Could not save vertex shader into [%s].", currentShader->vertexFile.GetFullPath());
		//
		//if (app->fs->Save(currentShader->fragmentFile.GetFullPath(), fragmentFile.c_str(), fragBufferSize) != fragBufferSize)
		//	_LOG(LOG_ERROR, "Could not save fragment shader into [%s].", currentShader->fragmentFile.GetFullPath());
		
		/// Not functionality since you cant edit shaders.

		return true;
	}

	return true;
}

bool EdShaderEditor::LoadCurrentShader()
{
	if (!currentShader) return false;
	
	if(currentShader->shaderFile.Empty()) return false;
	
	if (!currentShader->IsCodeLoaded()) currentShader->LoadCode();
	
	shaderLoaded = currentShader->IsCodeLoaded();

	return shaderLoaded;
}

void EdShaderEditor::LoadNewShaderFile(ResourceShader* sh)
{

	static const char* v =
		"#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec2 uv;\n"
		"layout(location = 3) in vec3 color;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"out vec3 outNormal;\n"
		"out vec2 outUv; \n"
		"out vec3 outColor;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(position, 1.0);\n"
		"	outNormal = normal;\n"
		"	outUv = uv;\n"
		"	outColor = color;\n"
		"}\n"
	;
	
	static const char* f= 
		"#version 330 core\n"
		"in vec3 outNormal;\n"
		"in vec2 outUv; \n"
		"in vec3 outColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	//FragColor = vec4(outColor, 1.0);\n"
		"	//FragColor = vec4(0.7, 0.7, 0.7, 1.0); \n"
		"	FragColor = vec4(outNormal, 1.0); \n"
		"}\n"
		;

	shaderLoaded = true;
}

void EdShaderEditor::LoadShaderMenu()
{
	ImGui::Begin("Load shader", &loadMenu);
	{
		std::vector<Resource*> sh;
		app->resources->GetResourcesOfType(sh, RES_SHADER);

		static int sel = -1;

		for (int i = 0; i < sh.size(); ++i)
		{
			ResourceShader* s = (ResourceShader*)sh[i];
			if (s)
			{
				if (ImGui::TreeNodeEx(s->name.c_str(), ((sel == i) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf))
					ImGui::TreePop();

				if (ImGui::IsItemClicked())
					sel = i;
			}
		}

		if (ImGui::Button("Load"))
		{
			if (sel >= 0 && sel < sh.size())
			{
				if (SaveCurrentShader())
				{
					currentShader = (ResourceShader*)sh[sel];
					LoadCurrentShader();
					loadMenu = false;
				}
				else
				{
					_LOG(LOG_WARN, "Could not save current shader before loading new one.");
				}
			}
		}

		ImGui::End();
	}
}

void EdShaderEditor::CreateShaderMenu()
{
	ImGui::Begin("Create shader", &createMenu);
	{
		static char newName[64];
		ImGui::InputText("New shader name", newName, 64);

		if (ImGui::Button("Create"))
		{
			currentShader = (ResourceShader*)app->resources->CreateResource(RES_SHADER);
			currentShader->name = newName;
			currentShader->OnCreation();
			LoadCurrentShader();

			createMenu = false;
		}

		ImGui::End();
	}
}
