#ifndef __EDINSPECTOR_H__
#define __EDINSPECTOR_H__

#include "EdWin.h"

class GameObject;
class Transform;
class Mesh;
class Material;
class Camera;

class EdInspector : public EdWin
{
public:
	EdInspector(bool startEnabled = false);
	virtual ~EdInspector();

	void Draw()override;

private:
	void DrawTrans(GameObject* selected);
	void DrawMesh(GameObject* selected, Mesh* mesh);
	void DrawCamera(GameObject* selected, Camera* cam);
};

#endif // !__EDINSPECTOR_H__