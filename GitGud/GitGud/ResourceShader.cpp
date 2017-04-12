#include "ResourceShader.h"



ResourceShader::ResourceShader(UID uuid) : Resource(uuid, RES_SHADER)
{
}


ResourceShader::~ResourceShader()
{
}

bool ResourceShader::LoadInMemory()
{
	return false;
}

bool ResourceShader::RemoveFromMemory()
{
	return false;
}
