#include "Light.h"
#include "App.h"
#include "M_GoManager.h"


Light::Light(GameObject* object) : Component(object, CMP_LIGHT)
{
	app->goManager->AddLight(this);
}

Light::~Light()
{
	app->goManager->RemoveLight(this);
}

void Light::SetType(LIGHT_TYPE _type)
{
	lightType = _type;
}

LIGHT_TYPE Light::GetType() const
{
	return lightType;
}

void Light::OnEnable()
{
	//TODO: Manage lights from go manager
}

void Light::OnDisable()
{
	//TODO: Manage lights from go manager
}

void Light::OnGameObjectDestroyed()
{
	//TODO: Manage lights from go manager
}

void Light::OnTransformUpdate(Transform * trans)
{
	//Not really necessary since position and direction can be get from transform cmp
}

void Light::OnSaveCmp(JsonFile & sect) const
{
	sect.AddInt("light_type", (int)lightType);
	sect.AddColor("light_color", color);
	sect.AddFloat("intensity", intensity);
	sect.AddFloat("range", range);
	sect.AddFloat("spot_angle", spotAngle);
	
}

void Light::OnLoadCmp(JsonFile * sect)
{
	if (sect)
	{
		lightType = (LIGHT_TYPE)sect->GetInt("light_type", 1);
		color = sect->GetColor("light_color", White);
		intensity = sect->GetFloat("intensity", 1.f);
		range = sect->GetFloat("range", 5.f);
		spotAngle = sect->GetFloat("spot_angle", 15.f);
	}
}
