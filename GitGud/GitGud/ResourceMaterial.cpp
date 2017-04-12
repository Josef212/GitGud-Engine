#include "ResourceMaterial.h"



ResourceMaterial::ResourceMaterial(UID uuid) : Resource(uuid, RES_MATERIAL)
{
}


ResourceMaterial::~ResourceMaterial()
{
}

bool ResourceMaterial::LoadInMemory()
{
	return false;
}

bool ResourceMaterial::RemoveFromMemory()
{
	return false;
}
