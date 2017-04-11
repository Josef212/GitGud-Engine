#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"
#include <string>
#include "JsonFile.h"

enum RESOURCE_TYPE
{
	RES_NONE = 0,
	RES_MESH = 1,
	RES_TEXTURE = 2,
	RES_MATERIAL = 4,
	RES_SCENE = 8,
	RES_SHADER = 16
};

class Resource
{
public:
	Resource(UID uuid, RESOURCE_TYPE resType)
	{}

	virtual ~Resource()
	{}

	RESOURCE_TYPE GetType()const
	{
		return type;
	}

	UID GetUID()const
	{
		return uuid;
	}

	uint Count()const
	{
		return instancesLoaded;
	}

	bool IsInMemory()const
	{
		return instancesLoaded > 0;
	}

	const char* GetOriginalFile()const
	{
		return originalFile.c_str();
	}

	const char* GetExportedFile()const
	{
		return exportedFile.c_str();
	}

	bool LoadToMemory()
	{
		if (instancesLoaded > 0)
			++instancesLoaded;
		else
			instancesLoaded = LoadInMemory() ? 1 : 0;

		return instancesLoaded > 0;
	}

	bool FreeResource()
	{
		bool ret = false;
		if (instancesLoaded > 0)
			ret = RemoveFromMemory();

		if (ret)
			instancesLoaded = 0;

		return ret;
	}

	virtual void Save(JsonFile& file)
	{
		file.AddInt("UID", uuid);
		file.AddInt("type", (int)type);
		file.AddString("original_file", originalFile.c_str());
		file.AddString("exported_file", exportedFile.c_str());
	}

	virtual void Load(JsonFile& file)
	{
		uuid = file.GetInt("UID", 0);
		type = (RESOURCE_TYPE)file.GetInt("type", 0);
		originalFile = file.GetString("original_file", "???");
		exportedFile = file.GetString("exported_file", "???");
	}

protected:
	virtual bool LoadInMemory() = 0;
	virtual bool RemoveFromMemory() = 0;

public:
	std::string originalFile = "";
	std::string exportedFile = "";

protected:
	RESOURCE_TYPE type;
	UID uuid;

	uint instancesLoaded = 0;
};

#endif // !__RESOURCE_H__
