#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Resource.h"
#include "Math.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh(UID uuid);
	virtual ~ResourceMesh();

	bool LoadInMemory()override;
	bool RemoveFromMemory()override;

	void LoadToVRAM();

public:
	uint numIndices = 0;
	uint* indices = nullptr;

	uint numVertices = 0;

	//----------------------

	float* vertices = nullptr;
	float* normals = nullptr;
	float* uvs = nullptr;
	float* colors = nullptr;

	//----------------------

	uint idIndices = 0;
	uint idVertices = 0;
	uint idNormals = 0;
	uint idUvs = 0;
	uint idColors = 0;

	uint idContainer = 0;

	//----------------------

	AABB aabb;

};

#endif // !__RESOURCE_MESH_H__