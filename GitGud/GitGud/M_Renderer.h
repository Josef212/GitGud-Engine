#ifndef __M_RENDERER_H__
#define __M_RENDERER_H__

#include "Module.h"
#include <SDL.h>


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


private:
	void OnResize(int width, int height);

	//TMP
	void TMPInit();

	void TMPDRAW();

	uint VBO, containrtVAO;
	uint shader;
	//---


public:
	bool showGrid = true;

private:
	SDL_GLContext context;
	bool vsync;
};


#endif // !__M_RENDERER_H__