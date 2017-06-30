#ifndef __IMPORTER_SHADER_H__
#define __IMPORTER_SHADER_H__

#include "Importer.h"

class ResourceShader;

class ImporterShader : public Importer
{
public:
	ImporterShader();
	virtual ~ImporterShader();

	bool LoadResource(Resource* resource)override;

	bool PrepareDefaultShader(ResourceShader* sh);
};

#endif // !__IMPORTER_SHADER_H__