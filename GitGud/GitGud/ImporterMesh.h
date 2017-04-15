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

	bool ImportMesh(const aiMesh* mesh, std::string& output, UID& id);

	bool LoadResource(Resource* resource)override;
	void GenBuffers(const ResourceMesh* res);

	UID SaveResource(ResourceMesh* res, std::string& outputName);


	bool LoadCube(ResourceMesh* res);
};

#endif // !__IMPORTER_MESH_H__