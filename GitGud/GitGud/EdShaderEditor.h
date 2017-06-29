#ifndef __EDSHADEREDITOR_H__
#define __EDSHADEREDITOR_H__

#include "EdWin.h"

class ResourceShader;

class EdShaderEditor : public EdWin
{
public:
	EdShaderEditor(bool startEnabled = false);
	virtual ~EdShaderEditor();

	void Draw()override;

	void SetShader(ResourceShader* shader);
	ResourceShader* GetCurrentShaderEdditing()const;

private:
	bool SaveCurrentShader();
	bool LoadCurrentShader();
	void LoadNewShaderFile();
	void LoadShaderMenu();

	void ResizeBuffer(char* buffer, int newSize);

private:
	char* vertexFile = nullptr;
	char* fragmentFile = nullptr;
	ResourceShader* currentShader = nullptr;
	bool editingVertex = true; //Else edit fragment
	bool shaderLoaded = false;
	bool loadMenu = false;

	unsigned int vertexBufferSize = 0;
	unsigned int fragBufferSize = 0;

	//TODO: Set a small viewport with a shader visualitzation render

};

#endif // !__EDSHADEREDITOR_H__