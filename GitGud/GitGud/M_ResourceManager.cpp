#include "M_ResourceManager.h"

#include "App.h"
#include "RandGen.h"
#include "M_FileSystem.h"

#include "Path.h"

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

#include "JsonFile.h"

#define RESERVED_RESOURCES 20

/** M_ResourceManager: Creates all importers. */
M_ResourceManager::M_ResourceManager(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG(LOG_INFO, "Resource manager: Creation.");

	meshImporter = new ImporterMesh();
	textureImporter = new ImporterTexture();
	materialImporter = new ImporterMaterial();
	sceneImporter = new ImporterScene();
	shaderImporter = new ImporterShader();
}

/** ~M_ResourceManager: Destroy all importers. */
M_ResourceManager::~M_ResourceManager()
{
	_LOG(LOG_INFO, "Resource manager: Destroying.");

	RELEASE(meshImporter);
	RELEASE(textureImporter);
	RELEASE(materialImporter);
	RELEASE(sceneImporter);
	RELEASE(shaderImporter);
}

/** M_ResourceManager - Init: Just get the resource file name from the config file. */
bool M_ResourceManager::Init(JsonFile * conf)
{
	_LOG(LOG_INFO, "Resource manager: Init.");

	resourceFile = conf->GetString("resource_file", "resources.json");

	return true;
}

/** M_ResourceManager - Start: Create all basic resources and load the resources from the resource file. */
bool M_ResourceManager::Start()
{
	_LOG(LOG_INFO, "Resource manager: Start.");

	//ImportFile("Data/Assets/MechaT/MechaT.fbx");
	//ImportFile("Data/Assets/Brute.fbx");
	if (!LoadBasicResources())
	{
		_LOG(LOG_ERROR, "Error loading basic resources.");
		return false;
	}

	LoadResources();

	return true;
}

/** M_ResourceManager - PreUpdate: Nothing for now. */
UPDATE_RETURN M_ResourceManager::PreUpdate(float dt)
{
	return UPDT_CONTINUE;
}

/** M_ResourceManager - CleanUp: Save all the resources into the resource file and clean up the memory. */
bool M_ResourceManager::CleanUp()
{
	_LOG(LOG_INFO, "Resource manager: CleanUp.");

	SaveResources();
	//TODO: Once all resources are saved, should cleanup all the resources.

	for (auto it : resources)
	{
		RELEASE(it.second);
	}
	resources.erase(resources.begin(), resources.end());

	return true;
}

/**
*	M_ResourceManager - ImportFile: Import any file that has to be imported into the engine.
*		If the file is not located insife Assets it will DUPLICATE the file into assets root folder,
*		if the file is already in Assets, no matter if is in a directory, will import it.
*
*	- const char* fileNam: Path of the file to import with its relative path to it.
*	- bool checkFirst: Flag to search for already imported files. False by default.
*
*	- Return UID: The id resulting resource, 0 if any error.
*
*/
UID M_ResourceManager::ImportFile(const char * fileName, bool checkFirst)
{
	UID ret = 0;

	Path source(fileName);

	//TODO: Check first or duplicate first?? Must change resource paths saved
	
	if (!source.IsFileLocatedInFolder(ASSETS_PATH))
	{
		source.SetFolders(ASSETS_PATH);
		app->fs->DuplicateFile(fileName, source.GetFullPath());
	}

	if (checkFirst)
	{
		Resource* tmp = FindResourceFromOriginalFullPath(source.GetFullPath());
		if (tmp)
			return tmp->GetUID();
	}

	RESOURCE_TYPE type = GetTypeFromExtension(source.GetExtension());

	bool success = false;
	Path exportedPath;
	UID resid = 0;

	switch (type)
	{
		//TODO
	case RES_TEXTURE:
		success = textureImporter->Import(source, exportedPath, resid);
		break;
	case RES_MATERIAL:
		break;
	case RES_SCENE:
		success = sceneImporter->Import(source, exportedPath, resid);
		break;
	case RES_SHADER:
		break;
	}

	if (success)
	{
		Resource* r = CreateResource(type, resid);
		r->originalFile.SetFullPath(source.GetFullPath());
		r->exportedFile.SetFullPath(exportedPath.GetFullPath());
		r->name = source.GetFileName();
		ret = r->GetUID();

		_LOG(LOG_INFO, "Imported file [%s] to [%s].", r->GetOriginalFile(), r->GetExportedFile());
	}
	else
	{
		_LOG(LOG_ERROR, "Could not import file [%s].", fileName);
	}
	
	return ret;
}

