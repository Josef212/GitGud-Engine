#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>
#include "Path.h"

#define _LOG(format, ...) _log(__FILE__, __LINE__, format, __VA_ARGS__);

void _log(const char file[], int line, const char* format, ...);

#define RELEASE( x )\
	{\
		if(x != nullptr)\
		{\
			delete x;\
			x = nullptr;\
		}\
	}

#define RELEASE_ARRAY( x )\
	{\
		if(x != nullptr)\
		{\
			delete[] x;\
			x = nullptr;\
		}\
	}

#define CAP01(n) ((n <= 0.0f) ? n = 0.0f : (n >= 1.0f) ? n = 1.0f : n = n)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI
#define PI 3.14159265358979323846264338327950288

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;
typedef unsigned __int32 UID;

enum UPDATE_RETURN
{
	UPDT_CONTINUE = 0,
	UPDT_STOP,
	UPDT_ERROR
};

enum DATA_TYPE
{
	D_BOOL,
	D_FLOAT,
	D_INT,
	D_UINT,
	D_STRING
};

//Configuration =============
#define APP_TITLE "GitGud"
#define APP_ORGANITZATION "Josef21296"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define WIN_SCALE 1

//PATHS ========================

#define ASSETS_PATH "Data/Assets/"
#define LIBRARY_PATH "Data/Library/"

#define MESH_SAVE_PATH "Data/Library/Meshes/"
#define TEXTURE_SAVE_PATH "Data/Library/Textures/"
#define MATERIAL_SAVE_PATH "Data/Library/Materials/"
#define SHADER_SAVE_PATH "Data/Library/Shaders/"

#define SCENE_SAVE_PATH "Data/Assets/Scenes/"
#define PREFABS_SAVE_PATH "Data/Prefabs/"	//TODO: Prefabs should be inside assets root dir and let user place them whereber u want

#define CONFIG_PATH "Data/Configuration/"
#define RESOURCES_PATH "Data/Resources/"

#define MESH_EXTENSION "ggmesh"
#define TEXTURE_EXTENSION "dds"//"ggtex"
#define MATERIAL_EXTENSION "ggmat"
#define SCENE_EXTENSION "ggscene"
#define PREFAB_EXTENSION "json"//"ggprefab"

// URLS ========================

#define URL_GITHUB "https://github.com/Josef212"
#define URL_REPO "https://github.com/Josef212/GitGud-Engine"
#define URL_RELEASES "https://github.com/Josef212/GitGud-Engine/releases"
#define URL_ISSUES "https://github.com/Josef212/GitGud-Engine/issues"
#define URL_PAGE ""

#define URL_SDL2 "https://www.libsdl.org/download-2.0.php"
#define URL_OPENGL "https://www.opengl.org/"
#define URL_ASSIMP "http://www.assimp.org/"
#define URL_DEVIL "http://openil.sourceforge.net/"
#define URL_IMGUI "https://github.com/ocornut/imgui"
#define URL_PHYSFS "https://icculus.org/physfs/"
#define URL_GLEW "http://glew.sourceforge.net/"
#define URL_PARSON "https://github.com/kgabis/parson"



#endif // !__GLOBALS_H__
