#ifndef __EDWIN_H__
#define __EDWIN_H__

class EdWin
{
public:
	EdWin(bool startEnabled) : active(startEnabled)
	{}

	virtual ~EdWin()
	{}

	virtual void Draw()
	{}

	bool IsActive()const { return active; }
	void SetActive(bool set) { active = set; }
	void SwapActive() { active = !active; }

public:
	bool active = false;
};

#endif // !__EDWIN_H__
