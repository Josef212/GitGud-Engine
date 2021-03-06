#include "ComponentResource.h"
#include "Resource.h"

#include "App.h"
#include "M_ResourceManager.h"

ComponentResource::~ComponentResource()
{
}

bool ComponentResource::SetResource(UID resUID)
{
	bool ret = false;

	if(resUID != 0)
	{
		auto res = app->resources->GetResourceFromUID(resUID);
		if(res && res->GetType() == GetComponentType())
		{
			if(res->LoadToMemory())
			{
				OnResourceChanged();
				resource = resUID;
				ret = true;
			}
		}
	}

	return ret;
}

/** ComponentResource - GetResource: Return the resource of the component. */
Resource * ComponentResource::GetResource() const
{
	return app->resources->GetResourceFromUID(resource);
}

/** ComponentResource - GetResourceUID: Return the resource UID. */
const UID ComponentResource::GetResourceUID() const
{
	return resource;
}

/** ComponentResource - OnSaveRes: Add the resource id into the GO save object. */
void ComponentResource::OnSaveRes(JsonFile & file) const
{
	file.AddInt("resource_id", resource);
}

/** ComponentResource - OnLoadRes: Loads the resource from the UID on the GO save object. */
void ComponentResource::OnLoadRes(JsonFile * file)
{
	SetResource(file->GetInt("resource_id", 0));
}
