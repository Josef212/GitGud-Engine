#ifndef __MCAMERA3D_H__
#define __MCAMERA3D_H__

#include "Module.h"
#include "Math.h"

class GameObject;
class Camera;

class M_Camera3D : public Module
{
public:
	M_Camera3D(const char* name, bool startEnabled = true);
	virtual ~M_Camera3D();

	bool Init(JsonFile* conf)override;
	bool Start()override;
	UPDATE_RETURN Update(float dt)override;
	bool CleanUp()override;


	GameObject* GetEditorCamObj()const;
	Camera* GetEditorCamera()const;

private:

public:
	float movSpeed = 10.f;
	float rotSpeed = 3.f;
	float zoomSpeed = 500.f;

private:
	GameObject* editorCameraObj = nullptr;
	Camera* editorCamera = nullptr;
};

#endif //!__MCAMERA3D_H__