#ifndef __RESOURCE_SHADER_H__
#define __RESOURCE_SHADER_H__

#include "Resource.h"

class ResourceShader : public Resource
{
	enum ShaderType
	{
		SH_ERROR = -1,
		SH_VERTEX = 0,
		SH_FRAGMENT,
		SH_GEOMETRY
	};

	friend class  ImporterShader;
public:
	ResourceShader(UID uuid);
	virtual ~ResourceShader();

	bool LoadInMemory()override;
	bool RemoveFromMemory()override;

	uint CompileCode(ShaderType type, const char* code = nullptr);
	bool LinkShader(uint vertex, uint fragment, uint geometry = 0);

	bool CompileAndLink();

	bool LoadCode();

	uint GetShaderID()const { return shaderID; }
	bool IsUsable()const { return usable; }
	bool IsCodeLoaded()const { return codeIsLoaded; }

	void OnSave(JsonFile& file)override;
	void OnLoad(JsonFile& file)override;

	const char* GetVertexCodeStr()const { return vertexCode.c_str(); }
	const char* GetFragmentCodeStr()const { return fragmentCode.c_str(); }
	const char* GetGeometryCodeStr()const { return geometryCode.c_str(); }

	int GetVertexCodeSize()const { return vertexCode.size(); }
	int GetFragmentCodeSize()const { return fragmentCode.size(); }
	int GetGeometryCodeSize()const { return geometryCode.size(); }

	void OnCreation();

private:
	bool CheckCompileErrors(uint shader, ShaderType type);

public:
	Path shaderFile;

private:
	bool usable = false;
	bool codeIsLoaded = false;
	uint shaderID = 0;

	std::string vertexCode = "";
	std::string fragmentCode = "";
	std::string geometryCode = "";
};

#endif // !__RESOURCE_SHADER_H__