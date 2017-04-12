#include "ResourceTexture.h"



ResourceTexture::ResourceTexture(UID uuid) : Resource(uuid, RES_TEXTURE)
{
}


ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadInMemory()
{
	return false;
}

bool ResourceTexture::RemoveFromMemory()
{
	return false;
}
