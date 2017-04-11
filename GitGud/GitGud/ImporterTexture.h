#ifndef __IMPORTER_TEXTURE_H__
#define __IMPORTER_TEXTURE_H__


#include "Importer.h"

class ImporterTexture : public Importer
{
public:
	ImporterTexture();
	virtual ~ImporterTexture();

	bool LoadResource(Resource* resource)override;
};

#endif // !__IMPORTER_TEXTURE_H__