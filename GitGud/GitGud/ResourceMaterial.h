#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__


#include "Resource.h"

class ResourceMaterial : public Resource
{
public:
	ResourceMaterial(UID uuid);
	virtual ~ResourceMaterial();
};

#endif // !__RESOURCE_MATERIAL_H__