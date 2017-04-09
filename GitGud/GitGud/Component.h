#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"
#include "Math.h"

class Transform;
class JsonFile;

enum COMPONENT_TYPE
{
	CMP_UNKNOWN = 0,
	CMP_TRANSFORM = 1,
	CMP_MESH = 2,
	CMP_MATERIAL = 4,
	CMP_CAMERA = 8,
	CMP_LIGHT = 16
};

class Component
{
public:
	Component(GameObject* object, COMPONENT_TYPE type);
	virtual ~Component();

	//--------------------------

	bool IsActive()const;
	void SetActive(bool set);
	void SwitchActive();
	void Enable();
	void Disable();

	//--------------------------

	COMPONENT_TYPE GetType()const;
	GameObject* GetGameObject()const;

	//--------------------------

	void Destroy();

	//--------------------------

	virtual void GetBox(AABB& box)const{}

	virtual void OnEnable() {}
	virtual void OnDisable() {}

	virtual void OnStart(){}
	virtual void PreUpdate(float dt) {}
	virtual void OnUpdate(float dt) {}
	virtual void OnFinish() {}

	virtual void OnDebugDraw() {}

	virtual void OnPlay() {}
	virtual void OnStop() {}
	virtual void OnPause() {}
	virtual void OnUnPause() {}

	virtual void OnGameObjectDestroyed() {}

	virtual void OnTransformUpdate(Transform* trans) {}

	//--------------------------

	virtual void OnSaveCmp(JsonFile& sect)const = 0;
	virtual void OnLoadCmp(JsonFile* sect) = 0;


private:


public:

private:

protected:
	COMPONENT_TYPE type = CMP_UNKNOWN;
	GameObject* object = nullptr;
	bool selfActive = true;
};

#endif // !__COMPONENT_H__