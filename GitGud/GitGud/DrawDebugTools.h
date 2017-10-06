#ifndef __DRAWDEBUG_H__
#define __DRAWDEBUG_H__

#include "Math.h"
#include "Color.h"

static class DrawDebug
{
public:
	DrawDebug();
	~DrawDebug();

	static void DrawGrid(Color color = White);
	static void DrawAABB(AABB& box, Color color = White);
	static void DrawFrustumDebug(Frustum& frust, Color color = White);
	static void DrawAxis(float3 position);
	static void DrawLine(float3 origin, float3 destination, float width = 1.f, Color color = White);

private:
	static void StartDrawDebug();
	static void EndDrawDebug();

};


#endif // !__DRAWDEBUG_H__
