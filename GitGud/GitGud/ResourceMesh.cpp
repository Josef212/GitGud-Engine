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

	if (idIndices > 0) { glDeleteBuffers(1, &idIndices); idIndices = 0; }
	if (idVertices > 0) { glDeleteBuffers(1, &idVertices); idVertices = 0; }
	if (idNormals > 0) { glDeleteBuffers(1, &idNormals); idNormals = 0; }
	if (idUvs > 0) { glDeleteBuffers(1, &idUvs); idUvs = 0; }
	if (idColors > 0) { glDeleteBuffers(1, &idColors); idColors = 0; }

	if (idContainer > 0) { glDeleteVertexArrays(1, &idContainer); idContainer = 0; }

	return true;
}

void ResourceMesh::LoadToVRAM()
{
	app->resources->meshImporter->GenBuffers(this);
}
