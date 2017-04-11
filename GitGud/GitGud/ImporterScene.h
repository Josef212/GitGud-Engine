#ifndef __IMPORTER_SCENE_H__
#define __IMPORTER_SCENE_H__


#include "Importer.h"

class ImporterScene : public Importer
{
public:
	ImporterScene();
	virtual ~ImporterScene();

	bool LoadResource(Resource* resource)override;
};

#endif // !__IMPORTER_SCENE_H__