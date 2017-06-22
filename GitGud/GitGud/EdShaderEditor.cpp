#include "EdShaderEditor.h"

#include "imGui/imgui.h"
#include "App.h"
#include "ResourceShader.h"
#include "M_ResourceManager.h"
#include "M_FileSystem.h"

#include <vector>

EdShaderEditor::EdShaderEditor(bool startEnabled) : EdWin(startEnabled)
{
	//vertexFile = new char[256];
	//fragmentFile = new char[256];
	//currentShader = new ResourceShader(11);
}


EdShaderEditor::~EdShaderEditor()
{
	RELEASE_ARRAY(vertexFile);
	RELEASE_ARRAY(fragmentFile);
}

void EdShaderEditor::Draw()
{
	int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y;
	
	ImVec2 pos(0.2 * w, 20);
	ImVec2 size(0.6 * w, 0.77 * h);

	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);

	if(ImGui::Begin("ShaderEditor", &active, ImGuiWindowFlags_MenuBar))
	{
		if(ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Create new shader")) 
				{
					if (SaveCurrentShader())
					{
						currentShader = (ResourceShader*)app->resources->CreateResource(RES_SHADER);
						LoadNewShaderFile();
					}
				}
				ImGui::MenuItem("Load shader", nullptr, &loadMenu);
				if (ImGui::MenuItem("Force Compile"))
				{
					SaveCurrentShader();
					if (currentShader) currentShader->CompileShader(); 
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Edit vertex")) editingVertex = true;
				if (ImGui::MenuItem("Edit fragment")) editingVertex = false;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::TextColored(ImVec4(1, 0, 0, 1), "ONLY READ USE!!!");
		
		if (currentShader)
		{
			static char _name[64];
			sprintf_s(_name, 64, currentShader->name.c_str());
			if (ImGui::InputText("Name", _name, 64)) currentShader->name = _name;
			
			ImGui::SameLine(0.70 * size.x);
			ImGui::Text("UID: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", currentShader->GetUID());
			ImGui::SameLine();
			ImGui::Text("GL ID: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", currentShader->GetShaderID());

			if (!shaderLoaded) shaderLoaded = LoadCurrentShader();

			if (shaderLoaded)
			{
				if (editingVertex)
				{
					ImGui::InputTextMultiline("###vertex_code", vertexFile, strlen(vertexFile), ImVec2(-1.f, -1.f), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly | 0);
				}//(int)(sizeof(vertexFile)/sizeof(*vertexFile))
				else
				{
					ImGui::InputTextMultiline("###fragment_code", fragmentFile, strlen(fragmentFile), ImVec2(-1.f, -1.f), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly | 0);
				}
			}
		}
		
		ImGui::End();
	}

	if (loadMenu) LoadShaderMenu();
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
		if (currentShader->vertexFile.Empty())
		{
			currentShader->vertexFile.SetFolders(SHADER_SAVE_PATH);
			currentShader->vertexFile.SetFileName(currentShader->name.c_str());
			currentShader->vertexFile.SetExtension("vertex");
		}

		if (currentShader->fragmentFile.Empty())
		{
			currentShader->fragmentFile.SetFolders(SHADER_SAVE_PATH);
			currentShader->fragmentFile.SetFileName(currentShader->name.c_str());
			currentShader->fragmentFile.SetExtension("fragment");
		}

		uint vSize = strlen(vertexFile);
		uint fSize = strlen(fragmentFile);

		if (app->fs->Save(currentShader->vertexFile.GetFullPath(), vertexFile, vSize) != vSize)
			_LOG("ERROR: Could not save vertex shader into [%s].", currentShader->vertexFile.GetFullPath());

		if (app->fs->Save(currentShader->fragmentFile.GetFullPath(), fragmentFile, fSize) != fSize)
			_LOG("ERROR: Could not save fragment shader into [%s].", currentShader->fragmentFile.GetFullPath());
		
		return true;
	}

	return true;
}

bool EdShaderEditor::LoadCurrentShader()
{
	if (!currentShader) return false;

	if(currentShader->vertexFile.Empty() || currentShader->fragmentFile.Empty()) return false;

	RELEASE_ARRAY(vertexFile);
	RELEASE_ARRAY(fragmentFile);

	uint vSize = app->fs->Load(currentShader->vertexFile.GetFullPath(), &vertexFile);
	uint fSize = app->fs->Load(currentShader->fragmentFile.GetFullPath(), &fragmentFile);

	if (vSize > 0 && fSize > 0)
	{
		vertexFile[vSize] = '\0';
		fragmentFile[fSize] = '\0';
		shaderLoaded = true;
	}
	else
	{
		shaderLoaded = false;
	}

	return shaderLoaded;
}

void EdShaderEditor::LoadNewShaderFile()
{
	RELEASE_ARRAY(vertexFile);
	RELEASE_ARRAY(fragmentFile);

	vertexFile = new char[512];
	fragmentFile = new char[256];

	static const char v[512] = 
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
	
	static const char f[256] =
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

	sprintf_s(vertexFile, 512, v);
	sprintf_s(fragmentFile, 256, f);

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
				if (ImGui::TreeNodeEx(s->name.c_str(), ((sel == i) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_Leaf)))
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
					_LOG("Could not save current shader before loading new one.");
				}
			}
		}

		ImGui::End();
	}
}
