#ifndef __EDSHADEREDITOR_H__
#define __EDSHADEREDITOR_H__

#include "EdWin.h"

#include <string>

class ResourceShader;

class EdShaderEditor : public EdWin
{
	enum SHADER_TYPE
	{
		SH_VERTEX = 0,
		SH_FRAGMENT,
		SH_GEOMETRY
	};

public:
	EdShaderEditor(bool startEnabled = false);
	virtual ~EdShaderEditor();

	void Draw()override;

	void SetShader(ResourceShader* shader);
	ResourceShader* GetCurrentShaderEdditing()const;

private:
	bool SaveCurrentShader();
	bool LoadCurrentShader();

	void LoadNewShaderFile(ResourceShader* sh);

	void LoadShaderMenu();
	void CreateShaderMenu();

private:
	ResourceShader* currentShader = nullptr;

	SHADER_TYPE editingShader = SH_VERTEX;

	bool shaderLoaded = false;

	bool loadMenu = false;
	bool createMenu = false;

	//TODO: Set a small viewport with a shader visualitzation render

};

#endif // !__EDSHADEREDITOR_H__