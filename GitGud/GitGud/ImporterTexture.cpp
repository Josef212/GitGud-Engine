#include "ImporterTexture.h"

#include "App.h"
#include "M_FileSystem.h"
#include "M_ResourceManager.h"
#include "ResourceTexture.h"

#include <il.h>
#include <ilu.h>
#include <ilut.h>


ImporterTexture::ImporterTexture()
{
	_LOG(LOG_INFO, "Texture importer: Created.");

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	ILuint devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		_LOG(LOG_ERROR, "Error while Devil Init: %s\n", iluErrorString(devilError));
	}
}

ImporterTexture::~ImporterTexture()
{
	ilShutDown();
}

bool ImporterTexture::Import(Path originalFile, Path & exportedFile, UID & resUID)
{
	bool ret = false;

	if (originalFile.Empty()) return ret;

	char* buffer = nullptr;
	uint size = app->fs->Load(originalFile.GetFullPath(), &buffer);

	if (buffer && size > 0)
		ret = ImportBuff(buffer, size, exportedFile, resUID);

	RELEASE_ARRAY(buffer);

	if (!ret)
		_LOG(LOG_WARN, "Could not import texture %s.", originalFile.GetFullPath());

	return ret;
}

bool ImporterTexture::ImportBuff(const void* buffer, uint size, Path& exportedFile, UID& resUID)
{
	bool ret = false;

	/**First load the image */
	//1-Gen the image and bind it
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	//2-Load the image from buffer
	if (ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		ilEnable(IL_FILE_OVERWRITE);

		ILuint _size;
		ILubyte* data = nullptr;

		//3-Set format (DDS, DDS compression)
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		//4-Get size
		_size = ilSaveL(IL_DDS, nullptr, 0);
		if (_size > 0)
		{
			//5-If size is more than 0 create the image buffer
			data = new ILubyte[_size];

			//6-Save the image with the correct format and save it with file system
			if (ilSaveL(IL_DDS, data, _size) > 0)
			{
				//Save it FS
				resUID = app->resources->GetNewUID();
				exportedFile.Set(TEXTURE_SAVE_PATH, std::to_string(resUID).c_str(), TEXTURE_EXTENSION);

				if (app->fs->Save(exportedFile.GetFullPath(), (const char*)data, _size) == _size)
					ret = true;
				else
					_LOG(LOG_WARN, "Error importing texture.");
			}

			//7-Release the image buffer to avoid memory leaks
			RELEASE_ARRAY(data);
		}

		//8-Finally if the image was loaded destroy the image to avoid more memory leaks
		ilDeleteImages(1, &image);
	}

	return ret;
}



bool ImporterTexture::LoadResource(Resource * resource)
{
	bool ret = false;

	if (!resource || resource->GetType() != RES_TEXTURE || resource->exportedFile.Empty())
		return ret;

	ResourceTexture* res = (ResourceTexture*)resource;

	char* data = nullptr;
	uint size = app->fs->Load(res->GetExportedFileFullPath(), &data);

	if (data && size > 0)
	{
		ILuint image = 0;
		ilGenImages(1, &image);
		ilBindImage(image);

		if (ilLoadL(IL_DDS, (const void*)data, size))
		{
			ILinfo info;
			iluGetImageInfo(&info);

			res->width = info.Width;
			res->height = info.Height;
			res->bpp = info.Bpp;
			res->depth = info.Depth;
			res->mips = info.NumMips;
			res->bytes = info.SizeOfData;

			switch (info.Format)
			{
			case IL_COLOUR_INDEX:
				res->format = ResourceTexture::COLOR_INDEX;
				break;
			case IL_RGB:
				res->format = ResourceTexture::RGB;
				break;
			case IL_RGBA:
				res->format = ResourceTexture::RGBA;
				break;
			case IL_BGR:
				res->format = ResourceTexture::BGR;
				break;
			case IL_BGRA:
				res->format = ResourceTexture::BGRA;
				break;
			case IL_LUMINANCE:
				res->format = ResourceTexture::LUMINANCE;
				break;
			default:
				res->format = ResourceTexture::UNKNOWN;
				break;
			}

			res->texID = ilutGLBindTexImage();
			ilDeleteImages(1, &image);

			ret = true;
		}
		else
		{
			_LOG(LOG_ERROR, "Devil could not load the texture resource [%s] from file [%s].", res->GetResourceName(), res->GetExportedFile());
		}
	}
	else
	{
		_LOG(LOG_ERROR, "Could not load texture resource [%s] from file [%s].", res->GetResourceName(), res->GetExportedFile());
	}

	RELEASE_ARRAY(data);

	return ret;
}

#define CHECKERS_WIDHT 64
#define CHECKERS_HEIGHT 64

bool ImporterTexture::LoadChequers(ResourceTexture * res)
{
	if (!res)return false;

	res->originalFile.SetFileName("*checkers*");
	res->exportedFile.SetFileName("*checkers*");
	res->name = "Checkers";

	GLubyte checkImage[CHECKERS_WIDHT][CHECKERS_HEIGHT][4];
	for (int i = 0; i < CHECKERS_HEIGHT; ++i)
	{
		for (int j = 0; j < CHECKERS_WIDHT; ++j)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	uint imageName = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &imageName);
	glBindTexture(GL_TEXTURE_2D, imageName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDHT, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	res->width = CHECKERS_WIDHT;
	res->height = CHECKERS_HEIGHT;
	res->bpp = 1;
	res->depth = 4;
	res->mips = 0;
	res->bytes = sizeof(GLubyte) * CHECKERS_WIDHT * CHECKERS_HEIGHT * 4;
	res->format = ResourceTexture::RGBA;
	res->texID = imageName;

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
