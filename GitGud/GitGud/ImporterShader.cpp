#include "ImporterShader.h"

#include "ResourceShader.h"
#include "OpenGL.h"

ImporterShader::ImporterShader()
{
	_LOG(LOG_INFO, "Shader importer: Created.");
}


ImporterShader::~ImporterShader()
{
}

bool ImporterShader::LoadResource(Resource * resource)
{
	bool ret = false;

	if (!resource) return ret;

	ResourceShader* res = (ResourceShader*)resource;

	if (res->LoadCode())
	{
		uint vertex = res->CompileCode(ResourceShader::SHADER_TYPE::SH_VERTEX);
		uint fragment = res->CompileCode(ResourceShader::SHADER_TYPE::SH_FRAGMENT);
		uint geometry = 0;
		if(!res->geometryCode.empty()) 
			geometry = res->CompileCode(ResourceShader::SHADER_TYPE::SH_GEOMETRY);

		if (res->LinkShader(vertex, fragment, geometry))
		{
			ret = true;
		}
	}
	else
	{
		_LOG(LOG_ERROR, "Could not load shader code.");
	}

	return ret;
}

bool ImporterShader::PrepareDefaultShader(ResourceShader * sh)
{
	if (sh)
	{
		sh->name = "default_simple_shader";

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

		GLuint vertex = sh->CompileCode(ResourceShader::SHADER_TYPE::SH_VERTEX, v);
		GLuint fragment = sh->CompileCode(ResourceShader::SHADER_TYPE::SH_FRAGMENT, f);

		sh->LinkShader(vertex, fragment);

		return sh->usable;
	}
	return false;
}
