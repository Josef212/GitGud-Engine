#include "ImporterMaterial.h"

#include "App.h"
#include "M_FileSystem.h"
#include "M_ResourceManager.h"
#include "JsonFile.h"
#include "ResourceMaterial.h"
#include <material.h>


ImporterMaterial::ImporterMaterial()
{
	_LOG(LOG_INFO, "Material importer: Created.");
}


ImporterMaterial::~ImporterMaterial()
{
}

bool ImporterMaterial::ImportMaterial(const aiMaterial * material, Path& exported, UID & resUID, Path* sourcePath)
{
	bool ret = false;

	if (!material) return ret;

	//ResourceMaterial r(0);
	//
	////Name
	//aiString name;
	//material->Get(AI_MATKEY_NAME, name);
	//r.name = (name.length > 0) ? name.C_Str() : "unamed_material";
	//_LOG("Material name: %s.", name.C_Str());
	//
	

	//Material importing should be done according the default shader and material of the engine.

	ret = true;

	return ret;
}

bool ImporterMaterial::LoadResource(Resource * resource)
{
	return false;
}

bool ImporterMaterial::SaveResource(ResourceMaterial* reosurce)
{
	return false;
}
