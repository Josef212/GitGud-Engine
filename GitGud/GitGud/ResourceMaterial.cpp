#include "ResourceMaterial.h"
#include "App.h"
#include "M_ResourceManager.h"
#include "ImporterMaterial.h"

#include "ResourceShader.h"

#include "OpenGL.h"


MaterialProperty::MaterialProperty(const char* name, int prop) : propertyType(MP_INT), propertyName(name)
{
	property.iProp = prop;
}

MaterialProperty::MaterialProperty(const char* name, float prop) : propertyType(MP_FLOAT), propertyName(name)
{
	property.fProp = prop;
}

MaterialProperty::MaterialProperty(const char* name, uint prop) : propertyType(MP_TEXTURE_RES), propertyName(name)
{
	property.textureResId = prop;
}

MaterialProperty::MaterialProperty(const char* name, float* prop, MAT_PROPERY_TYPE type) : propertyType(type),
                                                                                           propertyName(name)
{
	property.fPointer = prop;
}

void MaterialProperty::SendInfoToShader(uint shader)
{
	int location = glGetUniformLocation(shader, propertyName);
	if (location >= 0)
	{
		switch (propertyType)
		{
		case MP_INT:
			glUniform1i(location, property.iProp);
			break;
		case MP_FLOAT:
			glUniform1f(location, property.fProp);
			break;
		case MP_TEXTURE_RES:
			//TODO: Must activate textures and count textures etc.
			glUniform1i(location, property.textureResId);
			break;
		case MP_VEC2:
			glUniform2fv(location, 1, property.fPointer);
			break;
		case MP_VEC3:
			glUniform3fv(location, 1, property.fPointer);
			break;
		case MP_VEC4:
			glUniform4fv(location, 1, property.fPointer);
			break;
		case MP_MAT3:
			glUniformMatrix3fv(location, 1, GL_FALSE, property.fPointer);
			break;
		case MP_MAT4:
			glUniformMatrix4fv(location, 1, GL_FALSE, property.fPointer);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

ResourceMaterial::ResourceMaterial(UID uuid) : Resource(uuid, RES_MATERIAL)
{
}


ResourceMaterial::~ResourceMaterial()
{
	RemoveFromMemory();
}

bool ResourceMaterial::LoadInMemory()
{
	return app->resources->materialImporter->LoadResource(this);
}

bool ResourceMaterial::RemoveFromMemory()
{
	for (auto it : properties)
	{
		RELEASE(it.second);
	}
	properties.clear();

	return true;
}

bool ResourceMaterial::SetShader(uint shaderResource)
{
	bool ret = false;

	if (shaderResource != 0 && app->resources->GetResourceFromUID(shader) != nullptr)
	{
		shader = shaderResource;
		GetLocations();
		SetPropertiesFromShaderFile();
	}

	return ret;
}

void ResourceMaterial::GetLocations()
{
	ResourceShader* sh = (ResourceShader*)app->resources->GetResourceFromUID(shader);
	if (sh)
	{
		uint shaderGLID = sh->GetShaderID();

		modelLoc = glGetUniformLocation(shaderGLID, "model");
		viewLoc = glGetUniformLocation(shaderGLID, "view");
		projLoc = glGetUniformLocation(shaderGLID, "projection");
	}
}

void ResourceMaterial::SetPropertiesFromShaderFile()
{
	//TODO: Scan the shader to set the material properties.
}

void ResourceMaterial::SendMaterialToShader(float* model, float* camView, float* camProj)
{
	ResourceShader* sh = (ResourceShader*)app->resources->GetResourceFromUID(shader);
	if (sh)
	{
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camView);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, camProj);

		uint shaderGLID = sh->GetShaderID();

		for (std::map<std::string, MaterialProperty*>::iterator it = properties.begin(); it != properties.end(); ++it)
		{
			it->second->SendInfoToShader(shaderGLID); //TODO: Take into account textures.
		}
	}
}

bool ResourceMaterial::SaveMaterial()
{
	return app->resources->materialImporter->SaveResource(this);
}

//--------------------------------------------------------------------------------------------
