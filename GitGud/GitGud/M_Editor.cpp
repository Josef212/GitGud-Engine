#include "M_Editor.h"

#include "App.h"

#include "M_Window.h"
#include "M_FileSystem.h"

#include "EdWin.h"
#include "EdConfig.h"
#include "EdConsole.h"
#include "EdHierarchy.h"
#include "EdInspector.h"
#include "EdResources.h"
#include "EdMaterialCreator.h"
#include "EdShaderEditor.h"

#include "GameObject.h"
#include "Light.h"
#include "M_GoManager.h"

#include <SDL.h>
#include "imGui/imgui.h"
#include "imGui\imgui_impl_sdl_gl3.h"


M_Editor::M_Editor(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG("Editor: Creation.");

	//Creating panels
	//TODO: Start enabled according to engine start state
	config = new EdConfig(false);
	console = new EdConsole(true);
	hierarchy = new EdHierarchy(true);
	inspector = new EdInspector(true);
	resources = new EdResources(true);
	materialCreator = new EdMaterialCreator(false);
	shaderEditor = new EdShaderEditor(false);


	editorWins.push_back(config); 
	editorWins.push_back(console);
	editorWins.push_back(hierarchy);
	editorWins.push_back(inspector);
	editorWins.push_back(resources);
	editorWins.push_back(materialCreator);
	editorWins.push_back(shaderEditor);
}


M_Editor::~M_Editor()
{
	_LOG("Editor: Destroying.");

	std::list<EdWin*>::reverse_iterator it = editorWins.rbegin();
	for (; it != editorWins.rend(); ++it)
		RELEASE(*it);

	ImGui_ImplSdlGL3_Shutdown();
}

bool M_Editor::Init(JsonFile* file)
{
	_LOG("Editor: Init.");

	ImGui_ImplSdlGL3_Init(app->win->GetWindow());

	SetStyle((CONFIG_PATH + std::string("style.json")).c_str());

	return true;
}

bool M_Editor::Start()
{
	_LOG("Editor: Start.");

	return true;
}

UPDATE_RETURN M_Editor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(app->win->GetWindow());

	ImGuiIO& io = ImGui::GetIO();
	isUsingMouse = io.WantCaptureMouse;
	isUsingKeyboard = io.WantCaptureKeyboard;

	return UPDT_CONTINUE;
}