/** M_ResourceManager - ImportBuff: Import a buffer. Can be used to import meshes or materials casting the structure pointer to void pointer.
						It is a bit dirty way but good for now.
						Size nad source file parameter is not always needed but has to be passed.
						Return the resource UID or 0 if fail.
						*/
UID M_ResourceManager::ImportBuf(const void * buffer, RESOURCE_TYPE type, uint size, Path* sourceFile)
{
	UID ret = 0;

	if (!buffer)
		return ret;

	bool succes = false;
	Path output;

	switch (type)
	{
	case RES_MESH:
		succes = meshImporter->ImportMesh((const aiMesh*)buffer, output, ret);
		break;
	case RES_TEXTURE:
		succes = textureImporter->ImportBuff(buffer, size, output, ret);
		break;
	case RES_MATERIAL:
		succes = materialImporter->Import((const aiMaterial*)buffer, output, ret, sourceFile);
		break;
	case RES_SCENE:
		break;
	case RES_SHADER:
		break;
	}

	if (succes && ret != 0)
	{
		Resource* res = CreateResource(type, ret);
		res->originalFile.SetFullPath((sourceFile) ? sourceFile->GetFullPath() : "unknown");
		res->exportedFile.SetFullPath(output.GetFullPath());
		res->name = sourceFile ? sourceFile->GetFileName() : "unamed";
		_LOG(LOG_INFO, "Imported a buffer succesfully [%s].", output.GetFullPath());
	}
	else
	{
		_LOG(LOG_WARN, "Could not import the buffer.");
	}

	return ret;
}

/** M_ResourceManager - GetResourceFromUID: Return a resource from its UID, nullptr if not founf. */
Resource * M_ResourceManager::GetResourceFromUID(UID uuid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uuid);
	return it != resources.end() ? it->second : nullptr;
}

/** M_ResourceManager - CreateResource: Create a desired type resource. If no passed UID, 0 UID or 
										already used UID a new one is randomly generated. 
										The new resource is added to the resource map.
										Return the created resource. */
Resource * M_ResourceManager::CreateResource(RESOURCE_TYPE type, UID forceUID)
{
	Resource* ret = nullptr;

	if (forceUID == 0 || GetResourceFromUID(forceUID))
		forceUID = GetNewUID();

	switch (type)
	{
	case RES_MESH:
		ret = (Resource*)new ResourceMesh(forceUID);
		break;
	case RES_TEXTURE:
		ret = (Resource*)new ResourceTexture(forceUID);
		break;
	case RES_MATERIAL:
		ret = (Resource*)new ResourceMaterial(forceUID);
		break;
	case RES_SCENE:
		ret = (Resource*)new ResourceScene(forceUID);
		break;
	case RES_SHADER:
		ret = (Resource*)new ResourceShader(forceUID);
		break;
	}

	if (ret)
		resources[forceUID] = ret;

	return ret;
}

/** M_ResourceManager - FindResourceFromOriginalFullPath: Get a resource from its original full path. Return nullptr if not found. */
Resource * M_ResourceManager::FindResourceFromOriginalFullPath(const char * fullpath)
{
	for (auto it : resources)
	{
		if (it.second->originalFile.GetFullPath() == fullpath)
			return it.second;
	}

	return nullptr;
}

