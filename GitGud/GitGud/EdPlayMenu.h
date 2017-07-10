#ifndef __EDPLAYMENU_H__
#define	 __EDPLAYMENU_H__

#include "EdWin.h"

class EdPlayMenu : public EdWin
{
public:
	EdPlayMenu(bool startEnabled = true);
	virtual ~EdPlayMenu();

	void Draw()override;
};

#endif // !__EDPLAYMENU_H__