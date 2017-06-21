#include "M_GoManager.h"

#include "App.h"
#include "RandGen.h"
#include "JsonFile.h"

#include "M_FileSystem.h"

#include "GGOctree.h"

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"

#define OCTREE_SIZE 100 / 2

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

	octreeSize = conifg->GetInt("octree_size", OCTREE_SIZE);
	
	octree = new GGOctree();
	octree->Create(AABB::FromCenterAndSize(float3(0, 0, 0), float3(octreeSize, octreeSize, octreeSize)));

	return true;
}

bool M_GoManager::Start()
{
	_LOG("GoManager: Start.");

	GameObject* go = CreateGameObject();
	Mesh* m = (Mesh*)go->CreateComponent(CMP_MESH);
	m->SetResource(2);

	return true;
}

UPDATE_RETURN M_GoManager::PreUpdate(float dt)
{
	if (!objectsToDelete.empty())
	{
		for (auto obj : objectsToDelete)
		{
			if (obj == selected)
				selected = nullptr;

			obj->OnFinish();
			RELEASE(obj);
			//TODO: Event on obj destroyed
		}

		objectsToDelete.clear();
	}

	if (root)
	{
		if (anyGOTransHasChanged)
		{
			root->RecCalcTransform(root->transform->GetLocalTransform());
			root->RecCalcBoxes();
			anyGOTransHasChanged = false;
		}

		if (mustSave)
		{
			SaveSceneNow();
			mustSave = false;
		}

		if (mustLoad)
		{
			LoadSceneNow();
			mustLoad = false;
		}

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
	RELEASE(octree);
	return true;
}

GameObject * M_GoManager::GetRoot() const
{
	return root;
}

GameObject * M_GoManager::GetGOFromUid(UID uuid) const
{
	return GetGoFromUID(root, uuid);
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
	octree->Insert(object);
}

void M_GoManager::EraseFromTree(GameObject * object)
{
	octree->Erase(object);
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

void M_GoManager::GetToDrawStaticObjects(std::vector<GameObject*>& objects, Camera * cam)
{
	if(cam)
		octree->CollectCandidates(objects, cam->frustum);
}

std::list<GameObject*>* M_GoManager::GetDynamicObjects()
{
	return &dynamicGameObjects;
}

void M_GoManager::SaveScene()
{
	mustSave = true;
}

void M_GoManager::LoadScene()
{
	mustLoad = true;
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

GameObject * M_GoManager::GetGoFromUID(GameObject * obj, UID uuid) const
{
	if (!obj || uuid == 0)
		return nullptr;

	if (obj->GetUuid() == uuid)
		return obj;

	GameObject* ret = nullptr;

	for (std::vector<GameObject*>::iterator it = obj->childs.begin(); it != obj->childs.end() && ret == nullptr; ++it)
	{
		ret = GetGoFromUID((*it), uuid);
	}

	return ret;
}

void M_GoManager::SaveSceneNow()
{
	bool ret = false;

	//TODO: Resource scene organitzation!!

	JsonFile scene;
	scene.AddArray("game_objects");

	for (auto it : root->childs)
	{
		if (it)
			ret = it->OnSaveGo(scene);
	}

	if (ret)
	{
		char* buffer = nullptr;
		uint size = scene.WriteJson(&buffer, false); // TODO: fast
		if (buffer && size > 0)
		{
			std::string path(SCENE_SAVE_PATH);
			path.append("test_scene.json");

			if (app->fs->Save(path.c_str(), buffer, size) != size)
			{
				_LOG("ERROR while saving scene.");
			}
			else
			{
				ret = true;
				_LOG("Just saved scene into [%s].", path.c_str());
			}
		}

		RELEASE_ARRAY(buffer);
	}

}

void M_GoManager::LoadSceneNow()
{
	bool ret = false;

	//TODO: Resource scene organitzation!!

	std::string path(SCENE_SAVE_PATH);
	path.append("test_scene.json");

	char* buffer = nullptr;
	uint size = app->fs->Load(path.c_str(), &buffer);

	if (buffer && size > 0)
	{
		JsonFile scene(buffer);

		int goCount = scene.GetArraySize("game_objects");
		std::map<GameObject*, uint> relations;
		for (int i = 0; i < goCount; ++i)
		{
			GameObject* go = CreateGameObject();
			go->OnLoadGo(&scene.GetArray("game_objects", i), relations);
		}

		for (auto it : relations)
		{
			UID parentID = it.second;
			GameObject* go = it.first;

			if (parentID == 0)
			{
				go->SetNewParent(root);
			}
			else
			{
				GameObject* dad = GetGOFromUid(parentID);
				if (dad) go->SetNewParent(dad);
			}
		}

		root->RecCalcTransform(root->transform->GetLocalTransform(), true);
		root->RecalcBox();

		for (auto it : relations)
			if (it.first) it.first->OnStart();
	}

	RELEASE_ARRAY(buffer);

	_LOG("Scene loaded [%s].", path.c_str());
}
