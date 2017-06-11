#ifndef __M_RESOURCEMANAGER_H__
#define __M_RESOURCEMANAGER_H__

#include "Module.h"

#include <map>
#include <vector>
#include <string>

enum RESOURCE_TYPE;

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
	UPDATE_RETURN PreUpdate(float dt)override;
	bool CleanUp()override;

	UID ImportFile(const char* fileName, bool checkFirst = false);
	UID ImportBuf(const void* buffer, uint size, RESOURCE_TYPE type, Path* sourceFile = nullptr);

	Resource* GetResourceFromUID(UID uuid);
	Resource* CreateResource(RESOURCE_TYPE type, UID forceUID = 0);

	Resource* FindResourceFromOriginalFileName(const char* name);

	RESOURCE_TYPE GetTypeFromExtension(const char* ext)const;

	UID GetNewUID()const;

	void GetResourcesOfType(std::vector<Resource*>& res, RESOURCE_TYPE type)const;

private:
	void LoadResources();
	void SaveResources();
	void LoadBasicResources();

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