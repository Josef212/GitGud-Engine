#ifndef __RESOURCE_SCENE_H__
#define __RESOURCE_SCENE_H__


#include "Resource.h"

class ResourceScene : public Resource
{
public:
	ResourceScene(UID uuid);
	virtual ~ResourceScene();

	bool LoadInMemory()override;
	bool RemoveFromMemory()override;
};

#endif // !__RESOURCE_SCENE_H__