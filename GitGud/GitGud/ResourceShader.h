#ifndef __RESOURCE_SHADER_H__
#define __RESOURCE_SHADER_H__


#include "Resource.h"

class ResourceShader : public Resource
{
public:
	ResourceShader(UID uuid);
	virtual ~ResourceShader();

	bool LoadInMemory()override;
	bool RemoveFromMemory()override;
};

#endif // !__RESOURCE_SHADER_H__