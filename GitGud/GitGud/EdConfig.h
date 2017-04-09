#ifndef __EDCONFIG_H__
#define __EDCONFIG_H__

#include "EdWin.h"

#define MAX_FPS_LOG 100

class EdConfig : public EdWin
{
public:
	EdConfig(bool startEnabled = false);
	virtual ~EdConfig();

	void Draw()override;
	void PushFps(float _fps, float _ms);

private:
	float fps[MAX_FPS_LOG];
	float ms[MAX_FPS_LOG];
	int arraySize = 0;
};

#endif // !__EDCONFIG_H__