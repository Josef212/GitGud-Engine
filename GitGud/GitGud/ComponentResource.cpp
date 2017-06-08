#include "ComponentResource.h"
#include "Resource.h"

#include "App.h"
#include "M_ResourceManager.h"

ComponentResource::~ComponentResource()
{
}

const Resource * ComponentResource::GetResource() const
{
	return app->resources->GetResourceFromUID(resource);
}

const UID ComponentResource::GetResourceUID() const
{
	return resource;
}

void ComponentResource::OnSaveRes(JsonFile & file) const
{
	file.AddInt("resource_id", resource);
}

void ComponentResource::OnLoadRes(JsonFile * file)
{
	SetResource(file->GetInt("resource_id", 0));
}
