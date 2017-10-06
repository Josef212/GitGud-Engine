#ifndef __MESH_H__
#define __MESH_H__

#include "Component.h"
#include "ComponentResource.h"
#include "Math.h"
#include "Color.h"

class ResourceMesh;
class JsonFile;

class Mesh : public Component, public ComponentResource
{
public:
	Mesh(GameObject* object);
	virtual ~Mesh();

	void GetBox(AABB& box)const override;

	void ClearMesh();

	void OnSaveCmp(JsonFile& sect)const override;
	void OnLoadCmp(JsonFile* sect)override;

	bool SetResource(UID resUID);

	void OnDebugDraw() override;

public:
	bool renderWirefreame = false;
	bool renderNormals = false;

	Color tint = White;

private:
	bool onVRAM = false;

};

#endif // !__MESH_H__