/** M_ResourceManager - GetTypeFromExtension: Return the reource type from a extension string. Res_none if not identified. */
RESOURCE_TYPE M_ResourceManager::GetTypeFromExtension(const char * ext) const
{
	RESOURCE_TYPE ret = RES_NONE;

	if (strcmp(ext, "png") == 0)
		ret = RES_TEXTURE;
	else if (strcmp(ext, "jpg") == 0)
		ret = RES_TEXTURE;
	else if (strcmp(ext, "tga") == 0)
		ret = RES_TEXTURE;
	else if (strcmp(ext, "fbx") == 0)
		ret = RES_SCENE;
	else if (strcmp(ext, "obj") == 0)
		ret = RES_SCENE;
	else if (strcmp(ext, TEXTURE_EXTENSION) == 0)
		ret = RES_TEXTURE;
	else if (strcmp(ext, MESH_EXTENSION) == 0)
		ret = RES_MESH;
	else if (strcmp(ext, MATERIAL_EXTENSION) == 0)
		ret = RES_MATERIAL;
	else if (strcmp(ext, SCENE_EXTENSION) == 0)
		ret = RES_SCENE;
	else if (strcmp(ext, PREFAB_EXTENSION) == 0)
		ret = RES_SCENE;

	//TODO: Add more extensions

	return ret;
}

/** M_ResourceManager - GetNewUID: Return a new rnadom UID. */
UID M_ResourceManager::GetNewUID() const
{
	return app->random->GetRandInt();
}

/** M_ResourceManager - GetResourcesOfType: Fill a passed vector with all resources of type. Sveral types can be passed at once. */
void M_ResourceManager::GetResourcesOfType(std::vector<Resource*>& res, RESOURCE_TYPE type) const
{
	for (auto it : resources)
	{
		if (type & it.second->GetType())
			res.push_back(it.second);
	}
}

/** M_ResourceManager - LoadResources: Loads all resources from the resource file. */
void M_ResourceManager::LoadResources()
{
	char* buffer = nullptr;
	uint size = app->fs->Load((RESOURCES_PATH + resourceFile).c_str(), &buffer);

	if (buffer && size > 0)
	{
		JsonFile file(buffer);

		uint count = file.GetArraySize("resources");
		for (uint i = 0; i < count; ++i)
		{
			JsonFile r(file.GetArray("resources", i));
			RESOURCE_TYPE type = (RESOURCE_TYPE)r.GetInt("type", RES_NONE);
			UID id = r.GetInt("UID", 0);

			if (GetResourceFromUID(id))
				continue;

			Resource* res = CreateResource(type, id);
			res->Load(r);
		}
	}

	RELEASE_ARRAY(buffer);
}

/** M_ResourceManager - SaveResources: Saves all resources into the resource file. */
void M_ResourceManager::SaveResources()
{
	JsonFile save;
	save.AddArray("resources");

	std::map<UID, Resource*>::iterator it = resources.begin();
	for (; it != resources.end(); ++it)
	{
		if (it->first > RESERVED_RESOURCES)
		{
			JsonFile res;
			it->second->Save(res);
			save.AddArrayEntry(res);
		}
	}

	char* buffer = nullptr;
	uint size = save.WriteJson(&buffer, false);

	if (app->fs->Save((RESOURCES_PATH + resourceFile).c_str(), buffer, size) != size)
	{
		_LOG(LOG_ERROR, "Could not save resources!");
	}

	RELEASE_ARRAY(buffer);
}

/** M_ResourceManager - LoadBasicResources: Create all basic resources such as primitives, checker texture, default shader, etc. */
bool M_ResourceManager::LoadBasicResources()
{
	checkers = (ResourceTexture*)CreateResource(RES_TEXTURE, 1);
	if (!textureImporter->LoadChequers(checkers)) return false;
	checkers->AddInstance();

	cube = (ResourceMesh*)CreateResource(RES_MESH, 2);
	if (!meshImporter->LoadCube(cube)) return false;
	cube->AddInstance();

	quad = (ResourceMesh*)CreateResource(RES_MESH, 3);
	if(!meshImporter->LoadQuad(quad)) return false;
	quad->AddInstance();

	plane = (ResourceMesh*)CreateResource(RES_MESH, 4);
	if (!meshImporter->LoadPlane(plane)) return false;
	plane->AddInstance();

	//sphere = (ResourceMesh*)CreateResource(RES_MESH, 5);
	//if (!meshImporter->LoadSphere(sphere)) return false;
	//sphere->AddInstance();

	defaultShader = (ResourceShader*)CreateResource(RES_SHADER, 6);
	if (!shaderImporter->PrepareDefaultShader(defaultShader)) return false;
	defaultShader->AddInstance();

	return true;
}
