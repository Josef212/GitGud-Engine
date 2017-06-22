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
	Resource(UID uuid, RESOURCE_TYPE resType) : uuid(uuid), type(resType)
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
		return originalFile.GetFile();
	}

	const char* GetOriginalFileFullPath()const
	{
		return originalFile.GetFullPath();
	}

	const char* GetExportedFile()const
	{
		return exportedFile.GetFile();
	}

	const char* GetExportedFileFullPath()const
	{
		return exportedFile.GetFullPath();
	}

	const char* GetResourceName()const
	{
		return name.c_str();
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

	void AddInstance()
	{
		++instancesLoaded;
	}

	void RemoveInstance()
	{
		--instancesLoaded;
		if (instancesLoaded <= 0)
			RemoveFromMemory();
	}

	void Save(JsonFile& file)
	{
		file.AddInt("UID", uuid);
		file.AddInt("type", (int)type);
		file.AddString("original_file", originalFile.GetFullPath());
		file.AddString("exported_file", exportedFile.GetFullPath());
		file.AddString("resource_name", name.c_str());
		OnSave(file);
	}

	void Load(JsonFile& file)
	{
		uuid = file.GetInt("UID", 0);
		originalFile.SetFullPath(file.GetString("original_file", "???"));
		exportedFile.SetFullPath(file.GetString("exported_file", "???"));
		name = file.GetString("resource_name", "unamed");
		OnLoad(file);
	}

protected:
	virtual void OnSave(JsonFile& file) {}
	virtual void OnLoad(JsonFile& file) {}
	virtual bool LoadInMemory() = 0;
	virtual bool RemoveFromMemory() = 0;

public:
	Path originalFile;
	Path exportedFile;

	std::string name = "resource";

protected:
	RESOURCE_TYPE type;
	UID uuid;

	uint instancesLoaded = 0;
};

#endif // !__RESOURCE_H__
