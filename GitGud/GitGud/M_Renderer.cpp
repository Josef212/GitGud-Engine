#include "M_Renderer.h"

#include "App.h"
#include "M_Window.h"
#include "M_Editor.h"
#include "M_Camera3D.h"
#include "M_ResourceManager.h"
#include "M_GoManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"

#include "ResourceMesh.h"
#include "ResourceShader.h"

#include "DrawDebugTools.h"

//TMP
#include "Math.h"

#include "OpenGL.h"

/*
 -opengl32.lib
 -glu32.lib
 -glew32.lib
*/

M_Renderer::M_Renderer(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG(LOG_INFO, "Renderer: Creation.");

	configuration = M_INIT | M_START | M_PRE_UPDATE | M_POST_UPDATE | M_CLEAN_UP | M_SAVE_CONFIG | M_RESIZE_EVENT | M_DRAW_DEBUG;
}


M_Renderer::~M_Renderer()
{
	_LOG(LOG_INFO, "Renderer: Destroying.");
}

bool M_Renderer::Init(JsonFile* file)
{
	_LOG(LOG_INFO, "Renderer: Init.");
	bool ret = true;

	vsync = file->GetBool("vsync", true);

	context = SDL_GL_CreateContext(app->win->GetWindow());
	if (context == nullptr)
	{
		_LOG(LOG_ERROR, "OpenGL could not create context! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		glewExperimental = GL_TRUE;
		GLenum gl = glewInit();
		if (gl != GLEW_OK)
		{
			_LOG(LOG_ERROR, "Glew lib could not init %s\n", glewGetErrorString(gl));
			ret = false;
		}
	}

	if (ret)
	{
		_LOG(LOG_INFO, "Vendor: %s", glGetString(GL_VENDOR));
		_LOG(LOG_INFO, "Renderer: %s", glGetString(GL_RENDERER));
		_LOG(LOG_INFO, "OpenGL version supported %s", glGetString(GL_VERSION));
		_LOG(LOG_INFO, "GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		
		SetVSync(vsync);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			_LOG(LOG_ERROR, "Could not init OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glClearDepth(1.0f);

		glClearColor(0.f, 0.f, 0.f, 1.f);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			_LOG(LOG_ERROR, "Could not init OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glEnable(GL_DEPTH_TEST); // | GL_CULL_FACE);
		//glDepthFunc(GL_LESS);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_FILL | GL_LINE

		//TODO: Send event instead of direct resize
		OnResize(app->win->GetWidth(), app->win->GetHeight());
	}

	return ret;
}

bool M_Renderer::Start()
{
	_LOG(LOG_INFO, "Renderer: Start.");

	//TMP
	PrepareShaderLocs();

	//-----------------

	return true;
}

UpdateReturn M_Renderer::PreUpdate(float dt)
{
	Camera* cam = currentCamera ? currentCamera : app->camera->GetEditorCamera();
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

	return UPDT_CONTINUE;
}

UpdateReturn M_Renderer::PostUpdate(float dt)
{
	UpdateReturn ret = UPDT_CONTINUE;

	//TMP: Testing compatibility mode vs core

	//glLineWidth(2.0f);
	//glBegin(GL_LINES);
	//glVertex3f(0.0f, 0.0f, 0.0f);
	//glVertex3f(10.0f, 0.0f, 0.0f);
	//glEnd();
	//glLineWidth(1.0f);

	//-------------------------

	if (showGrid)
	{
		DrawDebug::DrawAxis(float3(0.f, 0.f, 0.f));
		DrawDebug::DrawGrid();
	}

	Camera* cam = currentCamera ? currentCamera : app->camera->GetEditorCamera(); //TODO: AppState, editor/game?

	std::vector<GameObject*> objects;
	app->goManager->GetToDrawStaticObjects(objects, cam);
	std::list<GameObject*>* dyn = app->goManager->GetDynamicObjects();

	//Static objects
	for (std::vector<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if(*it && (*it)->IsActive())
			DrawObject(*it, cam);
	}

	//Dynamic bjects
	if (dyn)
	{
		for (std::list<GameObject*>::iterator it = dyn->begin(); it != dyn->end(); ++it)
		{
			if(*it && (*it)->IsActive())
				if((*it)->enclosingBox.IsFinite() && cam->frustum.Intersects((*it)->enclosingBox))
					DrawObject(*it, cam);
		}
	}
	
	//------------

	//TODO: Debug draw
	if (app->debugMode)
	{
		app->DrawDebug();
	}

	//TODO: Editor state
	app->editor->DrawEditor();

	SDL_GL_SwapWindow(app->win->GetWindow());

	return ret;
}

bool M_Renderer::CleanUp()
{
	_LOG(LOG_INFO, "Renderer: CleanUp.");

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
			_LOG(LOG_WARN, "Unable to set VSync! SDL_Error: %s\n", SDL_GetError());
	}
}

Camera * M_Renderer::GetCurrentCamera() const
{
	return currentCamera;
}

void M_Renderer::SetCamera(Camera * cam)
{
	currentCamera = cam;
}

void M_Renderer::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
}

void M_Renderer::DrawObject(GameObject * object, Camera * cam)
{
	Mesh* meshCmp = (Mesh*)object->GetComponent(CMP_MESH);
	if (meshCmp)
	{
		ResourceMesh* mesh = (ResourceMesh*)meshCmp->GetResource();
		if (mesh)
		{
			glUseProgram(app->resources->defaultShader->GetShaderID());

			glBindVertexArray(mesh->idContainer);

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, object->transform->GetGlobalTransformGL());
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, cam->GetGLViewMatrix());
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, cam->GetGLProjectionMatrix());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndices);

			glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			glUseProgram(0);
		}
	}
}

void M_Renderer::PrepareShaderLocs()
{
	uint shader = app->resources->defaultShader->GetShaderID();

	viewLoc = glGetUniformLocation(shader, "view");
	modelLoc = glGetUniformLocation(shader, "model");
	projLoc = glGetUniformLocation(shader, "projection");
}

void M_Renderer::DrawChilds(GameObject * object, Camera* cam)
{
	for (auto it : object->childs)
	{
		if (it && cam->frustum.Intersects(it->enclosingBox))
			DrawObject(it, cam);
		DrawChilds(it, cam);
	}
}
