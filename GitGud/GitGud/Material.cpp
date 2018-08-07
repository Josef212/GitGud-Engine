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

void Material::OnSaveCmp(JsonFile & sect) const
{
}

void Material::OnLoadCmp(JsonFile * sect)
{
}
