#ifndef __IMPORTER_SHADER_H__
#define __IMPORTER_SHADER_H__


#include "Importer.h"

class ImporterShader : public Importer
{
public:
	ImporterShader();
	virtual ~ImporterShader();

	bool LoadResource(Resource* resource)override;
};

#endif // !__IMPORTER_SHADER_H__