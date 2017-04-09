#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>

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



//Configuration =============
#define APP_TITLE "GitGud"
#define APP_ORGANITZATION "Josef21296"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define WIN_SCALE 1

#endif // !__GLOBALS_H__
