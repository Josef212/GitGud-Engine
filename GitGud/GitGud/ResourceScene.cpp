#include "ResourceScene.h"



ResourceScene::ResourceScene(UID uuid) : Resource(uuid, RES_SCENE)
{
}


ResourceScene::~ResourceScene()
{
}

bool ResourceScene::LoadInMemory()
{
	return false;
}

bool ResourceScene::RemoveFromMemory()
{
	return false;
}
