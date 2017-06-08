#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__

#include "Resource.h"
#include "ResourceTexture.h"
#include "Color.h"
#include <string>
#include <vector>
#include <map>

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
	MaterialProperty()
	{}
	MaterialProperty(const char* propName, DATA_TYPE dType, void* data) : propertyName(propName), dataType(dType)
	{
		switch (dType)
		{
		case D_BOOL:
			this->data.b = (bool)&data;
			break;
		case D_FLOAT:
			this->data.f = (float)&data;
			break;
		case D_INT:
			this->data.i = &data;
			break;
		case D_UINT:
			this->data.ui = &data;
			break;
		case D_STRING:
			this->data.str = data;
			break;
		}
	}

	union
	{
		bool b;
		float f;
		int i;
		uint ui;
		std::string str;
	}data;

	DATA_TYPE dataType;
	std::string propertyName;
};

class ResourceMaterial : public Resource
{
public:
	ResourceMaterial(UID uuid);
	virtual ~ResourceMaterial();

	bool LoadInMemory()override;
	bool RemoveFromMemory()override;

public:
	//TODO: Must find a way to organiza propertirs. For now will hardcode a generic material based on unity standard material shader
	
	UID shader = 0;

	enum RENDER_MODE renderMode = RENDER_OPAQUE;
	
	UID ambientTexture = 0;
	Color ambientColor = White;

	UID diffuseTexture = 0;
	Color diffuseColor = White;

	//-----------

	UID metallicTexture = 0;
	Color metallicColor = White;
	float metallicValue = 0.0f; //0-1
	float metallicSmothness = 0.5f;//0-1
	//TODO: Source: metallic alpha / albedo alpha

	UID specularTexture = 0;
	Color specularColor = White;
	float specularSmothness = 0.5f;//0-1
	//TODO: Source: metallic alpha / albedo alpha

	//------------

	UID normalMap = 0;
	UID heightMap = 0;
	UID occlusion = 0;
	UID emissionTexture = 0;
	Color emissionColor = Black;
	float emissionValue = 0;
	//Global ilumin: none/realtime/baked

	UID detailMask = 0;
	 //TODO: Tiling/offset
	//TODO: Secondaty maps
	

};

#endif // !__RESOURCE_MATERIAL_H__