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
	return false;
}

bool ImporterShader::PrepareDefaultShader(ResourceShader * sh)
{
	if (sh)
	{
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

		bool suc = true;

		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &v, nullptr);
		glCompileShader(vertex);

		GLint succes;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
		if (succes == 0)
		{
			GLchar infoLog[512];
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			_LOG(LOG_ERROR, "Vertex shader compilation error: %s.", infoLog);
			suc = false;
		}

		GLuint fragment;
		if (suc)
		{
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &f, nullptr);
			glCompileShader(fragment);

			GLint succes;
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
			if (succes == 0)
			{
				GLchar infoLog[512];
				glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
				_LOG(LOG_ERROR, "Fragment shader compilation error: %s.", infoLog);
				suc = false;
			}
		}

		GLuint retID = 0;

		if (suc)
		{
			retID = glCreateProgram();
			glAttachShader(retID, vertex);
			glAttachShader(retID, fragment);
			glLinkProgram(retID);

			GLint success;
			glGetProgramiv(retID, GL_LINK_STATUS, &success);
			if (success == 0)
			{
				GLchar infoLog[512];
				glGetProgramInfoLog(retID, 512, nullptr, infoLog);
				_LOG(LOG_ERROR, "Shader link error: %s.", infoLog);
			}
			else
			{
				 sh->shaderID = retID;
				sh->usable = true;
				_LOG(LOG_INFO, "Default shader compiled and linked successfully.", );
			}
		}

		glDetachShader(retID, vertex);
		glDetachShader(retID, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return sh->usable;
	}
	return false;
}
