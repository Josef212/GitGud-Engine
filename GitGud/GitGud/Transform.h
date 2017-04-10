#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"
#include "Math.h"

class JsonFile;

class Transform : public Component
{
public:
	Transform(GameObject* object);
	virtual ~Transform();

	void OnEnable()override;

	//----------------------------

	//Translation
	float3 GetLocalPosition()const;
	float3 GetGlobalPosition()const;

	void SetLocalPosition(const float3& pos);
	//TODO: Set global position??

	//Scale
	float3 GetLocalScale()const;

	void SetLocalScale(const float3& scl);

	//Rotation
	float3 GetLocalRotation()const;
	Quat GetLocalQuatRotation()const;

	void SetLocalRotation(float3& eulerRot);
	void SetLocalRotation(const Quat& rot);

	//Transform matrix
	const float4x4 GetGlobalTransform()const;
	const float4x4 GetLocalTransform()const;

	void SetLocalTransform(const float4x4& transform);

	//OpenGL
	const float* GetGlobalTransformGL()const;

	//----------------------------

	void UpdateTransform(const float4x4& parentMat);

	void OnSaveCmp(JsonFile& sect)const override;
	void OnLoadCmp(JsonFile* sect)override;

	//----------------------------

private:

public:
	bool localTransformHasChanged = true;


private:
	float3 transaltion = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity;
	float3 editorRotation = float3::zero;

	float4x4 globalTransform = float4x4::identity;
	float4x4 localTransform = float4x4::identity;
};

#endif // !__TRANSFORM_H__