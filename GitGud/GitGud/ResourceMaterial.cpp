#include "ResourceMaterial.h"
#include "App.h"
#include "M_ResourceManager.h"
#include "ImporterMaterial.h"


ResourceMaterial::ResourceMaterial(UID uuid) : Resource(uuid, RES_MATERIAL)
{
}


ResourceMaterial::~ResourceMaterial()
{
}

bool ResourceMaterial::LoadInMemory()
{
	return app->resources->materialImporter->LoadResource(this);
}

bool ResourceMaterial::RemoveFromMemory()
{
	return false;
}
