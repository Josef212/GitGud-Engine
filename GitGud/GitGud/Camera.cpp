#include "Camera.h"

#include "App.h"

#include "GameObject.h"
#include "Transform.h"

#include "M_Renderer.h"


Camera::Camera(GameObject* object) : Component(object, CMP_CAMERA)
{
	frustum.SetPos(float3(0.f, 0.f, 0.f));
	frustum.SetFront(float3(0.f, 0.f, -1.f));
	frustum.SetUp(float3(0.f, 1.f, 0.f));

	frustum.SetViewPlaneDistances(1.f, 100.f);
	frustum.SetVerticalFovAndAspectRatio(45.f, aspectRatio);
	camType = CAM_PERSPECTIVE;
}


Camera::~Camera()
{
}

float Camera::GetFOV() const
{
	return frustum.VerticalFov();
}

void Camera::SetFOV(float vFov)
{
	frustum.SetVerticalFovAndAspectRatio(vFov, aspectRatio);
	projectionMatChaged = true;
}

float Camera::GetOthogonalSize() const
{
	return orthoSize;
}

void Camera::SetOrthoSize(float size)
{
	//TODO: Should always be related to viewport size
	frustum.SetOrthographic(16 * orthoSize, 9 * orthoSize);
	projectionMatChaged = true;
}

float Camera::GetFarPlaneDist() const
{
	return frustum.FarPlaneDistance();
}

void Camera::SetFarPlaneDist(float fD)
{
	if (fD > frustum.NearPlaneDistance())
	{
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), fD);
		projectionMatChaged = true;
	}
}

float Camera::GetNearPlaneDist() const
{
	return frustum.NearPlaneDistance();
}

void Camera::SetNearPlaneDist(float nD)
{
	if (nD > 0.f && nD < frustum.FarPlaneDistance())
	{
		frustum.SetViewPlaneDistances(nD, frustum.FarPlaneDistance());
		projectionMatChaged = true;
	}
}

float Camera::GetAspectRatio() const
{
	return aspectRatio;
}

void Camera::SetAspectRatio(float ar)
{
	aspectRatio = ar;
	frustum.SetVerticalFovAndAspectRatio(frustum.VerticalFov(), aspectRatio);
	projectionMatChaged = true;
}

bool Camera::IsCulling() const
{
	return culling;
}

void Camera::SetCulling(bool set)
{
	culling = set;
}

Color Camera::GetBackground() const
{
	return backgorund;
}

void Camera::GetBackground(float & r, float & g, float & b, float & a)
{
	r = backgorund.r;
	g = backgorund.g;
	b = backgorund.b;
	a = backgorund.a;
}

void Camera::SetBackground(Color col)
{
	backgorund = col;
}

void Camera::SetBackground(float r, float g, float b, float a)
{
	backgorund.Set(r, g, b, a);
}

float * Camera::GetGLViewMatrix()
{
	static float4x4 ret;
	ret = frustum.ViewMatrix();
	ret.Transpose();
	return (float*)ret.v;
}

float * Camera::GetGLProjectionMatrix()
{
	static float4x4 ret;
	ret = frustum.ProjectionMatrix();
	ret.Transpose();
	return (float*)ret.v;
}

void Camera::OnTransformUpdate(Transform * trans)
{
	if (trans)
	{
		float4x4 mat = trans->GetGlobalTransform();

		frustum.SetPos(mat.TranslatePart());
		frustum.SetFront(mat.WorldZ());
		frustum.SetUp(mat.WorldY());
	}
}

CAM_TYPE Camera::GetType() const
{
	return camType;
}

void Camera::SetType(CAM_TYPE type)
{
	if (type != camType)
		SwapType();
}

void Camera::SwapType()
{
	//TODO: Ortho cam must be checked and prob fixed
	if (camType == CAM_PERSPECTIVE)
	{
		camType = CAM_ORTHOGRAPHIC;
		frustum.SetOrthographic(16 * orthoSize, 9 * orthoSize);
	}
	else
	{
		camType = CAM_PERSPECTIVE;
		frustum.SetPerspective(frustum.HorizontalFov(), frustum.VerticalFov());
	}
}

void Camera::Look(const float3 spot, const float3 pos)
{
	frustum.SetPos(pos);
	LookAt(spot);
	projectionMatChaged = true;
}

void Camera::LookAt(const float3 spot)
{
	float3 dir = spot - frustum.Pos();
	float3x3 mat = float3x3::LookAt(frustum.Front(), dir.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(mat.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(mat.MulDir(frustum.Up()).Normalized());
}

void Camera::OnSaveCmp(JsonFile & sect) const
{
}

void Camera::OnLoadCmp(JsonFile * sect)
{
}
