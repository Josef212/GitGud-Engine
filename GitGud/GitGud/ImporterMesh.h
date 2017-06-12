#ifndef __IMPORTER_MESH_H__
#define __IMPORTER_MESH_H__

#include "Importer.h"
#include "Globals.h"
#include <string>

struct aiMesh;

class ResourceMesh;

class ImporterMesh : public Importer
{
public:
	ImporterMesh();
	virtual ~ImporterMesh();

	bool ImportMesh(const aiMesh* mesh, Path& output, UID& id);

	bool LoadResource(Resource* resource)override;
	void GenBuffers(const ResourceMesh* res);

	UID SaveResource(ResourceMesh* res, Path& outputPath);


	bool LoadCube(ResourceMesh* res);
	bool LoadQuad(ResourceMesh* res);
	bool LoadPlane(ResourceMesh* res);
	bool LoadCone(ResourceMesh* res);
	bool LoadCylinder(ResourceMesh* res);
	bool LoadTorus(ResourceMesh* res);
	bool LoadSphere(ResourceMesh* res);
};

#endif // !__IMPORTER_MESH_H__