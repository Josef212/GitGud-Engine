#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Component.h"
#include "ComponentResource.h"

class Material : public Component, public ComponentResource
{
public:
	Material(GameObject* object);
	virtual ~Material();

	bool SetResource(UID resUID);

	void OnSaveCmp(JsonFile& sect)const override;
	void OnLoadCmp(JsonFile* sect)override;
};

#endif // !__MATERIAL_H__