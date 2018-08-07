#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Component.h"
#include "ComponentResource.h"

class Material : public Component, public ComponentResource
{
public:
	Material(GameObject* object);
	virtual ~Material();

	void OnSaveCmp(JsonFile& sect)const override;
	void OnLoadCmp(JsonFile* sect)override;

	ComponentType GetComponentType()override { return type; }
};

#endif // !__MATERIAL_H__