#include "ResourceScene.h"

#include "App.h"
#include "M_ResourceManager.h"
#include "ImporterScene.h"

ResourceScene::ResourceScene(UID uuid) : Resource(uuid, RES_SCENE)
{
}


ResourceScene::~ResourceScene()
{
	RemoveFromMemory();
}

bool ResourceScene::LoadInMemory()
{
	return app->resources->sceneImporter->LoadResource(this);
}

bool ResourceScene::RemoveFromMemory()
{
	return false;
}
