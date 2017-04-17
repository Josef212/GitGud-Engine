#ifndef __IMPORTER_SCENE_H__
#define __IMPORTER_SCENE_H__

#include "Importer.h"
#include <string>
#include <map>

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

	bool Import(const char* originalFile, std::string& exportedFile, const char* originalFileExt, UID& resUID);

	bool LoadResource(Resource* resource)override;

private:
	void RecImport(const aiScene* scene, const aiNode* node, GameObject* parent, const std::string& basePath, const std::string& file);

private:
	std::map<int, ResourceMesh*> meshesImported;
};

#endif // !__IMPORTER_SCENE_H__