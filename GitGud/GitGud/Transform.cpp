#include "Transform.h"

#include "App.h"
#include "M_GoManager.h"

#include "JsonFile.h"
#include "GameObject.h"


Transform::Transform(GameObject* object) : Component(object, CMP_TRANSFORM)
{
}


Transform::~Transform()
{
	if (object)
		object->transform = nullptr;
}

void Transform::OnEnable()
{
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

float3 Transform::GetLocalPosition() const
{
	return transaltion;
}

float3 Transform::GetGlobalPosition() const
{
	return globalTransform.TranslatePart();
}

void Transform::SetLocalPosition(const float3 & pos)
{
	transaltion = pos;
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

float3 Transform::GetLocalScale() const
{
	return scale;
}

void Transform::SetLocalScale(const float3 & scl)
{
	scale = scl;
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

float3 Transform::GetLocalRotation() const
{
	return editorRotation;
}

Quat Transform::GetLocalQuatRotation() const
{
	return rotation;
}

void Transform::SetLocalRotation(float3 & eulerRot)
{
	float3 eRot = eulerRot - editorRotation;
	Quat qRot = Quat::FromEulerXYZ(eRot.x, eRot.y, eRot.z);
	rotation = rotation * qRot;
	editorRotation = eulerRot;
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

void Transform::SetLocalRotation(const Quat & rot)
{
	rotation = rot;
	editorRotation = rotation.ToEulerXYZ().Abs();
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

const float4x4 Transform::GetGlobalTransform() const
{
	return globalTransform;
}

const float4x4 Transform::GetLocalTransform() const
{
	return localTransform;
}

void Transform::SetLocalTransform(const float4x4 & transform)
{
	transform.Decompose(transaltion, rotation, scale);
	editorRotation = rotation.ToEulerXYZ().Abs();
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

const float * Transform::GetGlobalTransformGL() const
{
	return globalTransform.ptr();
}

void Transform::UpdateTransform(const float4x4 & parentMat)
{
	localTransformHasChanged = false;
	localTransform = float4x4::FromTRS(transaltion, rotation, scale);
	globalTransform = parentMat * localTransform;
}

void Transform::OnSaveCmp(JsonFile & sect) const
{
	sect.AddInt("cmp_type", (int)type);
	sect.AddBool("active", selfActive);
	sect.AddInt("go_int", object->GetUuid());

	sect.AddFloat3("position", transaltion);
	sect.AddFloat3("scale", scale);
	sect.AddFloatArray("rotation", (const float*)rotation.ptr(), 4);
}

void Transform::OnLoadCmp(JsonFile * sect)
{
	if (!sect)return;

	selfActive = sect->GetBool("active", true);

	SetLocalPosition(sect->GetFloat3("position", float3::zero));
	SetLocalScale(sect->GetFloat3("scale", float3::one));
	Quat r;
	r.x = sect->GetFloat("rotation", 0.f, 0);
	r.y = sect->GetFloat("rotation", 0.f, 1);
	r.z = sect->GetFloat("rotation", 0.f, 2);
	r.w = sect->GetFloat("rotation", 1.f, 3);
	SetLocalRotation(r);
}
