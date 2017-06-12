#ifndef __IMPORTER_TEXTURE_H__
#define __IMPORTER_TEXTURE_H__

#include "Importer.h"
#include <string>

class ResourceTexture;

class ImporterTexture : public Importer
{
public:
	ImporterTexture();
	virtual ~ImporterTexture();

	bool Import(Path originalFile, Path& exportedFile, UID& resUID);
	bool ImportBuff(const void* buffer, uint size, Path& exportedFile, UID& resUID);
	
	bool LoadResource(Resource* resource)override;

	bool LoadChequers(ResourceTexture* res);
};

#endif // !__IMPORTER_TEXTURE_H__