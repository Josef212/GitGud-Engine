#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"
#include <string>

class JsonFile;

typedef enum
{
	M_NONE = 0,
	M_INIT = 1 << 0,
	M_START = 1 << 1,
	M_PRE_UPDATE = 1 << 2,
	M_UPDATE = 1 << 3,
	M_POST_UPDATE = 1 << 4,
	M_CLEAN_UP = 1 << 5,
	M_SAVE_CONFIG = 1 << 6,
	M_RESIZE_EVENT = 1 << 7,
	M_DRAW_DEBUG = 1 << 8

} ModuleConfig;

#define MODULE_FULL_CONFIG\
	M_INIT | M_START |\
	M_PRE_UPDATE | M_UPDATE | M_POST_UPDATE |\
	M_CLEAN_UP |\
	M_SAVE_CONFIG |\
	M_RESIZE_EVENT

class Module
{
	friend class App;
public:
	Module(const char* name, bool startEnabled = true) : name(name), enabled(startEnabled) {}
	virtual ~Module() {}

	// ======================================================

	bool IsEnable() const { return enabled; }

	bool Enable()
	{
		if (!enabled)
		{
			enabled = true;
			return Start();
		}
		return false;
	}

	bool Disable()
	{
		if (enabled)
		{
			enabled = false;
			return CleanUp();
		}
		return false;
	}

	bool SetActive(bool set)
	{
		if (set != enabled)
		{
			return set ? Enable() : Disable();
		}
	}

	// ======================================================

	virtual bool Init(JsonFile* file) { return true; }
	virtual bool Start() { return true; }
	virtual UpdateReturn PreUpdate(float dt) { return UPDT_CONTINUE; }
	virtual UpdateReturn Update(float dt) { return UPDT_CONTINUE; }
	virtual UpdateReturn PostUpdate(float dt){ return UPDT_CONTINUE; }
	virtual bool CleanUp() { return true; }

	virtual bool Save(JsonFile* file) { return true; }
	virtual bool Load(JsonFile* file) { return true; }

	virtual void DrawDebug() {}
	virtual void OnResize(uint w, uint h) {}

private:

public:
	std::string name;

private:
	bool enabled;

protected:
	uint32 configuration = 0;
};

#endif // !__MODULE_H__