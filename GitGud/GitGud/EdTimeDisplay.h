#ifndef __EDTIMEDISPLAY_H__
#define __EDTIMEDISPLAY_H__

#include "EdWin.h"

class EdTimeDisplay : public EdWin
{
public:
	EdTimeDisplay(bool startEnabled = false);
	virtual ~EdTimeDisplay();

	void Draw()override;
};

#endif // !__EDTIMEDISPLAY_H__