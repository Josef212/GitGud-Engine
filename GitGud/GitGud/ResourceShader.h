#ifndef __RESOURCE_SHADER_H__
#define __RESOURCE_SHADER_H__

#include "Resource.h"

class ResourceShader : public Resource
{
public:
	ResourceShader(UID uuid);
	virtual ~ResourceShader();

	bool LoadInMemory()override;
	bool RemoveFromMemory()override;

	bool CompileShader();

	uint GetShaderID()const { return shaderID; }
	bool IsUsable()const { return usable; }

	void OnSave(JsonFile& file)override;
	void OnLoad(JsonFile& file)override;

private:
	uint CompileVertex();
	uint CompileFragment();

public:
	Path vertexFile;
	Path fragmentFile;

private:
	bool usable = false;
	uint shaderID = 0;
};

#endif // !__RESOURCE_SHADER_H__