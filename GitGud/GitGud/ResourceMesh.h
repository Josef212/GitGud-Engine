#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__


#include "Resource.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh(UID uuid);
	virtual ~ResourceMesh();
};

#endif // !__RESOURCE_MESH_H__