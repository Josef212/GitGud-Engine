#ifndef __COMPONENT_RESOURCE_H__
#define __COMPONENT_RESOURCE_H__

#include "Globals.h"

class Resource;
class JsonFile;

class ComponentResource
{
public:
	virtual ~ComponentResource();

	virtual bool SetResource(UID res) = 0;
	
	const Resource* GetResource()const;
	const UID GetResourceUID()const;

protected:
	void OnSaveRes(JsonFile& file)const;
	void OnLoadRes(JsonFile* file);

protected:
	Resource* resource = nullptr;
};

#endif // !__COMPONENT_RESOURCE_H__