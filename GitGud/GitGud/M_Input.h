#ifndef __M_INPUT_H__
#define __M_INPUT_H__


#include "Module.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class M_Input : public Module
{
public:
	M_Input(const char* name, bool startEnabled = true);
	virtual ~M_Input();

	bool Init(JsonFile* file)override;
	UpdateReturn PreUpdate(float dt)override;
	bool CleanUp()override;

	KEY_STATE GetKey(int id)const { return keyboard[id]; }
	KEY_STATE GetMouseButton(int id)const { return mouse[id]; }

	bool GetKetDown(int id)const { return keyboard[id] == KEY_DOWN; }
	bool GetKetUp(int id)const { return keyboard[id] == KEY_UP; }
	bool GetKetRepeat(int id)const { return keyboard[id] == KEY_REPEAT; }

	int GetMouseX()const { return mouseX; }
	int GetMouseY()const { return mouseY; }
	int GetMouseMotionX()const { return mouseMotionX; }
	int GetMouseMotionY()const { return mouseMotionY; }
	int GetMouseWheel()const { return wheelY; }

	void GetMouseScreenPos(int& x, int& y)const
	{
		x = mouseX;
		y = mouseY;
	}

	void GetMouseMotion(int& x, int& y)const
	{
		x = mouseMotionX;
		y = mouseMotionY;
	}


private:
	KEY_STATE* keyboard = nullptr;
	KEY_STATE mouse[MAX_MOUSE_BUTTONS];
	int mouseX, mouseY, mouseMotionX, mouseMotionY, wheelY;
};


#endif // !__M_INPUT_H__