#ifndef __M_RESOURCEMANAGER_H__
#define __M_RESOURCEMANAGER_H__

#include "Module.h"

#include <map>
#include <vector>
#include <string>

enum ResourceType;

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

	bool Init(JsonFile* conf)override;
	bool Start()override;
	UpdateReturn PreUpdate(float dt)override;
	bool CleanUp()override;

	UID ImportFile(const char* fileName, bool checkFirst = false);
	UID ImportBuf(const void* buffer, ResourceType type, uint size = 0, Path* sourceFile = nullptr);

	Resource* GetResourceFromUID(UID uuid);
	Resource* CreateResource(ResourceType type, UID forceUID = 0);

	Resource* FindResourceFromOriginalFullPath(const char* fullpath);

	ResourceType GetTypeFromExtension(const char* ext)const;

	UID GetNewUID()const;

	void GetResourcesOfType(std::vector<Resource*>& res, ResourceType type)const;

private:
	void LoadResources();
	void SaveResources();
	bool LoadBasicResources();

public:
	ImporterMesh*		meshImporter = nullptr;
	ImporterTexture*	textureImporter = nullptr;
	ImporterMaterial*	materialImporter = nullptr;
	ImporterScene*		sceneImporter = nullptr;
	ImporterShader*		shaderImporter = nullptr;

	//---------

	ResourceMesh* cube = nullptr;
	ResourceMesh* quad = nullptr;
	ResourceMesh* plane = nullptr;
	ResourceMesh* sphere = nullptr;
	ResourceMesh* cone = nullptr;

	ResourceTexture* checkers = nullptr;
	ResourceTexture* lenna = nullptr;

	ResourceMaterial* defaultMaterial = nullptr;

	ResourceShader* defaultShader = nullptr;
	ResourceShader* wireframeDebugShader = nullptr;
	ResourceShader* normalsDebugShader = nullptr;

private:
	std::string resourceFile;
	std::map<UID, Resource*> resources;


};

#endif // !__M_RESOURCEMANAGER_H__