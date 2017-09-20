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

/** LoadInMemory: Overloaded method. Actually loads the mesh resource using the mesh importer.
					It uses the mesh exported file to load the data. */
bool ResourceMesh::LoadInMemory()
{
	return app->resources->meshImporter->LoadResource(this);
}

/** RemoveFromMemory: Overloaded method. Actually frees the mesh resource from memory including VRAM. */
bool ResourceMesh::RemoveFromMemory()
{
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(uvs);
	RELEASE_ARRAY(colors);

	numIndices = 0;
	numVertices = 0;

	FreeFromVRAM();

	return true;
}

/** LoadToVRAM: Loads the mesh resource to VRAM. It check if has already been loaded, if it is the resource is freed and loaded again to ensure the resource is properly loaded. */
void ResourceMesh::LoadToVRAM()
{
	FreeFromVRAM();
	app->resources->meshImporter->GenBuffers(this);
}

/** FreeFromVRAM: Frees the resource from VRAM but not the full resource data. */
void ResourceMesh::FreeFromVRAM()
{
	if (idIndices > 0) { glDeleteBuffers(1, &idIndices); idIndices = 0; }
	if (idVertices > 0) { glDeleteBuffers(1, &idVertices); idVertices = 0; }
	if (idNormals > 0) { glDeleteBuffers(1, &idNormals); idNormals = 0; }
	if (idUvs > 0) { glDeleteBuffers(1, &idUvs); idUvs = 0; }
	if (idColors > 0) { glDeleteBuffers(1, &idColors); idColors = 0; }

	if (idContainer > 0) { glDeleteVertexArrays(1, &idContainer); idContainer = 0; }
}
