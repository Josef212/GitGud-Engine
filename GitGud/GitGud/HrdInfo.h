#ifndef __HRDINFO_H__
#define __HRDINFO_H__

#include "Globals.h"
#include <SDL.h>
#include <string>

struct Hrd
{
	//CPU
	const char*		platform = nullptr;
	int				cpuCacheSize; //Kb
	int				cpuCores;
	int				ram; //Mb
	bool hasAVX, hasAltiVec, hasMMX, hasRDTSC, hasSSE, hasSSE2, hasSSE3, hasSSE41, hasSSE42, has3DNow;
	//GPU
	uint gpuVendor = 0;
	uint gpuDevice = 0;
	char gpuBrand[250] = "";
	float vRamMbBudget = 0.f, vRamMbUsage = 0.f, vRamMbAvailable = 0.f, vRamMbReserved = 0.f;
	//SDL
	const char*		sdlRevision = nullptr;
	SDL_version sdlVersion;
};

class HrdInfo
{
private:
	Hrd info;
	std::string strCaps;

public:
	HrdInfo();
	virtual ~HrdInfo();

	void SetInfo();
	void LogHrdInfo();
	const Hrd* GetInfo();
	void SetCaps();
	const char* GetCaps();
};


#endif // !__HRDINFO_H__