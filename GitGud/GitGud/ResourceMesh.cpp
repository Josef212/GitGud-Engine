#include "ResourceMesh.h"



ResourceMesh::ResourceMesh(UID uuid) : Resource(uuid, RES_MESH)
{
}


ResourceMesh::~ResourceMesh()
{
}

bool ResourceMesh::LoadInMemory()
{
	return false;
}

bool ResourceMesh::RemoveFromMemory()
{
	return false;
}
