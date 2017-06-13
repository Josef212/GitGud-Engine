#ifndef __M_RENDERER_H__
#define __M_RENDERER_H__

#include "Module.h"
#include <SDL.h>

class GameObject;
class Camera;

class M_Renderer : public Module
{
public:
	M_Renderer(const char* name, bool startEnabled = true);
	virtual ~M_Renderer();

	bool Init(JsonFile* file)override;
	bool Start()override;
	UPDATE_RETURN PreUpdate(float dt)override;
	UPDATE_RETURN PostUpdate(float dt)override;
	bool CleanUp()override;

	bool GetVSync()const;
	void SetVSync(bool set);

	Camera* GetCurrentCamera()const;
	void SetCamera(Camera* cam);


private:
	void OnResize(int width, int height);

	void DrawObject(GameObject* object, Camera* cam);


	//****
	//TMP
	uint shader;
	int viewLoc, modelLoc, projLoc;

	void CreateShader();

	void Draw();
	void DrawChilds(GameObject* object, Camera* cam);

	//-------------------

public:
	bool showGrid = true;

private:
	SDL_GLContext context;
	bool vsync;

	Camera* currentCamera = nullptr; //TODO: Only one camera?? Viewport??
};


#endif // !__M_RENDERER_H__