#ifndef __M_GOMANAGER_H__
#define __M_GOMANAGER_H__

#include "Module.h"
#include <vector>
#include <string>
#include <map>
#include <list>

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
	UPDATE_RETURN PostUpdate(float dt)override;
	bool CleanUp()override;

	GameObject* GetRoot()const;
	GameObject* GetGOFromUid(UID uuid)const;

	GameObject* GetSelected()const;
	void SelectGo(GameObject* go);

	GameObject* CreateGameObject(GameObject* parent = nullptr);

	void InsertToTree(GameObject* object);
	void EraseFromTree(GameObject* object);
	void AddDynObject(GameObject* obj);
	void EraseDynObj(GameObject* ogj);

	void RemoveGameObject(GameObject* obj);

private:
	void OnPlay();
	void DoOnPlay();
	void OnStop();
	void DoOnStop();
	void OnPause();
	void DoOnPause();
	void OnUnPause();
	void DoOnUnPause();

	void DoPreUpdate();
	void DoUpdate(float dt);

public:

private:
	GameObject* root = nullptr;
	GameObject* selected = nullptr;

	std::vector<GameObject*> dynamicGameObjects;
	std::vector<GameObject*> objectsToDelete;

};

#endif // !__M_GOMANAGER_H__