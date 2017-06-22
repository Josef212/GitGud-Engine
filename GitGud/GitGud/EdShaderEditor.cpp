#include "EdShaderEditor.h"

#include "imGui/imgui.h"
#include "App.h"
#include "ResourceShader.h"

EdShaderEditor::EdShaderEditor(bool startEnabled) : EdWin(startEnabled)
{
}


EdShaderEditor::~EdShaderEditor()
{
}

void EdShaderEditor::Draw()
{
	if (ImGui::Begin("ShaderEditor", &active))
	{
		ImGui::Text("Shader editor");

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
