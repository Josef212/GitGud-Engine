#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__

#include "Resource.h"
#include "ResourceTexture.h"
#include "Color.h"

#include <string>
#include <vector>
#include <map>

class ResourceTexture;

enum SHADING_MODE
{
	SHADING_FLAT = 0x1,
	SHADING_GOURAUD = 0x2,
	SHADING_PHONG = 0x3,
	SHADING_BLINN = 0x4,
	SHADING_TOON = 0x5,
	SHADING_ORENAYAR = 0x6,
	SHADING_MINNAERT = 0x7,
	SHADING_COOLTORRANCE = 0x8,
	SHADING_NO_SHADING = 0x9,
	SHADING_FRESNEL = 0xA
};

enum BLEND_MODE
{
	/**
	*sourceColor * sourceAlpha + destColor * (1 - sourceAlpha)
	*/
	BLEND_DEFAULT = 0x0,

	/**
	*sourceColor * 1 + destColor * 1
	*/
	BLEND_ADDITIVE = 0x1
};

enum RENDER_MODE
{
	RENDER_OPAQUE = 0x0,
	RENDER_CUTOUT = 0x1,
	RENDER_TRANSPARENT = 0x2
};

struct MaterialProperty
{
	enum MAT_PROPERY_TYPE
	{
		MP_UNKNOWN = -1,
		MP_INT = 0,
		MP_FLOAT,
		MP_TEXTURE_RES,
		MP_VEC2,
		MP_VEC3,
		MP_VEC4,
		MP_MAT3,
		MP_MAT4
	}propertyType = MP_UNKNOWN;

	union
	{
		int iProp;
		float fProp;
		uint textureResId;
		float* fPointer;

	}property;

	const char* propertyName = nullptr;

	MaterialProperty(const char* name, int prop);
	MaterialProperty(const char* name, float prop);
	MaterialProperty(const char* name, uint prop);
	MaterialProperty(const char* name, float* prop, MAT_PROPERY_TYPE type);

	void SendInfoToShader(uint shader);
};

class ResourceMaterial : public Resource
{
	friend class ImporterMaterial;
public:
	ResourceMaterial(UID uuid);
	virtual ~ResourceMaterial();

	bool LoadInMemory()override;
	bool RemoveFromMemory()override;

	bool SetShader(uint shaderResource);

	void GetLocations();
	void SetPropertiesFromShaderFile();

	void SendMaterialToShader(float* model, float* camView, float* camProj);

	bool SaveMaterial();
	
private:

public:
	
	uint modelLoc = 0;
	uint viewLoc = 0;
	uint projLoc = 0;

private: 

	UID shader = 0;

	std::map<std::string, MaterialProperty*> properties;

};

#endif // !__RESOURCE_MATERIAL_H__