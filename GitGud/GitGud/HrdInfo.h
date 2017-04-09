#ifndef __HRDINFO_H__
#define __HRDINFO_H__

#include "Globals.h"
#include <SDL.h>
#include <string>

struct Hrd
{
	const char*		platform = nullptr;
	const char*		sdlRevision = nullptr;
	int				cpuCacheSize; //Kb
	int				cpuCores;
	int				ram; //Mb
	bool hasAVX, hasAltiVec, hasMMX, hasRDTSC, hasSSE, hasSSE2, hasSSE3, hasSSE41, hasSSE42, has3DNow;
	SDL_version sdlVersion;
};

class HrdInfo
{
private:
	Hrd info;
	std::string str;

public:
	HrdInfo();
	virtual ~HrdInfo();

	void SetInfo();
	void LogHrdInfo();
	Hrd* GetInfo() { return &info; }
	void SetCaps();
	const char* GetCaps();
};


#endif // !__HRDINFO_H__