UPDATE_RETURN M_Editor::Update(float dt)
{

	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save scene")) app->goManager->SaveScene(); //TMP
			if (ImGui::MenuItem("Load scene")) app->goManager->LoadScene();
			if (ImGui::MenuItem("Quit")) app->quit = true;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Configuration", nullptr, &config->active);
			ImGui::MenuItem("Console", nullptr, &console->active);
			ImGui::MenuItem("Hierarchy", nullptr, &hierarchy->active);
			ImGui::MenuItem("Inspector", nullptr, &inspector->active);
			ImGui::MenuItem("Resource", nullptr, &resources->active);
			ImGui::MenuItem("SetStyle", nullptr, &styleEditor);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create empty")) 
			{
				GameObject* selected = app->goManager->GetSelected();
				app->goManager->CreateGameObject(selected); 
			}

			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Directional light"))
				{
					GameObject* go = app->goManager->CreateGameObject();
					Light* l = (Light*)go->CreateComponent(CMP_LIGHT);
					l->SetType(LIGHT_TYPE::L_DIRECTIONAL);
				}
				if (ImGui::MenuItem("Point light"))
				{
					GameObject* go = app->goManager->CreateGameObject();
					Light* l = (Light*)go->CreateComponent(CMP_LIGHT);
					l->SetType(LIGHT_TYPE::L_POINT);
				}
				if (ImGui::MenuItem("Spot light"))
				{
					GameObject* go = app->goManager->CreateGameObject();
					Light* l = (Light*)go->CreateComponent(CMP_LIGHT);
					l->SetType(LIGHT_TYPE::L_SPOT);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Components"))
		{
			GameObject* selected = app->goManager->GetSelected();

			if (ImGui::MenuItem("Transform")) if (selected) selected->CreateComponent(CMP_TRANSFORM);
			if (ImGui::MenuItem("Mesh")) if (selected) selected->CreateComponent(CMP_MESH);
			if (ImGui::MenuItem("Camera")) if (selected) selected->CreateComponent(CMP_CAMERA);
			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Directional"))
				{
					if (selected)
					{
						Light* l = (Light*)selected->CreateComponent(CMP_LIGHT);
						l->SetType(L_DIRECTIONAL);
					}
				}

				if (ImGui::MenuItem("Point"))
				{
					if (selected)
					{
						Light* l = (Light*)selected->CreateComponent(CMP_LIGHT);
						l->SetType(L_POINT);
					}
				}

				if (ImGui::MenuItem("Spot"))
				{
					if (selected)
					{
						Light* l = (Light*)selected->CreateComponent(CMP_LIGHT);
						l->SetType(L_SPOT);
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Resources"))
		{
			ImGui::MenuItem("Create material...", nullptr, &materialCreator->active);
			ImGui::MenuItem("Shader editor", nullptr, &shaderEditor->active);
			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("Other"))
		{
			ImGui::MenuItem("ImGui demo", nullptr, &showImGuiDemo);
			if (ImGui::BeginMenu("3rd party"))
			{
				if (ImGui::MenuItem("SDL2")) app->Browse(URL_SDL2);
				if (ImGui::MenuItem("OpenGL")) app->Browse(URL_OPENGL);
				if (ImGui::MenuItem("Assimp")) app->Browse(URL_ASSIMP);
				if (ImGui::MenuItem("Devil")) app->Browse(URL_DEVIL);
				if (ImGui::MenuItem("ImGui")) app->Browse(URL_IMGUI);
				if (ImGui::MenuItem("PhysFs")) app->Browse(URL_PHYSFS);
				if (ImGui::MenuItem("Glew")) app->Browse(URL_GLEW);
				if (ImGui::MenuItem("Parson")) app->Browse(URL_PARSON);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("About me"))
			{
				if (ImGui::MenuItem("Repository")) app->Browse(URL_REPO);
				if (ImGui::MenuItem("Github")) app->Browse(URL_GITHUB);
				if (ImGui::MenuItem("Issues")) app->Browse(URL_ISSUES);
				if (ImGui::MenuItem("Releases")) app->Browse(URL_RELEASES);
				if (ImGui::MenuItem("Web")) ;

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


	for (std::list<EdWin*>::iterator it = editorWins.begin(); it != editorWins.end(); ++it)
	{
		if ((*it)->IsActive())
			(*it)->Draw();
	}

	if (styleEditor) SetStyleEditorWin();

	if (showImGuiDemo)
	{
		ImGui::ShowTestWindow(&showImGuiDemo);
		ImGui::ShowMetricsWindow(&showImGuiDemo);
	}

	return UPDT_CONTINUE;
}

bool M_Editor::CleanUp()
{
	_LOG("Editor: CleanUp.");

	return true;
}

void M_Editor::DrawEditor()
{
	ImGui::Render();
}

void M_Editor::PassInput(SDL_Event* ev)
{
	ImGui_ImplSdlGL3_ProcessEvent(ev);
}

void M_Editor::Log(const char * str)
{
	if (console)
		console->PushMessage(str);
}

void M_Editor::LogFPS(float fps, float ms)
{
	if (config)
		config->PushFps(fps, ms);
}

void M_Editor::SetStyle(const char* filename)
{
	char* buffer = nullptr;
	uint size = app->fs->Load(filename, &buffer);

	if (buffer && size > 0)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		JsonFile file(buffer);

		JsonFile colors = file.GetSection("colors");
		for (uint i = 0; i < ImGuiCol_COUNT; ++i)
		{
			style.Colors[i] = ImVec4(colors.GetFloat(ImGui::GetStyleColName(i), 0.f, 0), 
				colors.GetFloat(ImGui::GetStyleColName(i), 0.f, 1), 
				colors.GetFloat(ImGui::GetStyleColName(i), 0.f, 2),
				colors.GetFloat(ImGui::GetStyleColName(i), 0.f, 3));
		}

		JsonFile rendering = file.GetSection("rendering");
		style.AntiAliasedLines = rendering.GetBool("anti_aliased_lines", true);
		style.AntiAliasedShapes = rendering.GetBool("anti_aliased_shapes", true);
		style.CurveTessellationTol = rendering.GetFloat("curve_tessellation_tolerance", 0.f);
		style.Alpha = rendering.GetFloat("alpha", 1.f);

		JsonFile sizes = file.GetSection("sizes");
		style.WindowPadding = ImVec2(sizes.GetFloat("win_padding", 0.f, 0), sizes.GetFloat("win_padding", 0.f, 1));
		style.WindowRounding = sizes.GetFloat("win_rounding", 0.f);
		style.ChildWindowRounding = sizes.GetFloat("child_win_rounding", 0.f);
		style.FramePadding = ImVec2(sizes.GetFloat("frame_padding", 0.f, 0), sizes.GetFloat("frame_padding", 0.f, 1));
		style.FrameRounding = sizes.GetFloat("frame_rounding", 0.f);
		style.ItemSpacing = ImVec2(sizes.GetFloat("item_spacing", 0.f, 0), sizes.GetFloat("item_spacing", 0.f, 1));
		style.ItemInnerSpacing = ImVec2(sizes.GetFloat("item_inner_spacing", 0.f, 0), sizes.GetFloat("item_inner_spacing", 0.f, 1));
		style.TouchExtraPadding = ImVec2(sizes.GetFloat("touch_extra_padding", 0.f, 0), sizes.GetFloat("touch_extra_padding", 0.f, 1));
		style.IndentSpacing = sizes.GetFloat("indent_spacing", 0.f);
		style.ScrollbarSize = sizes.GetFloat("scroll_size", 0.f);
		style.ScrollbarRounding = sizes.GetFloat("scrollbar_rounding", 0.f);
		style.GrabMinSize = sizes.GetFloat("grab_min_size", 0.f);
		style.GrabRounding = sizes.GetFloat("grab_rounding", 0.f);

	}

	RELEASE_ARRAY(buffer);
}

void M_Editor::SaveStyle(ImGuiStyle * style)
{
	JsonFile file;

	JsonFile colors = file.AddSection("colors");

	for (uint i = 0; i < ImGuiCol_COUNT; ++i)
	{
		colors.AddFloatArray(ImGui::GetStyleColName(i), (float*)&style->Colors[i], 4);
	}
	
	JsonFile rendering = file.AddSection("rendering");
	
	rendering.AddBool("anti_aliased_lines", style->AntiAliasedLines);
	rendering.AddBool("anti_aliased_shapes", style->AntiAliasedShapes);
	rendering.AddFloat("curve_tessellation_tolerance", style->CurveTessellationTol);
	rendering.AddFloat("alpha", style->Alpha);

	JsonFile sizes = file.AddSection("sizes");

	sizes.AddFloatArray("win_padding", (float*)&style->WindowPadding, 2);
	sizes.AddFloat("win_rounding", style->WindowRounding);
	sizes.AddFloat("child_win_rounding", style->ChildWindowRounding);
	sizes.AddFloatArray("frame_padding", (float*)&style->FramePadding, 2);
	sizes.AddFloat("frame_rounding", style->FrameRounding);
	sizes.AddFloatArray("item_spacing", (float*)&style->ItemSpacing, 2);
	sizes.AddFloatArray("item_inner_spacing", (float*)&style->ItemInnerSpacing, 2);
	sizes.AddFloatArray("touch_extra_padding", (float*)&style->TouchExtraPadding, 2);
	sizes.AddFloat("indent_spacing", style->IndentSpacing);
	sizes.AddFloat("scroll_size", style->ScrollbarSize);
	sizes.AddFloat("scrollbar_rounding", style->ScrollbarRounding);
	sizes.AddFloat("grab_min_size", style->GrabMinSize);
	sizes.AddFloat("grab_rounding", style->GrabRounding);

	char* buffer = nullptr;
	uint size = file.WriteJson(&buffer, false);

	if (buffer && size > 0)
	{
		std::string savePath(CONFIG_PATH + std::string("style.json"));
		if (app->fs->Save(savePath.c_str(), buffer, size) != size)
		{
			_LOG("EDITOR_ERROR: Could not save style file in [%s].", savePath.c_str());
		}
		else
		{
			_LOG("Editor style saved: [%s].", savePath.c_str());
		}
	}

	RELEASE_ARRAY(buffer);
}

void M_Editor::SetStyleEditorWin()
{
	ImGui::Begin("Style editor", &styleEditor);
	{
		ImGuiStyle& style = ImGui::GetStyle();

		if (ImGui::CollapsingHeader("Sizes"))
		{
			ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat("ChildWindowRounding", &style.ChildWindowRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
			ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 16.0f, "%.0f");
		}

		if (ImGui::CollapsingHeader("Colors"))
		{

			static ImGuiColorEditMode editMode = ImGuiColorEditMode_RGB;
			ImGui::RadioButton("RGB", &editMode, ImGuiColorEditMode_RGB);
			ImGui::SameLine();
			ImGui::RadioButton("HSV", &editMode, ImGuiColorEditMode_HSV);
			ImGui::SameLine();
			ImGui::RadioButton("HEX", &editMode, ImGuiColorEditMode_HEX);
			
			static ImGuiTextFilter filter;
			filter.Draw("Filter colors", 200);

			ImGui::BeginChild("Col edit", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

			ImGui::ColorEditMode(editMode);

			for (uint i = 0; i < ImGuiCol_COUNT; ++i)
			{
				const char* name = ImGui::GetStyleColName(i);

				if (!filter.PassFilter(name))
					continue;

				ImGui::ColorEdit4(name, (float*)&style.Colors[i], true);
			}
			ImGui::EndChild();

		}

		if (ImGui::CollapsingHeader("Rendering"))
		{
			ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
			ImGui::Checkbox("Anti-aliased shapes", &style.AntiAliasedShapes);
			ImGui::PushItemWidth(100);
			ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, NULL, 2.0f);
			if (style.CurveTessellationTol < 0.0f) style.CurveTessellationTol = 0.10f;
			ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");
		}

		if (ImGui::Button("Save style")) SaveStyle(&style);
		ImGui::SameLine();
		if (ImGui::Button("Set default imgui style")) SetStyle((CONFIG_PATH + std::string("default_imgui_style.json")).c_str());

		ImGui::End();
	}
}
