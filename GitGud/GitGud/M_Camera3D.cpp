#include "M_Camera3D.h"

#include "App.h"
#include "M_Input.h"
#include "M_GoManager.h"
#include "M_Window.h"
#include "M_Editor.h"
#include <SDL_scancode.h>
#include <SDL_mouse.h>

#include "GameObject.h"
#include "Camera.h"

/** M_Camera3D: Creates the editor camera using a camera component. */
M_Camera3D::M_Camera3D(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG(LOG_INFO, "EditrCamera3D: Creation.");

	editorCameraObj = new GameObject(nullptr, 0);
	if (editorCameraObj)
	{
		editorCameraObj->SetName("Editor camera");
		editorCamera = (Camera*)editorCameraObj->CreateComponent(CMP_CAMERA);
		editorCamera->Look(float3::zero, float3(-5, 5, 7));
		editorCamera->SetBackground(DarkGrey);
	}

	configuration = M_UPDATE | M_CLEAN_UP | M_SAVE_CONFIG | M_RESIZE_EVENT;
}


M_Camera3D::~M_Camera3D()
{
	RELEASE(editorCameraObj);

	_LOG(LOG_INFO, "EditrCamera3D: Destroying.");
}

/** M_Camera3D - Init:  */
bool M_Camera3D::Init(JsonFile * conf)
{
	_LOG(LOG_INFO, "EditrCamera3D: Init.");
	return true;
}

/** M_Camera3D - Start:  */
bool M_Camera3D::Start()
{
	_LOG(LOG_INFO, "EditrCamera3D: Start.");
	return true;
}

/** M_Camera3D - Update: Check the editor camera input. */
UpdateReturn M_Camera3D::Update(float dt)
{
	if (!app->editor->UsingKeyboard())
		Move(dt);

	if (!app->editor->UsingMouse())
	{
		Rotate(dt);
		Zoom(dt);

		if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			app->goManager->SelectGo(Pick());
		}
	}

	return UPDT_CONTINUE;
}

bool M_Camera3D::CleanUp()
{
	_LOG(LOG_INFO, "EditrCamera3D: CleanUp.");
	return true;
}

GameObject * M_Camera3D::GetEditorCamObj() const
{
	return editorCameraObj;
}

Camera * M_Camera3D::GetEditorCamera() const
{
	return editorCamera;
}

void M_Camera3D::Look(const float3 & position, const float3 & reference, bool rotateAroundReference)
{
	editorCamera->Look(position, reference);
}

void M_Camera3D::LookAt(const float3 & spot)
{
	editorCamera->LookAt(spot);
}

void M_Camera3D::LookAt(float dx, float dy)
{
	//dx will be rotation along x axis
	if (dx != 0.f)
	{
		Quat rot = Quat::RotateY(dx);
		editorCamera->frustum.SetFront(rot.Mul(editorCamera->frustum.Front()).Normalized());
		editorCamera->frustum.SetUp(rot.Mul(editorCamera->frustum.Up()).Normalized());
	}

	//dy will be rotation along y axis
	//more complex as the frustum up changes
	if (dy != 0.f)
	{
		Quat rot = Quat::RotateAxisAngle(editorCamera->frustum.WorldRight(), dy);

		editorCamera->frustum.SetFront(rot.Mul(editorCamera->frustum.Front()).Normalized());
		editorCamera->frustum.SetUp(rot.Mul(editorCamera->frustum.Up()).Normalized());
	}
}

void M_Camera3D::Move(float dt)
{
	Frustum* frust = &editorCamera->frustum;

	float speed;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = movSpeed / 2;
	else
		speed = movSpeed;

	float3 movement(float3::zero);
	float3 forw(frust->Front());
	float3 right(frust->WorldRight());

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += forw;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= forw;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += right;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= right;
	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += float3::unitY;
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) movement -= float3::unitY;

	if (!movement.Equals(float3::zero))
	{
		movement *= (speed * dt);
		frust->SetPos(frust->Pos() + movement);
	}
}

void M_Camera3D::Rotate(float dt)
{
	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int motX = app->input->GetMouseMotionX();
		int motY = app->input->GetMouseMotionY();

		if (motX == 0 && motY == 0)
			return;

		float speed;
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = rotSpeed / 2;
		else
			speed = rotSpeed;

		float x = (float)-motX * speed * dt;
		float y = (float)-motY * speed * dt;

		if (app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
			Orbit(x, y);
		else
			LookAt(x, y);
	}
}

void M_Camera3D::Orbit(float dx, float dt)
{
}

void M_Camera3D::Zoom(float dt)
{
}

GameObject * M_Camera3D::Pick() const
{
	/**
	*	-1, 1   ----------------   1, 1
	*			|				|
	*			|				|
	*			|				|
	*	-1, -1  ----------------   1, -1
	*/

	float w = (float)app->win->GetWidth();
	float h = (float)app->win->GetHeight();

	int x, y;
	app->input->GetMouseScreenPos(x, y);

	float normX = -(1.f - ((float)x * 2.f) / w);
	float normY = 1.f - ((float)y * 2.f) / h;

	LineSegment picking = editorCamera->frustum.UnProjectLineSegment(normX, normY);
	float dist;
	GameObject* hit = app->goManager->CastRay(picking, dist);

	return hit;
}
