#include "ResourceShader.h"

#include "App.h"
#include "M_FileSystem.h"

#include "OpenGL.h"

ResourceShader::ResourceShader(UID uuid) : Resource(uuid, RES_SHADER)
{
}


ResourceShader::~ResourceShader()
{
	if (shaderID != 0) glDeleteProgram(shaderID);
}

bool ResourceShader::LoadInMemory()
{
	return false;
}

bool ResourceShader::RemoveFromMemory()
{
	return false;
}

bool ResourceShader::CompileShader()
{
	if (vertexFile.Empty() || fragmentFile.Empty())
	{
		_LOG("ERROR: Invalid shader files.");
		return false;
	}

	uint v = CompileVertex();
	uint f = CompileFragment();

	GLuint retID = glCreateProgram();
	glAttachShader(retID, v);
	glAttachShader(retID, f);
	glLinkProgram(retID);

	GLint success;
	glGetProgramiv(retID, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(retID, 512, nullptr, infoLog);
		_LOG("Shader link error: %s.", infoLog);

		usable = false;
	}
	else
	{
		shaderID = retID;
		usable = true;
		_LOG("Shader %s compiled and linked successfully.", name.c_str());
	}

	glDetachShader(retID, v);
	glDetachShader(retID, f);
	glDeleteShader(v);
	glDeleteShader(f);

	return usable;
}

void ResourceShader::OnSave(JsonFile & file)
{
	file.AddString("vertex_file_full_path", vertexFile.GetFullPath());
	file.AddString("fragment_file_full_path", fragmentFile.GetFullPath());
}

void ResourceShader::OnLoad(JsonFile & file)
{
	vertexFile.SetFullPath(file.GetString("vertex_file_full_path", "???"));
	fragmentFile.SetFullPath(file.GetString("fragment_file_full_path", "???"));
}

uint ResourceShader::CompileVertex()
{
	char* buffer = nullptr;
	uint size = app->fs->Load(vertexFile.GetFullPath(), &buffer);

	if (buffer && size > 0)
	{
		buffer[size] = '\0';

		uint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &buffer, nullptr);
		glCompileShader(vertex);

		GLint succes;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
		if (succes == 0)
		{
			GLchar infoLog[512];
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			_LOG("Vertex shader compilation error: %s.", infoLog);
		}
		else
		{
			return vertex;
		}
	}

	RELEASE_ARRAY(buffer);

	return 0;
}

uint ResourceShader::CompileFragment()
{
	char* buffer = nullptr;
	uint size = app->fs->Load(fragmentFile.GetFullPath(), &buffer);

	if (buffer && size > 0)
	{
		buffer[size] = '\0';

		uint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &buffer, nullptr);
		glCompileShader(fragment);

		GLint succes;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
		if (succes == 0)
		{
			GLchar infoLog[512];
			glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
			_LOG("Vertex shader compilation error: %s.", infoLog);
		}
		else
		{
			return fragment;
		}
	}

	RELEASE_ARRAY(buffer);

	return 0;
}
