#ifndef __M_GOMANAGER_H__
#define __M_GOMANAGER_H__

#include "Module.h"
#include <vector>
#include <string>
#include <map>
#include <list>

class GGOctree;
class GameObject;
class Component;

class M_GoManager : public Module
{
public:
	M_GoManager(const char* name, bool startEnabled = true);
	virtual ~M_GoManager();

	bool Init(JsonFile* conifg)override;
	bool Start()override;
	UPDATE_RETURN PreUpdate(float dt)override;
	UPDATE_RETURN Update(float dt)override;
	bool CleanUp()override;

	GameObject* GetRoot()const;
	GameObject* GetGOFromUid(UID uuid)const;

	GameObject* GetSelected()const;
	void SelectGo(GameObject* go);

	GameObject* CreateGameObject(GameObject* parent = nullptr);

	void InsertToTree(GameObject* object);
	void EraseFromTree(GameObject* object);
	void AddDynObject(GameObject* obj);
	void EraseDynObj(GameObject* obj);

	void RemoveGameObject(GameObject* obj);
	void FastRemoveGameObject(GameObject* obj);

private:
	void OnPlay();
	void DoOnPlay(GameObject* obj);
	void OnStop();
	void DoOnStop(GameObject* obj);
	void OnPause();
	void DoOnPause(GameObject* obj);
	void OnUnPause();
	void DoOnUnPause(GameObject* obj);

	void DoPreUpdate(GameObject* obj);
	void DoUpdate(GameObject* obj, float dt);


public:
	bool anyGOTransHasChanged = true;

private:
	GameObject* root = nullptr;
	GameObject* selected = nullptr;

	std::list<GameObject*> dynamicGameObjects;
	std::vector<GameObject*> objectsToDelete;

	GGOctree* octree = nullptr;

};

#endif // !__M_GOMANAGER_H__