#include "M_ResourceManager.h"

#include "App.h"

#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceMaterial.h"
#include "ResourceScene.h"
#include "ResourceShader.h"

#include "ImporterMesh.h"
#include "ImporterTexture.h"
#include "ImporterMaterial.h"
#include "ImporterScene.h"
#include "ImporterShader.h"


M_ResourceManager::M_ResourceManager(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG("Resource manager: Creation.");

	meshImporter = new ImporterMesh();
	textureImporter = new ImporterTexture();
	materialImporter = new ImporterMaterial();
	sceneImporter = new ImporterScene();
	shaderImporter = new ImporterShader();
}


M_ResourceManager::~M_ResourceManager()
{
	_LOG("Resource manager: Destroying.");
}
