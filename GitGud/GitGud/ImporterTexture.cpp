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
	_LOG("Texture importer: Created.");

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	ILuint devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		_LOG("Error while Devil Init: %s\n", iluErrorString(devilError));
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
		_LOG("Could not import texture %s.", originalFile.GetFullPath());

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
					_LOG("Error importing texture.");
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
	return false;
}

#define CHECKERS_WIDHT 64
#define CHECKERS_HEIGHT 64

bool ImporterTexture::LoadChequers(ResourceTexture * res)
{
	if (!res)return false;

	res->originalFile = "*checkers*";
	res->exportedFile = "*checkers*";

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
