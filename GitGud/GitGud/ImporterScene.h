#ifndef __IMPORTER_SCENE_H__
#define __IMPORTER_SCENE_H__

#include "Importer.h"
#include "JsonFile.h"
#include <string>
#include <vector>

struct aiNode;
struct aiScene;
class ResourceMesh;
class ResourceScene;
class GameObject;

class ImporterScene : public Importer
{
public:
	ImporterScene();
	virtual ~ImporterScene();

	/**
	*	- Import:
	*		Import a scene file located in assets path.
	*		Imported scenes are actually fbx or other 3d scenes so they are imported into prefabs but engine 
	*		scenes are no imported since only exist in the engine logic.
	*		Before calling this make sure file has been copied to assets folder.
	*	
	*	- Path original file: File to import.
	*	- Path exported file: Resulting file path.
	*	- UID resUID: Resulting UID.	//TODO: Prob not necessary since is allocated and easily accessed from path
	*
	*/
	bool Import(Path originalFile, Path& exportedFile, UID& resUID);

	bool LoadResource(Resource* resource)override;

private:
	void RecImport(const aiScene* scene, const aiNode* node, GameObject* parent, Path& file, JsonFile& metaFile);

	void ImportMeshes(const aiScene* scene, Path& file, JsonFile& metaFile);
	void ImportMaterials(const aiScene* scene, Path& file, JsonFile& metaFile);
	void ImportBones(const aiScene* scene, Path& file, JsonFile& metaFile);
	void ImportAnimations(const aiScene* scene, Path& file, JsonFile& metaFile);

	bool SaveScene(Path& path, JsonFile& scene, JsonFile& metaFile);

private:
	std::vector<UID> meshesImported;
	std::vector<UID> materialsImported;
};

#endif // !__IMPORTER_SCENE_H__