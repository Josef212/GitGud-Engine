#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Component.h"
#include "Color.h"
#include "JsonFile.h"

enum LightType
{
	L_NONE = -1,
	L_DIRECTIONAL = 0,
	L_POINT = 1,
	L_SPOT = 2
};

class Light : public Component
{
public:
	Light(GameObject* object);
	virtual ~Light();

	void SetType(LightType _type);
	LightType GetType()const;

	void OnEnable()override;
	void OnDisable()override;

	void OnSaveCmp(JsonFile& sect)const override;
	void OnLoadCmp(JsonFile* sect)override;

public:
	LightType lightType = L_DIRECTIONAL;

	Color color = White;
	float intensity = 1.f;

	float range = 5.f; //For point and spot

	float spotAngle = 15.f; //For spot
};

#endif // !__LIGHT_H__