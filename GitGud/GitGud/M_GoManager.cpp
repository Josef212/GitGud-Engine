#include "M_GoManager.h"

#include "App.h"
#include "RandGen.h"

#include "GameObject.h"
#include "Component.h"

M_GoManager::M_GoManager(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG("GoManager: Creation.");
}


M_GoManager::~M_GoManager()
{
	_LOG("GoManager: Destroying.");
}

UPDATE_RETURN M_GoManager::PreUpdate(float dt)
{
	for (auto obj : objectsToDelete)
	{
		if (obj->IsStatic())
		{
			//TODO: Remove from tree
		}
		else
		{
			std::vector<GameObject*>::iterator it = std::find(dynamicGameObjects.begin(), dynamicGameObjects.end(), obj);
			if (it != dynamicGameObjects.end())
				dynamicGameObjects.erase(it);
		}
		obj->OnFinish();
		RELEASE(obj);
		//TODO: Event on obj destroyed
	}

	objectsToDelete.clear();

	if (root)
	{
		DoPreUpdate();
	}

	return UPDT_CONTINUE;
}
