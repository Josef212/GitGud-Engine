#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "Math.h"

#include <vector>
#include <map>
#include <string>

class Transform;
class Mesh;
class Material;
class Camera;

class JsonFile;


class GameObject
{
public:
	GameObject(GameObject* parent, UID uuid);
	virtual ~GameObject();

	//--------------------------

	UID GetUuid()const;
	const char* GetName()const;
	void SetName(const char* str);

	//--------------------------

	GameObject* CreateChild();
	Component* CreateComponent(COMPONENT_TYPE type);

	Component* GetComponent(COMPONENT_TYPE type);
	std::vector<Component*> GetComponents(COMPONENT_TYPE types);
	bool HasComponent(COMPONENT_TYPE type);
	uint CountComponents(COMPONENT_TYPE type);

	void RemoveChild(GameObject* obj);
	void RemoveAllChilds();

	//--------------------------

	GameObject* GetParent()const;
	void SetNewParent(GameObject* parent, bool force = false);

	//--------------------------

	void DrawDebug();

	//--------------------------

	void Destroy();
	void RemoveComponent(Component* cmp);

	//--------------------------

	bool IsStatic()const;
	void SetStatic(bool set);

	//--------------------------

	bool IsActive()const;
	void SetActive(bool set);
	void SwitchActive();
	void Enable();
	void Disable();

	//--------------------------

	void PreUpdate();
	void Update(float dt);

	//--------------------------

	void OnStart();
	void OnFinish();
	void OnEnable();
	void OnDisable();

	void OnPlay();
	void OnStop();
	void OnPause();
	void OnUnPause();

	void OnGameObjectDestroyed();

	//--------------------------

	void RecCalcTransform(const float4x4& parentTrans, bool force = false);
	void RecCalcBoxes();
	void RecalcBox();

	//--------------------------

	bool OnSaveGo(JsonFile& sect, std::map<uint, uint>* duplicate = nullptr)const;
	bool OnLoadGo(JsonFile* sect, std::map<GameObject*, uint>& relations);

	//--------------------------

	const char* GetTag()const;
	void SetTag(const char* _tag);

	Layer GetLayer()const;
	void SetLayer(Layer _layer);

	//--------------------------



private:

public:
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	AABB enclosingBox;

	Transform* transform = nullptr;

private:
	std::string name;
	UID uuid = 0;
	GameObject* parent = nullptr;
	bool selfActive = true;
	bool isStatic = false;


	bool wasDirty = true;

	std::vector<Component*> componentsToRemove;

	int currentCMPs = 0;

	std::string tag = "untagged";
	Layer layer;

};

#endif // !__GAME_OBJECT_H__