#ifndef __IMPORTER_MATERIAL_H__
#define __IMPORTER_MATERIAL_H__


#include "Importer.h"

class ImporterMaterial : public Importer
{
public:
	ImporterMaterial();
	virtual ~ImporterMaterial();

	bool LoadResource(Resource* resource)override;
};

#endif // !__IMPORTER_MATERIAL_H__