#ifndef __M_RESOURCEMANAGER_H__
#define __M_RESOURCEMANAGER_H__

#include "Module.h"

class Resource;
class ResourceMesh;
class ResourceTexture;
class ResourceMaterial;
class ResourceScene;
class ResourceShader;

class ImporterMesh;
class ImporterTexture;
class ImporterMaterial;
class ImporterScene;
class ImporterShader;

class M_ResourceManager : public Module
{
public:
	M_ResourceManager(const char* name, bool startEnabled = true);
	virtual ~M_ResourceManager();

private:

public:
	ImporterMesh*		meshImporter = nullptr;
	ImporterTexture*	textureImporter = nullptr;
	ImporterMaterial*	materialImporter = nullptr;
	ImporterScene*		sceneImporter = nullptr;
	ImporterShader*		shaderImporter = nullptr;

private:


};

#endif // !__M_RESOURCEMANAGER_H__