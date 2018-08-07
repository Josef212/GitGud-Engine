#ifndef __COMPONENT_RESOURCE_H__
#define __COMPONENT_RESOURCE_H__

#include "Globals.h"

class Resource;
class JsonFile;
enum ComponentType;

class ComponentResource
{
public:
	virtual ~ComponentResource();

	bool SetResource(UID resUID);
	
	Resource* GetResource()const;
	const UID GetResourceUID()const;

protected:
	virtual void OnResourceChanged() {}
	virtual ComponentType GetComponentType() = 0;

	void OnSaveRes(JsonFile& file)const;
	void OnLoadRes(JsonFile* file);

protected:
	UID resource = 0;
};

#endif // !__COMPONENT_RESOURCE_H__