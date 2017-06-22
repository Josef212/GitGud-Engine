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
	ResourceShader* currentShader = nullptr;

};

#endif // !__EDSHADEREDITOR_H__