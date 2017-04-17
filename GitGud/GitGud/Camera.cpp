#include "Camera.h"

#include "App.h"

#include "GameObject.h"
#include "Transform.h"

#include "M_Renderer.h"

#include "JsonFile.h"


Camera::Camera(GameObject* object) : Component(object, CMP_CAMERA)
{
	if (object && object->transform)
	{
		float4x4 mat = object->transform->GetLocalTransform();
		frustum.SetPos(mat.TranslatePart());
		frustum.SetFront(mat.WorldZ());
		frustum.SetUp(mat.WorldY());
	}
	else
	{
		frustum.SetPos(float3(0.f, 0.f, 0.f));
		frustum.SetFront(float3(0.f, 0.f, 1.f));
		frustum.SetUp(float3(0.f, 1.f, 0.f));
	}

	frustum.SetViewPlaneDistances(1.f, 100.f);
	frustum.SetVerticalFovAndAspectRatio(45.f * DEGTORAD, aspectRatio);
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);

	camType = CAM_PERSPECTIVE;
}


Camera::~Camera()
{
}

float Camera::GetFOV() const
{
	return frustum.VerticalFov() * RADTODEG;
}

void Camera::SetFOV(float vFov)
{
	if (vFov > 0.5f)
	{
		frustum.SetVerticalFovAndAspectRatio(vFov * DEGTORAD, aspectRatio);
		projectionMatChaged = true;
	}
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
	sect.AddInt("type", (int)type);
	sect.AddBool("active", selfActive);
	sect.AddInt("go_uuid", object->GetUuid());

	sect.AddFloat("near_plane", frustum.NearPlaneDistance());
	sect.AddFloat("far_plane", frustum.FarPlaneDistance());
	sect.AddFloat("fov", frustum.VerticalFov());
	sect.AddFloat("ar", frustum.AspectRatio());
	//TODO: Save ortho mode etc

	sect.AddFloat3("pos", frustum.Pos());
	sect.AddFloat3("fornt", frustum.Front());
	sect.AddFloat3("up", frustum.Up());

	//TODO: Save if active
}

void Camera::OnLoadCmp(JsonFile * sect)
{
	if (sect)
	{
		selfActive = sect->GetBool("active", true);

		float3 p = sect->GetFloat3("pos", float3(0.f, 0.f, 0.f));
		float3 f = sect->GetFloat3("front", float3(0.f, 0.f, 1.f));
		float3 u = sect->GetFloat3("up", float3(0.f, 1.f, 0.f));

		float n = sect->GetFloat("near_plane", 100.f);
		float fa = sect->GetFloat("far_plane", 100.f);
		float fov = sect->GetFloat("fov", 45.f);
		float ar = sect->GetFloat("ar", 16.f / 9.f);

		frustum.SetFrame(p, f, u);
		frustum.SetViewPlaneDistances(n, fa);
		frustum.SetVerticalFovAndAspectRatio(fov, ar);
	}
}
