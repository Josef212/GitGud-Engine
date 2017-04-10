#ifndef __EDHIERARCHY_H__
#define __EDHIERARCHY_H__

#include "EdWin.h"

class GameObject;

class EdHierarchy : public EdWin
{
public:
	EdHierarchy(bool startEnabled = false);
	virtual ~EdHierarchy();

	void Draw()override;
	void RecDraw(GameObject* node, GameObject* selected = nullptr);
};

#endif // !__EDHIERARCHY_H__