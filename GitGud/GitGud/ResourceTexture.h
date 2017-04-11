#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__


#include "Resource.h"

class ResourceTexture :
	public Resource
{
public:
	ResourceTexture(UID uuid);
	virtual ~ResourceTexture();
};

#endif // !__RESOURCE_TEXTURE_H__