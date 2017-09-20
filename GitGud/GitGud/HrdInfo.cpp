#include "HrdInfo.h"
#include "gpudetect/DeviceId.h"


HrdInfo::HrdInfo()
{
}


HrdInfo::~HrdInfo()
{
}

void HrdInfo::SetInfo()
{
	info.platform = SDL_GetPlatform();
	info.cpuCacheSize = SDL_GetCPUCacheLineSize();
	info.cpuCores = SDL_GetCPUCount();
	info.ram = SDL_GetSystemRAM();
	info.hasAVX = SDL_HasAVX();
	info.hasAltiVec = SDL_HasAltiVec();
	info.hasMMX = SDL_HasMMX();
	info.hasRDTSC = SDL_HasRDTSC();
	info.hasSSE = SDL_HasSSE();
	info.hasSSE2 = SDL_HasSSE2();
	info.hasSSE3 = SDL_HasSSE3();
	info.hasSSE41 = SDL_HasSSE41();
	info.hasSSE42 = SDL_HasSSE42();
	info.has3DNow = SDL_Has3DNow();
	SDL_GetVersion(&info.sdlVersion);
	info.sdlRevision = SDL_GetRevision();

	//GPU
	uint vendor, deviceId;
	std::wstring brand;
	unsigned __int64 videoMemBudget, videoMemUsage, videoMemAvailable, videoMemReserved;

	if (getGraphicsDeviceInfo(&vendor, &deviceId, &brand, &videoMemBudget, &videoMemUsage, &videoMemAvailable, &videoMemReserved))
	{
		info.gpuVendor = vendor;
		info.gpuDevice = deviceId;
		sprintf_s(info.gpuBrand, 250, "%S", brand.c_str());
		info.vRamMbBudget = float(videoMemBudget) / 1073741824.0f;
		info.vRamMbUsage = float(videoMemUsage) / (1024.f * 1024.f);
		info.vRamMbAvailable = float(videoMemAvailable) / (1024.f * 1024.f * 1024.f);
		info.vRamMbReserved = float(videoMemReserved) / (1024.f * 1024.f * 1024.f);
	}

	SetCaps();
}

void HrdInfo::LogHrdInfo()
{
	_LOG(LOG_INFO, "---Hardware information:  ---");
	_LOG(LOG_INFO, "CPU cores: %d.", info.cpuCores);
	_LOG(LOG_INFO, "CPU cache size: %dKb.", info.cpuCacheSize);
	_LOG(LOG_INFO, " System RAM: %dMb.", info.ram);
	_LOG(LOG_INFO, "Caps:");
	if (info.hasAVX)
		_LOG(LOG_INFO, "HasAVX");
	if (info.hasAltiVec)
		_LOG(LOG_INFO, "HasAltiVec");
	if (info.hasMMX)
		_LOG(LOG_INFO, "HasMMX");
	if (info.hasRDTSC)
		_LOG(LOG_INFO, "HasRDTSC");
	if (info.hasSSE)
		_LOG(LOG_INFO, "HasSSE");
	if (info.hasSSE2)
		_LOG(LOG_INFO, "HasSSE2");
	if (info.hasSSE3)
		_LOG(LOG_INFO, "HasSSE3");
	if (info.hasSSE41)
		_LOG(LOG_INFO, "HasSSE41");
	if (info.hasSSE42)
		_LOG(LOG_INFO, "HasSSE42");
	if (info.has3DNow)
		_LOG(LOG_INFO, "Has3DNow");
}

const Hrd * HrdInfo::GetInfo()
{
	unsigned __int64 videoMemBudget, videoMemUsage, videoMemAvailable, videoMemReserved;

	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &videoMemBudget, &videoMemUsage, &videoMemAvailable, &videoMemReserved))
	{
		info.vRamMbBudget = float(videoMemBudget) / (1024.f * 1024.f);
		info.vRamMbUsage = float(videoMemUsage) / (1024.f * 1024.f);
		info.vRamMbAvailable = float(videoMemAvailable) / (1024.f * 1024.f);
		info.vRamMbReserved = float(videoMemReserved) / (1024.f * 1024.f);
	}

	return &info;
}

void HrdInfo::SetCaps()
{
	if (info.hasAVX)
		strCaps.append("AVX ");
	if (info.hasAltiVec)
		strCaps.append("AltiVec ");
	if (info.hasMMX)
		strCaps.append("MMX ");
	if (info.hasRDTSC)
		strCaps.append("RDTSC ");
	if (info.hasSSE)
		strCaps.append("SSE ");
	if (info.hasSSE2)
		strCaps.append("SSE2 ");
	if (info.hasSSE3)
		strCaps.append("SSE3 ");
	if (info.hasSSE41)
		strCaps.append("SSE41 ");
	if (info.hasSSE42)
		strCaps.append("SSE42 ");
	if (info.has3DNow)
		strCaps.append("3DNow");
}

const char* HrdInfo::GetCaps()
{
	return strCaps.c_str();
}