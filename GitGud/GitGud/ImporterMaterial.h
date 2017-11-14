#ifndef __IMPORTER_MATERIAL_H__
#define __IMPORTER_MATERIAL_H__

#include "Importer.h"
#include <string>

struct aiMaterial;
class ResourceMaterial;

class ImporterMaterial : public Importer
{
public:
	ImporterMaterial();
	virtual ~ImporterMaterial();

	bool ImportMaterial(const aiMaterial* material, Path& exported, UID& resUID, Path* sourcePath);

	bool LoadResource(Resource* resource)override;
	bool SaveResource(ResourceMaterial* reosurce);
};

#endif // !__IMPORTER_MATERIAL_H__