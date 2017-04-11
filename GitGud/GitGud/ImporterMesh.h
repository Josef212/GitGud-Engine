#ifndef __IMPORTER_MESH_H__
#define __IMPORTER_MESH_H__


#include "Importer.h"

class ImporterMesh : public Importer
{
public:
	ImporterMesh();
	virtual ~ImporterMesh();

	bool LoadResource(Resource* resource)override;
};

#endif // !__IMPORTER_MESH_H__