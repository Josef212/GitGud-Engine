#ifndef __RESOURCE_SHADER_H__
#define __RESOURCE_SHADER_H__

#include "Resource.h"

class ResourceShader : public Resource
{
	enum SHADER_TYPE
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

	uint CompileCode(SHADER_TYPE type, const char* code = nullptr);
	bool LinkShader(uint vertex, uint fragment, uint geometry = 0);

	uint GetShaderID()const { return shaderID; }
	bool IsUsable()const { return usable; }
	bool IsCodeLoaded()const { return codeIsLoaded; }

	void OnSave(JsonFile& file)override;
	void OnLoad(JsonFile& file)override;

private:
	bool CheckCompileErrors(uint shader, SHADER_TYPE type);
	bool LoadCode();

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