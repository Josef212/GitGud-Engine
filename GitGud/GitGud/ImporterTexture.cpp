#include "ImporterTexture.h"

#include "App.h"
#include "M_FileSystem.h"
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
