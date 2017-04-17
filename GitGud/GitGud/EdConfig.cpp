#include "EdConfig.h"
#include "Globals.h"
#include "imGui/imgui.h"

#include "M_Camera3D.h"

#include "App.h"
#include "HrdInfo.h"
#include "mmgr/mmgr.h"

#include "M_Renderer.h"
#include "M_Window.h"
#include "M_Input.h"
#include "M_FileSystem.h"


#include "Camera.h"


EdConfig::EdConfig(bool startEnabled) : EdWin(startEnabled)
{
}


EdConfig::~EdConfig()
{
}

void EdConfig::Draw()
{
	ImGui::Begin("Configuration", &active);
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			static char appName[60];
			strcpy_s(appName, 60, app->GetTitle());
			if (ImGui::InputText("App name", appName, 60)) app->SetTitle(appName);

			static char org[60];
			strcpy_s(org, 60, app->GetOrganitzation());
			if (ImGui::InputText("Organitzation", org, 60)) app->SetOrganitzation(org);

			static int max = app->GetMaxFPS();
			if (ImGui::SliderInt("Max FPS", &max, 0, 240)) app->SetMaxFPS(max);

			ImGui::Text("Limit framerate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%i", max);

			if (arraySize > 0)

			{
				char title[25];
				sprintf_s(title, 25, "Framerate %.1f", fps[arraySize - 1]);
				ImGui::PlotHistogram("##framerate", &fps[0], arraySize, 0, title, 0.0f, 100.0f, ImVec2(310, 100));
				sprintf_s(title, 25, "Milliseconds %0.1f", ms[arraySize - 1]);
				ImGui::PlotHistogram("##milliseconds", &ms[0], arraySize, 0, title, 0.0f, 40.0f, ImVec2(310, 100));
			}
			
			// Memory --------------------
			sMStats stats = m_getMemoryStatistics();
			static int speed = 0;
			static std::vector<float> memory(100);
			if (++speed > 20)
			{
				speed = 0;
				if (memory.size() == 100)
				{
					for (uint i = 0; i < 100 - 1; ++i)
						memory[i] = memory[i + 1];

					memory[100 - 1] = (float)stats.totalReportedMemory;
				}
				else
					memory.push_back((float)stats.totalReportedMemory);
			}

			ImGui::PlotHistogram("##memory", &memory[0], memory.size(), 0, "Memory Consumption", 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));

			ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
			ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
			ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
			ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
			ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
			ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
			ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
			ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
			ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);

		}

		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::Text("Icon:");//TODO: add functionality to change app icon
			float brightness = app->win->GetBrightness();
			if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f)) app->win->SetBrightness(brightness);

			int width, height, minW, minH, maxW, maxH;
			app->win->GetWindowSize(width, height);
			app->win->GetRange(minW, minH, maxW, maxH);
			if (ImGui::SliderInt("Width", &width, minW, maxW)) app->win->SetWidth(width);
			if (ImGui::SliderInt("Height", &height, minH, maxH)) app->win->SetHeight(height);

			int x = 0, y = 0;
			int maxX = maxW - width;
			int maxY = maxH - height;
			app->win->GetPosition(x, y);
			if (ImGui::SliderInt("Position X", &x, 0, maxX)) app->win->SetPosition(x, y);
			if (ImGui::SliderInt("Position Y", &y, 0, maxY)) app->win->SetPosition(x, y);

			if (ImGui::InputInt("PositionX", &x)) app->win->SetPosition(x, y);
			if (ImGui::InputInt("PositionY", &y)) app->win->SetPosition(x, y);

			ImGui::Text("Refresh rate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%i", app->win->GetRefresh());

			ImGui::Separator();

			bool fullscreen = app->win->IsFullScreen();
			bool resizable = app->win->IsResizable();
			bool borderless = app->win->IsBorderless();
			bool fullscreenDesktop = app->win->IsFullscreenDesktop();
			if (ImGui::Checkbox("Fullscreen", &fullscreen)) app->win->SetFullScreen(fullscreen);
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable)) app->win->SetResizable(resizable);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.69, 0.13, 0.13, 1), "Reset app too apply."); //TODO: add functionality to save and load to change resizale
			if (ImGui::Checkbox("Borderless", &borderless)) app->win->SetBorderless(borderless);
			ImGui::SameLine();
			if (ImGui::Checkbox("FullDesktop", &fullscreenDesktop)) app->win->SetFullScreenDesktop(fullscreenDesktop);
		}

		if (ImGui::CollapsingHeader("Renderer"))
		{
			bool vsync = app->renderer->GetVSync();
			if (ImGui::Checkbox("VSyc", &vsync)) app->renderer->SetVSync(vsync);
		}

		if(ImGui::CollapsingHeader("FileSystem"))
		{
			ImGui::Text("BasePath: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), app->fs->GetBaseDir());

			ImGui::Text("ReadPath: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "./");

			ImGui::Text("WritePath: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "./");
		}

		if(ImGui::CollapsingHeader("Input"))
		{
			int x, y, mx, my, z;
			app->input->GetMouseScreenPos(x, y);
			app->input->GetMouseMotion(mx, my);
			z = app->input->GetMouseWheel();

			ImGui::Text("Mouse position: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d,%d", x, y);

			ImGui::Text("Mouse motion: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d,%d", mx, my);

			ImGui::Text("Mouse wheel: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", z);
		}

		if (ImGui::CollapsingHeader("Information"))
		{
			ImGui::Text("SDL Version: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.%d.%d", app->info->GetInfo()->sdlVersion.major, app->info->GetInfo()->sdlVersion.minor, app->info->GetInfo()->sdlVersion.patch);
			
			ImGui::Separator();

			ImGui::Text("CPUs: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dCores (Cache: %dKb)", app->info->GetInfo()->cpuCores, app->info->GetInfo()->cpuCacheSize);

			ImGui::Text("System RAM: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dGb", app->info->GetInfo()->ram);

			ImGui::Text("Caps: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", app->info->GetCaps());

			ImGui::Separator();

			ImGui::TextColored(ImVec4(1, 0, 0, 1), "TODO: get GPU info");
		}

		if (ImGui::CollapsingHeader("Editor camera"))
		{
			Camera* cam = app->camera->GetEditorCamera();
			if (cam)
			{
				float3 pos = cam->frustum.Pos();
				float3 front = cam->frustum.Front();
				float3 up = cam->frustum.Up();

				ImGui::DragFloat3("Pos", (float*)&pos);
				ImGui::DragFloat3("Front", (float*)&front);
				ImGui::DragFloat3("Up", (float*)&up);

				ImGui::Separator();

				Color col = cam->GetBackground();
				if (ImGui::ColorEdit4("Backgorund", (float*)&col, false))
					cam->SetBackground(col);

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
					if (ImGui::DragFloat("FOV", &fov, 0.5f, 10.0f)) cam->SetFOV(fov);
				}
				else
				{
					float size = cam->GetOthogonalSize();
					if (ImGui::DragFloat("Size", &size)) cam->SetOrthoSize(size);
				}

				bool culling = cam->IsCulling();
				if (ImGui::Checkbox("Culling", &culling)) cam->SetCulling(culling);

				ImGui::DragFloat("Move speed", &app->camera->movSpeed, 0.25f, 0.01f);
				ImGui::DragFloat("Rot speed", &app->camera->rotSpeed, 0.25f, 0.01f);
			}
		}

		ImGui::End();
	}
}

void EdConfig::PushFps(float _fps, float _ms)
{
	if (arraySize == MAX_FPS_LOG)
	{
		for (uint i = 0; i < MAX_FPS_LOG - 1; ++i)
		{
			fps[i] = fps[i + 1];
			ms[i] = ms[i + 1];
		}
	}
	else
	{
		++arraySize;
	}

	fps[arraySize - 1] = _fps;
	ms[arraySize - 1] = _ms;
}
