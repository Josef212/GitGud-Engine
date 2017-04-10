#include "M_GoManager.h"

#include "App.h"
#include "RandGen.h"

#include "GameObject.h"
#include "Component.h"

M_GoManager::M_GoManager(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG("GoManager: Creation.");

	root = new GameObject(nullptr, 0);
	root->SetName("SceneRoot");
}


M_GoManager::~M_GoManager()
{
	_LOG("GoManager: Destroying.");
	RELEASE(root);
}

bool M_GoManager::Init(JsonFile * conifg)
{
	_LOG("GoManager: Init.");
	//TODO: Create tree
	return true;
}

bool M_GoManager::Start()
{
	_LOG("GoManager: Start.");
	return true;
}

UPDATE_RETURN M_GoManager::PreUpdate(float dt)
{
	if (!objectsToDelete.empty())
	{
		selected = nullptr;
		for (auto obj : objectsToDelete)
		{
			obj->OnFinish();
			RELEASE(obj);
			//TODO: Event on obj destroyed
		}

		objectsToDelete.clear();
	}

	if (root)
	{
		//TODO
		//root->RecCalcTransform(root->GetTransform()->GetLocalTransform());
		root->RecCalcBoxes();

		for (auto obj : root->childs)
		{
			DoPreUpdate(obj);
		}
	}

	return UPDT_CONTINUE;
}

UPDATE_RETURN M_GoManager::Update(float dt)
{
	if (root)
	{
		for (auto go : root->childs)
		{
			if (go->IsActive())
				DoUpdate(go, dt);
		}
	}

	return UPDT_CONTINUE;
}

bool M_GoManager::CleanUp()
{
	_LOG("GoManager: CleanUp.");
	return true;
}

GameObject * M_GoManager::GetRoot() const
{
	return root;
}

GameObject * M_GoManager::GetGOFromUid(UID uuid) const
{
	//TODO
	return nullptr;
}

GameObject * M_GoManager::GetSelected() const
{
	return selected;
}

void M_GoManager::SelectGo(GameObject * go)
{
	selected = go;
}

GameObject * M_GoManager::CreateGameObject(GameObject * parent)
{
	GameObject* ret = nullptr;

	if (parent)
		ret = parent->CreateChild();
	else
		ret = root->CreateChild();

	return ret;
}

void M_GoManager::InsertToTree(GameObject * object)
{
	//TODO
}

void M_GoManager::EraseFromTree(GameObject * object)
{
	//TODO
}

void M_GoManager::AddDynObject(GameObject * obj)
{
	dynamicGameObjects.push_back(obj);
}

void M_GoManager::EraseDynObj(GameObject * obj)
{
	std::list<GameObject*>::iterator it = std::find(dynamicGameObjects.begin(), dynamicGameObjects.end(), obj);
	if (it != dynamicGameObjects.end()) dynamicGameObjects.erase(it);
}

void M_GoManager::RemoveGameObject(GameObject * obj)
{
	if (obj)
	{
		if (obj->GetParent())
			obj->GetParent()->RemoveChild(obj);
		obj->RemoveAllChilds();

		objectsToDelete.push_back(obj);
	}
}

void M_GoManager::FastRemoveGameObject(GameObject * obj)
{
	if (obj)
	{
		obj->RemoveAllChilds();
		objectsToDelete.push_back(obj);
	}
}

void M_GoManager::OnPlay()
{
	for (auto obj : root->childs)
	{
		DoOnPlay(obj);
	}
}

void M_GoManager::DoOnPlay(GameObject * obj)
{
	if (obj && obj->IsActive())
	{
		obj->OnPlay();

		for (auto go : obj->childs)
		{
			DoOnPlay(go);
		}
	}
}

void M_GoManager::OnStop()
{
	for (auto obj : root->childs)
	{
		DoOnPlay(obj);
	}
}

void M_GoManager::DoOnStop(GameObject * obj)
{
	if (obj && obj->IsActive())
	{
		obj->OnStop();

		for (auto go : obj->childs)
		{
			DoOnStop(go);
		}
	}
}

void M_GoManager::OnPause()
{
	for (auto obj : root->childs)
	{
		DoOnPause(obj);
	}
}

void M_GoManager::DoOnPause(GameObject * obj)
{
	if (obj && obj->IsActive())
	{
		obj->OnPause();

		for (auto go : obj->childs)
		{
			DoOnPause(go);
		}
	}
}

void M_GoManager::OnUnPause()
{
	for (auto obj : root->childs)
	{
		DoOnUnPause(obj);
	}
}

void M_GoManager::DoOnUnPause(GameObject * obj)
{
	if (obj && obj->IsActive())
	{
		obj->OnUnPause();

		for (auto go : obj->childs)
		{
			DoOnUnPause(go);
		}
	}
}

void M_GoManager::DoPreUpdate(GameObject * obj)
{
	if (obj)
	{
		obj->PreUpdate();

		for (auto go : obj->childs)
		{
			DoPreUpdate(go);
		}
	}
}

void M_GoManager::DoUpdate(GameObject * obj, float dt)
{
	if (obj && obj->IsActive())
	{
		obj->Update(dt);

		for (auto go : obj->childs)
		{
			DoUpdate(go, dt);
		}
	}
}
