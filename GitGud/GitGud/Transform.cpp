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

/** Transform - OnEnable: Marks some flags to recalculate transforms when this transform is activated. */
void Transform::OnEnable()
{
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

/** Transform - GetLocalPosition: Return the local position. */
float3 Transform::GetLocalPosition() const
{
	return transaltion;
}

/** Transform - GetGlobalPositon: Return the glboal position. */
float3 Transform::GetGlobalPosition() const
{
	return globalTransform.TranslatePart();
}

/** Transform - SetLocalPositon: Sets the transform local position. */
void Transform::SetLocalPosition(const float3 & pos)
{
	transaltion = pos;
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

/** Transform - GetLocalScale: Return the local scale. */
float3 Transform::GetLocalScale() const
{
	return scale;
}

/** Transform - SetLocalScale: Sets the local scale. */
void Transform::SetLocalScale(const float3 & scl)
{
	scale = scl;
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

/** Transform - GetLocalRotation: Return the local rotation as euler angles. */
float3 Transform::GetLocalRotation() const
{
	return editorRotation;
}

/** Transform - GetLocalQuatRotation: Returns the local rotation as quaternion. */
Quat Transform::GetLocalQuatRotation() const
{
	return rotation;
}

/** Transform - SetLocalRotation: Sets the local rotation by euler angles. */
void Transform::SetLocalRotation(float3 & eulerRot)
{
	float3 eRot = eulerRot - editorRotation;
	Quat qRot = Quat::FromEulerXYZ(eRot.x, eRot.y, eRot.z);
	rotation = rotation * qRot;
	editorRotation = eulerRot;
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

/** Transform - SetLocalRotation: Sets the local rotation by a quaternion. */
void Transform::SetLocalRotation(const Quat & rot)
{
	rotation = rot;
	editorRotation = rotation.ToEulerXYZ().Abs();
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

/** Transform - GetGlobalTransform: Return a 4x4 matrix of the global transform. */
const float4x4 Transform::GetGlobalTransform() const
{
	return globalTransform;
}

/** Transform - GetLocalTransform: Return a 4x4 matrix of the local transform. */
const float4x4 Transform::GetLocalTransform() const
{
	return localTransform;
}

/** Transform - SetLocalTransform: Sets the local 4x4 transform. */
void Transform::SetLocalTransform(const float4x4 & transform)
{
	transform.Decompose(transaltion, rotation, scale);
	editorRotation = rotation.ToEulerXYZ().Abs();
	localTransformHasChanged = true;
	app->goManager->anyGOTransHasChanged = true;
}

/** Transform - GetGlobalTransformGL: Returns a float pointer of the global transform adapted to OpenGL math system. */
const float * Transform::GetGlobalTransformGL() const
{
	return globalTransform.Transposed().ptr();
}

/** Transform - UpdateTransform: Updates the transform to match the hierarchy. */
void Transform::UpdateTransform(const float4x4 & parentMat)
{
	localTransformHasChanged = false;
	localTransform = float4x4::FromTRS(transaltion, rotation, scale);
	globalTransform = parentMat * localTransform;
}

/** Transform - OnSaveCmp: Save the transform into the GO save object. */
void Transform::OnSaveCmp(JsonFile & sect) const
{
	sect.AddInt("cmp_type", (int)type);
	sect.AddBool("active", selfActive);
	sect.AddInt("go_int", object->GetUuid());

	sect.AddFloat3("position", transaltion);
	sect.AddFloat3("scale", scale);
	sect.AddQuaternion("rotation", rotation);
}

/** Transform - OnLoadCmp: Loads the transform from the GO save file. */
void Transform::OnLoadCmp(JsonFile * sect)
{
	if (!sect)return;

	selfActive = sect->GetBool("active", true);

	SetLocalPosition(sect->GetFloat3("position", float3::zero));
	SetLocalScale(sect->GetFloat3("scale", float3::one));
	SetLocalRotation(sect->GetQuaternion("rotation", Quat::identity));
}
