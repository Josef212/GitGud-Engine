#include "ResourceShader.h"

#include "App.h"
#include "M_FileSystem.h"
#include "M_ResourceManager.h"
#include "ImporterShader.h"

#include "OpenGL.h"

#include <string>

ResourceShader::ResourceShader(UID uuid) : Resource(uuid, RES_SHADER)
{
}


ResourceShader::~ResourceShader()
{
	RemoveFromMemory();
}

bool ResourceShader::LoadInMemory()
{
	return app->resources->shaderImporter->LoadResource(this);
}

bool ResourceShader::RemoveFromMemory()
{
	if (shaderID != 0) glDeleteProgram(shaderID);

	if (!vertexCode.empty()) vertexCode.clear();
	if (!fragmentCode.empty()) fragmentCode.clear();
	if (!geometryCode.empty()) geometryCode.clear();

	usable = false;
	codeIsLoaded = false;

	return true;
}

uint ResourceShader::CompileCode(ShaderType type, const char * code)
{
	uint ret = 0;

	if (!code && !codeIsLoaded)
	{
		_LOG(LOG_WARN, "Must load shader code begore compiling it.");
		return ret;
	}

	if (!code)
	{
		switch (type)
		{
		case ResourceShader::SH_VERTEX:
			code = vertexCode.c_str();
			break;
		case ResourceShader::SH_FRAGMENT:
			code = fragmentCode.c_str();
			break;
		case ResourceShader::SH_GEOMETRY:
			code = geometryCode.c_str();
			break;
		default:
			_LOG(LOG_ERROR, "No shader code passed to compile.");
			break;
		}
	}

	if (code)
	{
		uint sh = 0;

		switch (type)
		{
		case ResourceShader::SH_VERTEX:
			sh = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ResourceShader::SH_FRAGMENT:
			sh = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ResourceShader::SH_GEOMETRY:
			sh = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		default:
			_LOG(LOG_WARN, "Invalid shader type passed.");
			break;
		}

		if (sh != 0)
		{
			glShaderSource(sh, 1, &code, NULL);
			glCompileShader(sh);

			if (CheckCompileErrors(sh, type)) ret = sh;
		}
	}

	return ret;
}

bool ResourceShader::LinkShader(uint vertex, uint fragment, uint geometry)
{
	int program = glCreateProgram();

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	if (geometry != 0) glAttachShader(program, geometry);

	glLinkProgram(program);

	GLint succes = 0;
	GLchar infoLog[1024];

	glGetProgramiv(program, GL_LINK_STATUS, &succes);
	if (!succes)
	{
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		_LOG(LOG_ERROR, "Shader link error: %s.", infoLog);
	}

	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
	if (geometry != 0) glDetachShader(program, geometry);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometry != 0) glDeleteShader(geometry);

	if (succes != 0)
	{
		_LOG(LOG_INFO, "Just compiled and linked succesfully [%s] shader.", name.c_str());
		shaderID = program;
		usable = true;
		return true;
	}
	else
	{
		_LOG(LOG_ERROR, "Could not link [%s] shader.", name.c_str());
		shaderID = 0;
		usable = false;
		return false;
	}
}

bool ResourceShader::CompileAndLink()
{
	bool ret = false;

	if (!codeIsLoaded)
		LoadCode();

	if (codeIsLoaded)
	{
		uint v = CompileCode(SH_VERTEX);
		uint f = CompileCode(SH_FRAGMENT);
		uint g = 0;
		if (!geometryCode.empty()) g = CompileCode(SH_GEOMETRY);

		ret = LinkShader(v, f, g);
	}

	return ret;
}

bool ResourceShader::LoadCode()
{
	bool ret = false;

	if (shaderFile.Empty())
	{
		_LOG(LOG_WARN, "No shader shader file.");
		codeIsLoaded = false;
		return ret;
	}

	char* buffer = nullptr;
	uint fileSize = app->fs->Load(shaderFile.GetFullPath(), &buffer);
	// TODO: Refactor this a bit. One json with all files info and single files for each shader.
	if (buffer && fileSize > 0)
	{
		JsonFile codeFile(buffer);

		vertexCode = codeFile.GetString("vertex_shader", "");
		fragmentCode = codeFile.GetString("fragment_shader", "");
		geometryCode = codeFile.GetString("geometry_shader", "");

		_LOG(LOG_INFO, "Shader code from [%s] loaded.", shaderFile.GetFile());

		codeIsLoaded = true;
		ret = true;
	}
	else
	{
		codeIsLoaded = false;
		_LOG(LOG_ERROR, "Could not load shader file.");
	}

	RELEASE_ARRAY(buffer);

	return ret;
}

void ResourceShader::OnSave(JsonFile & file)
{
	file.AddString("shader_file_full_path", shaderFile.GetFullPath());
}

void ResourceShader::OnLoad(JsonFile & file)
{
	shaderFile.SetFullPath(file.GetString("shader_file_full_path", "???").c_str());

	LoadInMemory();
}

void ResourceShader::OnCreation()
{
	shaderFile.Set(SHADER_SAVE_PATH, name.c_str(), SHADER_EXTENSION);

	static const char* v =
		"#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec2 uv;\n"
		"layout(location = 3) in vec3 color;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"out vec3 outNormal;\n"
		"out vec2 outUv; \n"
		"out vec3 outColor;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(position, 1.0);\n"
		"	outNormal = normal;\n"
		"	outUv = uv;\n"
		"	outColor = color;\n"
		"}\n"
		;

	static const char* f =
		"#version 330 core\n"
		"in vec3 outNormal;\n"
		"in vec2 outUv; \n"
		"in vec3 outColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	//FragColor = vec4(outColor, 1.0);\n"
		"	//FragColor = vec4(0.7, 0.7, 0.7, 1.0); \n"
		"	FragColor = vec4(abs(outNormal), 1.0); \n"
		"}\n"
		;

	JsonFile code;

	JsonFile uniforms;
	JsonFile uniform1Example;
	uniform1Example.AddString("type", "sampler2D/int/float/vec3/mat4/...");
	uniform1Example.AddString("name", "unifrom name");

	uniforms.AddSection("uniform1", uniform1Example);
	code.AddSection("uniforms", uniforms); //TODO: Prob change uniforms from several objects to object array

	code.AddString("vertex_shader", v);
	code.AddString("fragment_shader", f);
	code.AddString("geometry_shader", "");

	auto buffer = code.Write(true);

	if (buffer.size() > 0)
	{
		uint size = app->fs->Save(shaderFile.GetFullPath(), buffer.c_str(), buffer.size());

		if (size != buffer.size())
		{
			_LOG(LOG_WARN, "Could not create a new shader file. [%s]", shaderFile.GetFile());
		}
	}
}

bool ResourceShader::CheckCompileErrors(uint shader, ShaderType type)
{
	GLint succes = 0;
	GLchar infoLog[1024];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::string typeStr = "Unknown";
		switch (type)
		{
		case ResourceShader::SH_VERTEX:
			typeStr = "Vertex";
			break;
		case ResourceShader::SH_FRAGMENT:
			typeStr = "Fragment";
			break;
		case ResourceShader::SH_GEOMETRY:
			typeStr = "Geometry";
			break;
		}

		_LOG(LOG_ERROR, "%s shader compilation error: %s.", typeStr.c_str(), infoLog);
	}

	return succes != 0;
}


