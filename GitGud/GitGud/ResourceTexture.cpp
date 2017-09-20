#include "ResourceTexture.h"

#include "App.h"
#include "M_ResourceManager.h"
#include "ImporterTexture.h"

#include "OpenGL.h"

ResourceTexture::ResourceTexture(UID uuid) : Resource(uuid, RES_TEXTURE)
{
}


ResourceTexture::~ResourceTexture()
{
}

/** LoadInMemory: Overloaded method. Actually loads the texture resource using the texture importer.
					It uses the texture exported file to load the data. */
bool ResourceTexture::LoadInMemory()
{
	return app->resources->textureImporter->LoadResource(this);
}

/** RemoveFromMemory: Overloaded method. Actually frees the texture resource from memory including VRAM. */
bool ResourceTexture::RemoveFromMemory()
{
	width = 0;
	height = 0;
	depth = 0;
	bpp = 0;
	mips = 0;
	bytes = 0;

	format = UNKNOWN;
	textureType = TEX_NONE;

	if (texID > 0)
	{
		glDeleteBuffers(1, &texID);
		texID = 0;
		return true;
	}

	return false;
}

/** GetTextureTypeStr: Return the texture type as string.
					Types: None, Diffuse, Specular, Ambient, Emissive, Height, Normal map,
								Shininess, Opacity, Displacement, Light map, Reflection. */
const char * ResourceTexture::GetTextureTypeStr()const
{
	static const char* types[] = { 
		"none", "diffuse", "specular", "ambient", "emissive", "height", "normal map", 
		"shininess", "opacity", "displacement", "light map", "reflection" };

	return types[type];
}

/** GetTextureFormatStr: Return the texture format as string.
						Formats: Color index, Rgb, Rgba, Bgr, Bgra, Luminance, Unknown. */
const char * ResourceTexture::GetTextureFormatStr() const
{
	static const char* formats[] = {
		"color index", "rgb", "rgba", "bgr", "bgra", "luminance", "unknown" };

	return formats[format];
}
