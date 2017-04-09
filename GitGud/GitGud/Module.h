#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"
#include <string>

class Module
{
public:
	Module(const char* name, bool startEnabled = true) : name(name), enabled(startEnabled)
	{}

	virtual ~Module()
	{}

	bool IsEnable() { return enabled; }

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
			if (set)
				return Enable();
			else
				return Disable();
		}
	}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual UPDATE_RETURN PreUpdate(float dt)
	{
		return UPDT_CONTINUE;
	}

	virtual UPDATE_RETURN Update(float dt)
	{
		return UPDT_CONTINUE;
	}

	virtual UPDATE_RETURN PostUpdate(float dt)
	{
		return UPDT_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Save()
	{
		return true;
	}

	virtual bool Load()
	{
		return true;
	}

	virtual void DrawDebug()
	{}

private:

public:
	std::string name;

private:
	bool enabled;
};

#endif // !__MODULE_H__