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

	TMPInit();

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
	//glLoadIdentity();

	return UPDT_CONTINUE;
}

UPDATE_RETURN M_Renderer::PostUpdate(float dt)
{
	UPDATE_RETURN ret = UPDT_CONTINUE;


	TMPDRAW();


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

void M_Renderer::TMPInit()
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	glGenVertexArrays(1, &containrtVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containrtVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	std::string vertexCode = std::string(
		"#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(position, 1.0);\n"
		"}\n"
	);

	std::string fragCode = std::string(
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n"
	);

	const char* str = vertexCode.c_str();
	uint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &str, nullptr);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		_LOG("Vertex shader compilation error: %s.", infoLog);
	}

	str = fragCode.c_str();
	uint fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &str, nullptr);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
		_LOG("Fragment shader compilation error: %s.", infoLog);
	}

	shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragShader);
	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(shader, 512, nullptr, infoLog);
		_LOG("Shader link error: %s.", infoLog);
	}

	glDetachShader(shader, vertexShader);
	glDetachShader(shader, fragShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void M_Renderer::TMPDRAW()
{
	glBindVertexArray(containrtVAO);
	glUseProgram(shader);

	int viewLoc = glGetUniformLocation(shader, "view");
	int modelLoc = glGetUniformLocation(shader, "model");
	int projLoc = glGetUniformLocation(shader, "projection");

	Camera* cam = app->camera->GetEditorCamera();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, cam->GetGLViewMatrix());
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, cam->GetGLProjectionMatrix());
	float4x4 model = float4x4::identity;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.Transposed().ptr());

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUseProgram(0);
	glBindVertexArray(0);
}
