#ifndef __EDWINCREATOR_H__
#define __EDWINCREATOR_H__

#include "EdWin.h"

class Resource;
class ResourceMaterial;
class Material;
class GameObject;

class EdMaterialCreator : public EdWin
{
public:
	EdMaterialCreator(bool startEnabled = false);
	virtual ~EdMaterialCreator();

	void Draw()override;

public:
	ResourceMaterial* currentMaterial = nullptr;

};

#endif // !__EDWINCREATOR_H__