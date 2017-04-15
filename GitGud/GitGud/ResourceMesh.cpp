#include "ResourceMesh.h"

#include "App.h"
#include "M_FileSystem.h"
#include "ImporterMesh.h"
#include "M_ResourceManager.h"

#include "OpenGL.h"


ResourceMesh::ResourceMesh(UID uuid) : Resource(uuid, RES_MESH)
{
}


ResourceMesh::~ResourceMesh()
{
}

bool ResourceMesh::LoadInMemory()
{
	return app->resources->meshImporter->LoadResource(this);
}

bool ResourceMesh::RemoveFromMemory()
{
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(uvs);
	RELEASE_ARRAY(colors);

	numIndices = 0;
	numVertices = 0;

	//TODO: Free from VRAM

	return true;
}