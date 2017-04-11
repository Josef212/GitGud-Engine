#include "ComponentResource.h"
#include "Resource.h"


ComponentResource::~ComponentResource()
{
}

const Resource * ComponentResource::GetResource() const
{
	return resource;
}

const UID ComponentResource::GetResourceUID() const
{
	return resource != nullptr ? resource->GetUID() : 0;
}

void ComponentResource::OnSaveRes(JsonFile & file) const
{
	file.AddInt("resource_id", GetResourceUID());
}

void ComponentResource::OnLoadRes(JsonFile * file)
{
	SetResource(file->GetInt("resource_id", 0));
}
