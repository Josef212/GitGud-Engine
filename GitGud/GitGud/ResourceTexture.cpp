#include "ResourceTexture.h"

#include "App.h"
#include "M_ResourceManager.h"
#include "ImporterTexture.h"

ResourceTexture::ResourceTexture(UID uuid) : Resource(uuid, RES_TEXTURE)
{
}


ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadInMemory()
{
	return app->resources->textureImporter->LoadResource(this);
}

bool ResourceTexture::RemoveFromMemory()
{
	return false;
}
