#ifndef __EDWIN_H__
#define __EDWIN_H__

#include "Globals.h"

class EdWin
{
public:
	EdWin(bool startEnabled) : active(startEnabled)
	{}

	virtual ~EdWin()
	{}

	virtual void Draw()
	{}

	virtual void OnEnable()
	{}

	virtual void OnDisable()
	{}

	bool IsActive()const { return active; }

	void SetActive(bool set)
	{
		if (set != active)
		{
			if (active)
				Disable();
			else
				Enable();
		}
	}

	void SwapActive() 
	{
		if (active)
			Disable();
		else
			Enable();
	}

	void Enable() 
	{
		if (!active)
		{
			active = true;
			OnEnable();
		}
	}
	void Disable() 
	{
		if (active)
		{
			active = false;
			OnDisable();
		}
	}

public:
	bool active = false;
};

#endif // !__EDWIN_H__
