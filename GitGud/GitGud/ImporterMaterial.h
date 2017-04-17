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

	bool Import(const aiMaterial* material, std::string& exported, UID& resUID, const char* basePath);

	bool LoadResource(Resource* resource)override;
};

#endif // !__IMPORTER_MATERIAL_H__