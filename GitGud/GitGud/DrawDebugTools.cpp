#include "DrawDebugTools.h"
#include "M_Camera3D.h"
#include "Camera.h"
#include "App.h"

#include "OpenGL.h"

DrawDebug::DrawDebug()
{
}

DrawDebug::~DrawDebug()
{
}

void DrawDebug::DrawGrid(Color color)
{
	StartDrawDebug();

	glLineWidth(1.0f);
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_LINES);

	float d = 200.f;
	for (float i = -d; i <= d; i += 1.f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

	EndDrawDebug();
}

void DrawDebug::DrawAABB(AABB & box, Color color)
{
	StartDrawDebug();

	float3 vertices[8];
	box.GetCornerPoints(vertices);

	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[1]);

	glEnd();

	EndDrawDebug();
}

void DrawDebug::DrawFrustumDebug(Frustum & frust, Color color)
{
	StartDrawDebug();

	float3 vertices[8];
	frust.GetCornerPoints(vertices);

	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[1]);

	glEnd();

	EndDrawDebug();
}

void DrawDebug::DrawAxis(float3 position)
{
	StartDrawDebug();
	
	float4x4 model = float4x4::FromTRS(position, Quat::identity, float3::one);
	glPushMatrix();
	glMultMatrixf(model.Transposed().ptr());

	//---------------------------------------------

	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	glEnd();

	//---------------------------------------------

	glPopMatrix();
	
	EndDrawDebug();
}

void DrawDebug::DrawLine(float3 origin, float3 destination, float width, Color color)
{
	StartDrawDebug();

	glLineWidth(width);

	glBegin(GL_LINES);

	glColor3f(color.r, color.g, color.b);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	EndDrawDebug();
}

void DrawDebug::StartDrawDebug()
{
	glUseProgram(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Camera* cam = app->camera->GetEditorCamera();

	if (cam)
	{
		glPushMatrix();
		glMultMatrixf(cam->GetGLProjectionMatrix());
		glPushMatrix();
		glMultMatrixf(cam->GetGLViewMatrix());
	}
}

void DrawDebug::EndDrawDebug()
{
	glPopMatrix();
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glLineWidth(1.f);
	glColor4f(1.f, 1.f, 1.f, 1.f);
}
