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
	//vertexFile = new char[256];
	//fragmentFile = new char[256];
	//currentShader = new ResourceShader(11);
}


EdShaderEditor::~EdShaderEditor()
{
	//RELEASE_ARRAY(vertexFile);
	//RELEASE_ARRAY(fragmentFile);
}

void EdShaderEditor::Draw()
{
	int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y;

	ImVec2 pos(0.2 * w, 20);
	ImVec2 size(0.6 * w, 0.77 * h);

	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);

	if (ImGui::Begin("ShaderEditor", &active, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//if (ImGui::MenuItem("Create new shader")) 
				//{
				//	if (SaveCurrentShader())
				//		createMenu = true;
				//}
				//ImGui::MenuItem("Load shader", nullptr, &loadMenu);
				//if (ImGui::MenuItem("Save shader")) SaveCurrentShader();
				//if (ImGui::MenuItem("Force Compile"))
				//{
				//	if (SaveCurrentShader()) currentShader->CompileShader();
				//}

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

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "This feature is still in revision so is normal if its bugged, for now is in read only so edit shaders in a text extern editor.");

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
				if (editingVertex)
				{
					//ImGui::SameLine();
					//ImGui::Text("\tVertex file length: %d, vertex buffer size: %d.", strlen(vertexFile.c_str()), vertexBufferSize);

					ImGui::InputTextMultiline("###vertex_code", (char*)vertexFile.c_str(), vertexBufferSize, ImVec2(-1.f, -1.f), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly | 0);

					//if (strlen(vertexFile.c_str()) >= vertexBufferSize - 16)
					//{
					//	//Resize vertex file
					//	vertexBufferSize = strlen(vertexFile.c_str()) + BUFFER_APPEND_SIZE;
					//	_LOG("Resizing vertex buffer from %d to %d.", strlen(vertexFile.c_str()), vertexBufferSize);
					//	vertexFile.resize(vertexBufferSize);
					//}
				}
				else
				{
					//ImGui::SameLine();
					//ImGui::Text("\tFragment file length: %d, fragment buffer size: %d.", fragmentFile.size(), fragBufferSize);

					ImGui::InputTextMultiline("###fragment_code", (char*)fragmentFile.c_str(), fragBufferSize, ImVec2(-1.f, -1.f), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly | 0);

					//if (strlen(fragmentFile.c_str()) >= fragBufferSize - 16)
					//{
					//	//Resize fragment file
					//	fragBufferSize = strlen(fragmentFile.c_str()) + BUFFER_APPEND_SIZE;
					//	_LOG("Resizing fragment buffer from %d to %d.", strlen(fragmentFile.c_str()), fragBufferSize);
					//	fragmentFile.resize(fragBufferSize);
					//}
				}
			}



			ImGui::End();
		}

		if (loadMenu) LoadShaderMenu();
		if (createMenu) CreateShaderMenu();
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
		
		return true;
	}

	return true;
}

bool EdShaderEditor::LoadCurrentShader()
{
	//if (!currentShader) return false;
	//
	//if(currentShader->vertexFile.Empty() || currentShader->fragmentFile.Empty()) return false;
	//
	//vertexFile.clear();
	//fragmentFile.clear();
	//
	////RELEASE_ARRAY(vertexFile);
	////RELEASE_ARRAY(fragmentFile);
	//
	//char* v = nullptr;
	//char* f = nullptr;
	//
	//uint vSize = app->fs->Load(currentShader->vertexFile.GetFullPath(), &v);
	//uint fSize = app->fs->Load(currentShader->fragmentFile.GetFullPath(), &f);
	//
	//if (vSize > 0 && fSize > 0)
	//{
	//	//vertexFile[vSize] = '\0';
	//	vertexFile = v;
	//	vertexBufferSize = vSize;
	//	//fragmentFile[fSize] = '\0';
	//	fragmentFile = f;
	//	fragBufferSize = fSize;
	//	shaderLoaded = true;
	//}
	//else
	//{
	//	shaderLoaded = false;
	//}
	//
	//RELEASE_ARRAY(v);
	//RELEASE_ARRAY(f);

	return shaderLoaded;
}

void EdShaderEditor::LoadNewShaderFile(ResourceShader* sh)
{
	//RELEASE_ARRAY(vertexFile);
	//RELEASE_ARRAY(fragmentFile);

	vertexFile.clear();
	fragmentFile.clear();

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

	vertexFile = v;
	fragmentFile = f;

	vertexBufferSize = strlen(v);
	fragBufferSize = strlen(f);

	//vertexBufferSize = strlen(v) + 1;
	//fragBufferSize = strlen(f) + 1;

	//vertexFile = new char[vertexBufferSize];
	//fragmentFile = new char[fragBufferSize];
	
	//sprintf_s(vertexFile, vertexBufferSize, v);
	//sprintf_s(fragmentFile, fragBufferSize, f);

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
			//LoadNewShaderFile(); //TODO: Do this
			SaveCurrentShader();

			createMenu = false;
		}

		ImGui::End();
	}
}

void EdShaderEditor::ResizeBuffer(char** buffer, int newSize)
{
	char* tmp = new char[newSize];
	sprintf_s(tmp, newSize, *buffer);
	RELEASE_ARRAY(*buffer);
	*buffer = tmp;
}
