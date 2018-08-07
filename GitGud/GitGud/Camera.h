#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Component.h"
#include "Math.h"
#include "Color.h"

class Transform;
class JsonFile;

enum CameraType
{
	CAM_PERSPECTIVE = 0,
	CAM_ORTHOGRAPHIC
};

class Camera : public Component
{
public:
	Camera(GameObject* object);
	virtual ~Camera();

	float GetFOV()const;
	void SetFOV(float vFov);

	float GetOthogonalSize()const;
	void SetOrthoSize(float size);

	float GetFarPlaneDist()const;
	void SetFarPlaneDist(float fD);

	float GetNearPlaneDist()const;
	void SetNearPlaneDist(float nD);

	float GetAspectRatio()const;
	void SetAspectRatio(float ar);

	bool IsCulling()const;
	void SetCulling(bool set);

	Color GetBackground()const;
	void GetBackground(float& r, float& g, float& b, float& a);
	void SetBackground(Color col);
	void SetBackground(float r, float g, float b, float a = 1.f);

	//View mat must be updated every frame
	float* GetGLViewMatrix();
	
	float* GetGLProjectionMatrix();

	void OnTransformUpdate(Transform* trans)override;

	CameraType GetType()const;
	void SetType(CameraType type);
	void SwapType();

	void Look(const float3 spot, const float3 pos);
	void LookAt(const float3 spot);

	void OnSaveCmp(JsonFile& sect)const override;
	void OnLoadCmp(JsonFile* sect)override;

	void OnDebugDraw() override;

private:

public:
	Color backgorund = Black;
	Frustum frustum;
	bool projectionMatChaged = false;

private:
	bool culling = true;
	float aspectRatio = 16 / 9;
	float orthoSize = 1;
	CameraType camType;
};

#endif // !__CAMERA_H__