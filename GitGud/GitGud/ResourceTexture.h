#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"

enum TEXTURE_TYPE
{
	TEX_NONE = 0x0,
	TEX_DIFFUSE = 0x1,
	TEX_SPECULAR = 0x2,
	TEX_AMBIENT = 0x3,
	TEX_EMISSIVE = 0x4,
	TEX_HEIGHT = 0x5,
	TEX_NORMAL_MAP = 0x6,
	TEX_SHININESS = 0x7,
	TEX_OPACITY = 0x8,
	TEX_DISPLACEMENT = 0x9,
	TEX_LIGHT_MAP = 0xA,
	TEX_REFLECTION = 0xB
};

enum TEXTURE_MAP_MODE
{
	TEX_WRAP = 0x1,
	TEX_CLAMP = 0x2,
	TEX_DECAL = 0x3,
	TEX_MIRROR = 0x4
};

enum TEXTURE_FLAG
{
	TEX_FLAG_INVERT = 0x1,
	TEX_FLAG_USE_ALPHA = 0x2,
	TEX_FLAG_IGNORE_ALPHA = 0x4
};

class ResourceTexture : public Resource
{
public:
	enum FORMAT
	{
		COLOR_INDEX = 0,
		RGB,
		RGBA,
		BGR,
		BGRA,
		LUMINANCE,
		UNKNOWN
	};

	ResourceTexture(UID uuid);
	virtual ~ResourceTexture();

	bool LoadInMemory()override;
	bool RemoveFromMemory()override;

public:
	uint width = 0;
	uint height = 0;
	uint depth = 0;
	uint bpp = 0;
	uint mips = 0;
	uint bytes = 0;

	uint texID = 0;

	FORMAT format = UNKNOWN;
};

#endif // !__RESOURCE_TEXTURE_H__