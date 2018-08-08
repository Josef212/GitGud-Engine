#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"
#include <string>
#include "JsonFile.h"

enum ResourceType
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
	Resource(UID uuid, ResourceType resType) : uuid(uuid), type(resType)
	{}

	virtual ~Resource()
	{
	}

	/** GetType: Return the resource type. */
	ResourceType GetType()const
	{
		return type;
	}

	/** GetUID: Return the resource unique identifier. */
	UID GetUID()const
	{
		return uuid;
	}

	/**Count: Return the ammount of instances used/loaded. */
	uint Count()const
	{
		return instancesLoaded;
	}

	/** IsInMemory: Return true if is at least one instance in memory. */
	bool IsInMemory()const
	{
		return instancesLoaded > 0;
	}

	/** GetOriginalFile: Return the original file of the resource. */
	const char* GetOriginalFile()const
	{
		return originalFile.GetFile();
	}

	/** GetOriginalFileFullPath: Return the resource original file full path. */
	const char* GetOriginalFileFullPath()const
	{
		return originalFile.GetFullPath();
	}

	/** GetExportedFile: Return the resource exported file. */
	const char* GetExportedFile()const
	{
		return exportedFile.GetFile();
	}

	/** GetExportedFileFullPath: Return the resource exported full path. */
	const char* GetExportedFileFullPath()const
	{
		return exportedFile.GetFullPath();
	}

	/**GetResourceName: Return the resource name. */
	const char* GetResourceName()const
	{
		return name.c_str();
	}

	/** LoadToMemory: If the resource is already in memory just adds an instance to counter otherwise calls the overloaded LoadInMemory method to current load it for first time. 
						Return true if could load it properly, false if not. */
	bool LoadToMemory()
	{
		if (instancesLoaded > 0)
			++instancesLoaded;
		else
			instancesLoaded = LoadInMemory() ? 1 : 0;

		return instancesLoaded > 0;
	}

	/** FreeResource: If there are several instances in memory just remove one from the counter, otherwise if is the last isntace (counter == 1) calls the overloaded RemoveFromMemory method to actually free the resource.
						Return true if the resource was currently freed, false if could not free it or was already freed. */
	bool FreeResource()
	{
		bool ret = false;
		if (instancesLoaded > 0)
			ret = RemoveFromMemory();

		if (ret)
			instancesLoaded = 0;

		return ret;
	}

	/** AddInstance: Just add one to the instance counter. */
	void AddInstance()
	{
		++instancesLoaded;
	}

	/** RemoveInstace: Just remove one instace from the instances counter if is higher than one. If there was only one left also free the resource from memory. */
	void RemoveInstance()
	{
		if (instancesLoaded > 0)
		{
			if (instancesLoaded == 1)
				RemoveFromMemory();
			--instancesLoaded;
		}
	}

	/** Save: Saves into the json object the resource UID, type, original file, exported file and name. Also calls the overloaded OnSave method. */
	void Save(JsonFile& file)
	{
		file.AddInt("UID", uuid);
		file.AddInt("type", (int)type);
		file.AddString("original_file", originalFile.GetFullPath());
		file.AddString("exported_file", exportedFile.GetFullPath());
		file.AddString("resource_name", name.c_str());
		OnSave(file);
	}

	/** Load: Load into the resource from the json object the resource UID, type, original file, exported file and name. Also calls the overloaded OnLoad method. */
	void Load(JsonFile& file)
	{
		uuid = file.GetInt("UID", 0);
		originalFile.SetFullPath(file.GetString("original_file", "???").c_str());
		exportedFile.SetFullPath(file.GetString("exported_file", "???").c_str());
		name = file.GetString("resource_name", "unamed");
		OnLoad(file);
	}

protected:
	/** OnSave: Virtual method called while saving the resource. */
	virtual void OnSave(JsonFile& file) {}
	/** OnLoad: Virtual method called while laoding the resource. */
	virtual void OnLoad(JsonFile& file) {}
	/** LoadInMemory: Pure virtual method called when want to actually load the resource into memory. For example a mesh or a texture. */
	virtual bool LoadInMemory() = 0;
	/** RemoveFromMemory: Pure virtual method called when want to actually free the resource from memory. For example a mesh or a texture. */
	virtual bool RemoveFromMemory() = 0;

public:
	Path originalFile;
	Path exportedFile;

	std::string name = "resource";

protected:
	ResourceType type;
	UID uuid;

	uint instancesLoaded = 0;
};

#endif // !__RESOURCE_H__
