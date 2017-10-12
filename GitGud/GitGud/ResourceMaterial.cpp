#include "ResourceMaterial.h"
#include "App.h"
#include "M_ResourceManager.h"
#include "ImporterMaterial.h"

#include "ResourceShader.h"

#include "OpenGL.h"


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
	return false;
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

void ResourceMaterial::SendMaterialToShader(float* model, float* camView, float* camProj)
{
	ResourceShader* sh = (ResourceShader*)app->resources->GetResourceFromUID(shader);
	if (sh)
	{
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, camView);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, camProj);

		uint shaderGLID = sh->GetShaderID();

	}
}

//--------------------------------------------------------------------------------------------

