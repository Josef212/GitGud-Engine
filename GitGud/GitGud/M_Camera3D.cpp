#include "M_Camera3D.h"

#include "App.h"
#include "M_Input.h"

#include "GameObject.h"
#include "Camera.h"


M_Camera3D::M_Camera3D(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG("EditrCamera3D: Creation.");

	editorCameraObj = new GameObject(nullptr, 0);
	if (editorCameraObj)
	{
		editorCameraObj->SetName("Editor camera");
		editorCamera = (Camera*)editorCameraObj->CreateComponent(CMP_CAMERA);
		editorCamera->Look(float3::zero, float3(0, 10, 10));
	}
}


M_Camera3D::~M_Camera3D()
{
	_LOG("EditrCamera3D: Destroying.");
}

bool M_Camera3D::Init(JsonFile * conf)
{
	_LOG("EditrCamera3D: Init.");
	return true;
}

bool M_Camera3D::Start()
{
	_LOG("EditrCamera3D: Start.");
	return true;
}

UPDATE_RETURN M_Camera3D::Update(float dt)
{
	return UPDT_CONTINUE;
}

bool M_Camera3D::CleanUp()
{
	_LOG("EditrCamera3D: CleanUp.");
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
