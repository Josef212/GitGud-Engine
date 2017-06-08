#ifndef __ED_RESOURCES_H__
#define __ED_RESOURCES_H__

#include "EdWin.h"
#include <vector>

class Resource;
class ResourceMesh;
class ResourceMaterial;
class ResourceTexture;
class ResourceShader;

class EdResources : public EdWin
{
public:
	EdResources(bool startEnabled = false);
	virtual ~EdResources();

	void Draw()override;

private:
	void MeshResource(std::vector<Resource*> meshes);
	void MaterialResource(std::vector<Resource*> materials);
	void TextureResource(std::vector<Resource*> textures);

	int infoW = 300;
	int infoH = 150; //TODO: Should calc this with the window size??
};

#endif // !__ED_RESOURCES_H__