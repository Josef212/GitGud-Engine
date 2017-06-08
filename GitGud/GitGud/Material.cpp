#include "Material.h"

#include "App.h"
#include "M_ResourceManager.h"
#include "Resource.h"
#include "GameObject.h"


Material::Material(GameObject* object) : Component(object, CMP_MATERIAL)
{
}


Material::~Material()
{

}

bool Material::SetResource(UID resUID)
{
	bool ret = false;

	if (resUID != 0)
	{
		Resource* res = app->resources->GetResourceFromUID(resUID);
		if (res && res->GetType() == RES_MATERIAL)
		{
			if (res->LoadToMemory())
			{
				resource = resUID;
				object->RecalcBox();
				ret = true;
			}
		}
	}

	return ret;
}

void Material::OnSaveCmp(JsonFile & sect) const
{
}

void Material::OnLoadCmp(JsonFile * sect)
{
}
