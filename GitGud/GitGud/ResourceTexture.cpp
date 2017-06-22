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

const char * ResourceTexture::GetTextureTypeStr()const
{
	static const char* types[] = { 
		"none", "diffues", "specular", "ambient", "emissive", "height", "normal map", 
		"shininess", "opacity", "displacement", "light map", "reflection" };

	return types[type];
}

const char * ResourceTexture::GetTextureFormatStr() const
{
	static const char* formats[] = {
		"color index", "rgb", "rgba", "bgr", "bgra", "luminance", "unknown" };

	return formats[format];
}
