#include "Camera.h"

#include "App.h"

#include "GameObject.h"
#include "Transform.h"

#include "DrawDebugTools.h"

#include "M_Renderer.h"

#include "JsonFile.h"


Camera::Camera(GameObject* object) : Component(object, CMP_CAMERA)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	if (object && object->transform)
	{
		float4x4 mat = object->transform->GetLocalTransform();
		frustum.SetFrame(mat.TranslatePart(), mat.WorldZ(), mat.WorldY());
	}
	else
	{
		frustum.SetFrame(float3(0.f, 0.f, 0.f), float3::unitZ, float3::unitY);
	}

	frustum.SetViewPlaneDistances(1.f, 100.f);
	frustum.SetVerticalFovAndAspectRatio(45.f * DEGTORAD, aspectRatio);

	camType = CAM_PERSPECTIVE;
}


Camera::~Camera()
{
}

/** Camera - GetFOV: Returns the camera vertical fov. */
float Camera::GetFOV() const
{
	return frustum.VerticalFov() * RADTODEG;
}

/** Camera - SetFOV: Sets the vertical FOV. */
void Camera::SetFOV(float vFov)
{
	if (vFov > 0.5f)
	{
		frustum.SetVerticalFovAndAspectRatio(vFov * DEGTORAD, aspectRatio);
		projectionMatChaged = true;
	}
}

/** Camera - GetOrthogonalSize: Return the orthogonal size. */
float Camera::GetOthogonalSize() const
{
	return orthoSize;
}

/** Camera - SetOrthoSize: Sets the orthogonal size. */
void Camera::SetOrthoSize(float size)
{
	//TODO: Should always be related to viewport size
	frustum.SetOrthographic(16 * orthoSize, 9 * orthoSize);
	projectionMatChaged = true;
}

/** Camera - GetFarPlaneDist: Return the far plane distance. */
float Camera::GetFarPlaneDist() const
{
	return frustum.FarPlaneDistance();
}

/** Camera - SetFarPlaneDist: Sets the far plane distance. */
void Camera::SetFarPlaneDist(float fD)
{
	if (fD > frustum.NearPlaneDistance())
	{
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), fD);
		projectionMatChaged = true;
	}
}

/** Camera - GetNearPlaneDist: Return the near plane distance. */
float Camera::GetNearPlaneDist() const
{
	return frustum.NearPlaneDistance();
}

/** Camera - SetNearPlaneDist: Sets the near plane distance. */
void Camera::SetNearPlaneDist(float nD)
{
	if (nD > 0.f && nD < frustum.FarPlaneDistance())
	{
		frustum.SetViewPlaneDistances(nD, frustum.FarPlaneDistance());
		projectionMatChaged = true;
	}
}

/** Camera - GetAspectRatio: Return the camera aspect ratio. */
float Camera::GetAspectRatio() const
{
	return aspectRatio;
}

/** Camera - SetAspectRatio: Sets the camera aspect ratio. */
void Camera::SetAspectRatio(float ar)
{
	aspectRatio = ar;
	frustum.SetVerticalFovAndAspectRatio(frustum.VerticalFov(), aspectRatio);
	projectionMatChaged = true;
}

/** Camera - IsCulling: Return true if camera is setted to frustum cull. */
bool Camera::IsCulling() const
{
	return culling;
}

/** Camera - SetCulling: Set the frustum culling flag. */
void Camera::SetCulling(bool set)
{
	culling = set;
}

/** Camera - GetBackgorund: Return the camera background color. */
Color Camera::GetBackground() const
{
	return backgorund;
}

/** Camera - GetBackgorund: Gives the camera background color by reference. */
void Camera::GetBackground(float & r, float & g, float & b, float & a)
{
	r = backgorund.r;
	g = backgorund.g;
	b = backgorund.b;
	a = backgorund.a;
}

/** Camera - SetBackground: Sets the camera background color. */
void Camera::SetBackground(Color col)
{
	backgorund = col;
}

/** Camera - SetBackground: Sets the camera background color. */
void Camera::SetBackground(float r, float g, float b, float a)
{
	backgorund.Set(r, g, b, a);
}

/** Camera - GetGLViewMatrix: Return the view matrix pointer for the view transformation. */
float * Camera::GetGLViewMatrix()
{
	static float4x4 ret;
	ret = frustum.ViewMatrix();
	ret.Transpose();
	return (float*)ret.v;
}

/** Camera - GetGLProjectionMatrix: Return the projection matrix pointer for the view transformation. */
float * Camera::GetGLProjectionMatrix()
{
	static float4x4 ret;
	ret = frustum.ProjectionMatrix();
	ret.Transpose();
	return (float*)ret.v;
}

/** Camera - OnTransformUpdate: Refactors the camera position, front vector and right vector. */
void Camera::OnTransformUpdate(Transform * trans)
{
	if (trans)
	{
		float4x4 mat = trans->GetGlobalTransform();
		frustum.SetFrame(mat.TranslatePart(), mat.WorldZ(), mat.WorldY());
	}
}

/** Camera - GetType: Return the camera type. */
CameraType Camera::GetType() const
{
	return camType;
}

/** Camera - SetType: Set the camera type. */
void Camera::SetType(CameraType type)
{
	if (type != camType)
		SwapType();
}

/** Camera - SwapType: Swap the camera type. */
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

/** Camera - Look: Set the camera to look to a spot from a position. */
void Camera::Look(const float3 spot, const float3 pos)
{
	frustum.SetPos(pos);
	LookAt(spot);
}

/** Camera - LookAt: Set the camera to look to a spot from the same position. */
void Camera::LookAt(const float3 spot)
{
	float3 dir = spot - frustum.Pos();
	float3x3 mat = float3x3::LookAt(frustum.Front(), dir.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(mat.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(mat.MulDir(frustum.Up()).Normalized());
	projectionMatChaged = true;
}

/** Camera - OnSaveCmp: Saves the camera component info into the GO save file. */
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

/** Camera - OnLoadCmp: Loads the camera component info from the GO save file. */
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

void Camera::OnDebugDraw()
{
	DrawDebug::DrawFrustumDebug(frustum, Yellow);
}
