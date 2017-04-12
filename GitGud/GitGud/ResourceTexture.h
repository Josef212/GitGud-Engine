#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__


#include "Resource.h"

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