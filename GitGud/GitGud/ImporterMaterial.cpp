#include "ImporterMaterial.h"

#include "App.h"
#include "M_FileSystem.h"
#include "M_ResourceManager.h"
#include "JsonFile.h"
#include "ResourceMaterial.h"
#include <material.h>


ImporterMaterial::ImporterMaterial()
{
	_LOG("Material importer: Created.");
}


ImporterMaterial::~ImporterMaterial()
{
}

bool ImporterMaterial::Import(const aiMaterial * material, Path& exported, UID & resUID, Path* sourcePath)
{
	bool ret = false;

	if (!material) return ret;

	ResourceMaterial r(0);

	//Name
	aiString name;
	material->Get(AI_MATKEY_NAME, name);
	r.name = (name.length > 0) ? name.C_Str() : "unamed_material";
	_LOG("Material name: %s.", name.C_Str());

	//Ambient color
	aiColor4D ambientColor;
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
	r.ambientColor.Set(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);

	//Ambient texture
	if (material->GetTextureCount(aiTextureType_AMBIENT) > 0)
	{
		aiString str;
		material->GetTexture(aiTextureType_AMBIENT, 0, &str);
		//TODO: Load resource
		_LOG("Ambient tex: %s.", str.C_Str());
	}

	//Diffuse color
	aiColor4D diffuseColor;
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	r.diffuseColor.Set(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);

	//Diffuse texture
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString str;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		//TODO: Load resource
		_LOG("Diffuse tex: %s.", str.C_Str());
	}

	//Specular color
	aiColor4D specularColor;
	material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	r.specularColor.Set(specularColor.r, specularColor.g, specularColor.b, specularColor.a);

	//Specular texture
	if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
	{
		aiString str;
		material->GetTexture(aiTextureType_SPECULAR, 0, &str);
		//TODO: Load resource
		_LOG("Specular tex: %s.", str.C_Str());
	}
	
	//Normal map
	if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
	{
		aiString str;
		material->GetTexture(aiTextureType_NORMALS, 0, &str);
		//TODO: Load resource
		_LOG("Normal map tex: %s.", str.C_Str());
	}

	//Height map
	if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
	{
		aiString str;
		material->GetTexture(aiTextureType_HEIGHT, 0, &str);
		//TODO: Load resource
		_LOG("Height tex: %s.", str.C_Str());
	}

	//Emission color
	aiColor4D emissionColor;
	material->Get(AI_MATKEY_COLOR_EMISSIVE, emissionColor);
	r.emissionColor.Set(emissionColor.r, emissionColor.g, emissionColor.b, emissionColor.a);

	//Emission texture
	if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
	{
		aiString str;
		material->GetTexture(aiTextureType_EMISSIVE, 0, &str);
		//TODO: Load resource
		_LOG("Emission tex: %s.", str.C_Str());
	}


	//TODO: Metallic?? Is calculated with same as specular??

	//TMP
	
	for (uint i = 0; i < material->mNumProperties; ++i)
	{
		const aiMaterialProperty* prop = material->mProperties[i];
		_LOG("Material property name: %s.", prop->mKey.C_Str());
	}

	//----

	//TODO: Save the resource

	JsonFile save;

	

	return ret;
}

bool ImporterMaterial::LoadResource(Resource * resource)
{
	return false;
}
