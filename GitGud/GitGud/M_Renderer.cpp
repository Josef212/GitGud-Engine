#include "M_Renderer.h"

#include "App.h"
#include "M_Window.h"
#include "M_Editor.h"
#include "M_Camera3D.h"

#include "Camera.h"

#include "OpenGL.h"

/*
 -opengl32.lib
 -glu32.lib
 -glew32.lib
*/

M_Renderer::M_Renderer(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG("Renderer: Creation.");
}


M_Renderer::~M_Renderer()
{
	_LOG("Renderer: Destroying.");
}

bool M_Renderer::Init(JsonFile* file)
{
	_LOG("Renderer: Init.");
	bool ret = true;

	vsync = file->GetBool("vsync", true);

	context = SDL_GL_CreateContext(app->win->GetWindow());
	if (context == nullptr)
	{
		_LOG("REND_Error: OpenGL could not create context! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		glewExperimental = GL_TRUE;
		GLenum gl = glewInit();
		if (gl != GLEW_OK)
		{
			_LOG("REND_Error: Glew lib could not init %s\n", glewGetErrorString(gl));
			ret = false;
		}
	}

	if (ret)
	{
		_LOG("Vendor: %s", glGetString(GL_VENDOR));
		_LOG("Renderer: %s", glGetString(GL_RENDERER));
		_LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		_LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		
		SetVSync(vsync);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			_LOG("REND_Error: Could not init OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glClearDepth(1.0f);

		glClearColor(0.f, 0.f, 0.f, 1.f);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			_LOG("REND_Error: Could not init OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glEnable(GL_DEPTH_TEST | GL_CULL_FACE);

		//TODO: Send event instead of direct resize
		OnResize(app->win->GetWidth(), app->win->GetHeight());
	}

	return ret;
}

bool M_Renderer::Start()
{
	_LOG("Renderer: Start.");
	return true;
}

UPDATE_RETURN M_Renderer::PreUpdate(float dt)
{
	Camera* cam = app->camera->GetEditorCamera();
	if (cam)
	{
		Color col = cam->GetBackground();
		glClearColor(col.r, col.g, col.b, col.a);
	}
	else
	{
		glClearColor(0.f, 0.f, 0.f, 1.f);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	return UPDT_CONTINUE;
}

UPDATE_RETURN M_Renderer::PostUpdate(float dt)
{
	UPDATE_RETURN ret = UPDT_CONTINUE;





	//TODO: Editor state
	app->editor->DrawEditor();

	SDL_GL_SwapWindow(app->win->GetWindow());

	return ret;
}

bool M_Renderer::CleanUp()
{
	_LOG("Renderer: CleanUp.");

	SDL_GL_DeleteContext(context);

	return true;
}

bool M_Renderer::GetVSync() const
{
	return vsync;
}

void M_Renderer::SetVSync(bool set)
{
	if (vsync != set)
	{
		vsync = set;
		if (SDL_GL_SetSwapInterval(vsync ? 1 : 0) < 0)
			_LOG("Warning: Unable to set VSync! SDL_Error: %s\n", SDL_GetError());
	}
}

void M_Renderer::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
}
