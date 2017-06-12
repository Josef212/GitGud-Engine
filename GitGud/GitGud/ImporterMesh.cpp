#include "ImporterMesh.h"

#include "App.h"
#include "ImporterMesh.h"
#include "ResourceMesh.h"
#include "M_ResourceManager.h"
#include "M_FileSystem.h"

#include <string>

#include <cimport.h>
#include <scene.h>
#include <postprocess.h>
#include <cfileio.h>

#include "OpenGL.h"

ImporterMesh::ImporterMesh() : Importer()
{
	_LOG("Mesh importer: Created.");
}

ImporterMesh::~ImporterMesh()
{
}

bool ImporterMesh::ImportMesh(const aiMesh * mesh, Path& output, UID & id)
{
	if (!mesh) return false;

	ResourceMesh m(0);

	m.name = (mesh->mName.length > 0) ? mesh->mName.C_Str() : "unamed_mesh";

	m.numVertices = mesh->mNumVertices;
	m.vertices = new float[m.numVertices * 3];
	memcpy(m.vertices, mesh->mVertices, sizeof(float) * m.numVertices * 3);

	if (mesh->HasFaces())
	{
		m.numIndices = mesh->mNumFaces * 3;
		m.indices = new uint[m.numIndices];
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
				_LOG("WARNING, geometry face with != 3 indices!");
			memcpy(&m.indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));

		}
	}

	if (mesh->HasNormals())
	{
		m.normals = new float[m.numVertices * 3];
		memcpy(m.normals, mesh->mNormals, sizeof(float) * m.numVertices * 3);
	}

	if (mesh->HasVertexColors(0))
	{
		m.colors = new float[m.numVertices * 3];
		memcpy(m.colors, mesh->mColors, sizeof(float) * m.numVertices * 3);
	}

	//TODO: Maybe 3d uvs
	if (mesh->HasTextureCoords(0))
	{
		m.uvs = new float[m.numVertices * 2];
		aiVector3D* tmp = mesh->mTextureCoords[0];
		for (uint i = 0; i < m.numVertices * 2; i += 2)
		{
			m.uvs[i] = tmp->x;
			m.uvs[i + 1] = tmp->y;
			++tmp;
		}
	}

	m.aabb.SetNegativeInfinity();
	m.aabb.Enclose((float3*)m.vertices, m.numVertices);

	id = SaveResource(&m, output);

	return (id > 0);
}

bool ImporterMesh::LoadResource(Resource * resource)
{
	if(!resource || resource->GetType() != RES_MESH || resource->exportedFile.Empty())
		return false;

	bool ret = false;

	ResourceMesh* res = (ResourceMesh*)resource;

	char* buffer = nullptr;
	uint size = app->fs->Load(res->GetExportedFileFullPath(), &buffer);

	if (buffer && size > 0)
	{
		char* cursor = buffer;

		//Ranges
		uint ranges[5];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		res->numIndices = ranges[0];
		res->numVertices = ranges[1];

		//Indices
		cursor += bytes;
		bytes = sizeof(uint) * res->numIndices;
		res->indices = new uint[res->numIndices];
		memcpy(res->indices, cursor, bytes);

		//Vertices
		cursor += bytes;
		bytes = sizeof(float) * res->numVertices * 3;
		res->vertices = new float[res->numVertices * 3];
		memcpy(res->vertices, cursor, bytes);

		//Normals
		if (ranges[2] > 0)
		{
			cursor += bytes;
			bytes = sizeof(float) * res->numVertices * 3;
			res->normals = new float[res->numVertices * 3];
			memcpy(res->normals, cursor, bytes);
		}

		//Colors
		if (ranges[3] > 0)
		{
			cursor += bytes;
			bytes = sizeof(float) * res->numVertices * 3;
			res->colors = new float[res->numVertices * 3];
			memcpy(res->colors, cursor, bytes);
		}

		//UVs
		if (ranges[4] > 0)
		{
			cursor += bytes;
			bytes = sizeof(float) * res->numVertices * 2;
			res->uvs = new float[res->numVertices * 2];
			memcpy(res->uvs, cursor, bytes);
		}

		//AABB
		cursor += bytes;
		bytes = sizeof(AABB);
		memcpy(&res->aabb, cursor, bytes);


		ret = true;

		GenBuffers(res);
	}

	RELEASE_ARRAY(buffer);

	return ret;
}

void ImporterMesh::GenBuffers(const ResourceMesh * res)
{
	if (res)
	{
		if (res->vertices && res->indices)
		{
			glGenVertexArrays(1, &(GLuint)res->idContainer);

			glGenBuffers(1, &(GLuint)res->idVertices);
			glGenBuffers(1, &(GLuint)res->idIndices);
			if (res->normals) glGenBuffers(1, &(GLuint)res->idNormals);
			if (res->colors) glGenBuffers(1, &(GLuint)res->idColors);
			if (res->uvs) glGenBuffers(1, &(GLuint)res->idUvs);

			glBindVertexArray(res->idContainer);

			//Vertices
			glBindBuffer(GL_ARRAY_BUFFER, res->idVertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * res->numVertices * 3, res->vertices, GL_STATIC_DRAW);

			//Indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->idIndices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * res->numIndices, res->indices, GL_STATIC_DRAW);

			//Vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
			glEnableVertexAttribArray(0);


			//Normals
			if (res->idNormals > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, res->idNormals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * res->numVertices * 3, res->normals, GL_STATIC_DRAW);

				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
				glEnableVertexAttribArray(1);
			}

			//UVs
			if (res->idUvs > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, res->idUvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * res->numVertices * 2, res->uvs, GL_STATIC_DRAW);

				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
				glEnableVertexAttribArray(2);
			}


			//Colors
			if (res->idColors> 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, res->idColors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * res->numVertices * 3, res->colors, GL_STATIC_DRAW);

				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)0);
				glEnableVertexAttribArray(3);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

		}
	}
}

UID ImporterMesh::SaveResource(ResourceMesh * res, Path& outputPath)
{
	//TODO: Save name?? But name is already saved into the resources file..

	uint ranges[5] = {
		res->numIndices,
		res->numVertices,
		(res->normals) ? res->numVertices : 0,
		(res->colors) ? res->numVertices : 0,
		(res->uvs) ? res->numVertices : 0
	};

	uint size = sizeof(ranges) + sizeof(uint) * res->numIndices + sizeof(float) * res->numVertices * 3;
	if (res->normals) size += sizeof(float) * res->numVertices * 3;
	if (res->colors) size += sizeof(float) * res->numVertices * 3;
	if (res->uvs) size += sizeof(float) * res->numVertices * 2;
	size += sizeof(AABB);

	//Allocate mem
	char* data = new char[size];
	char* cursor = data;

	//Ranges 
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	//Indices
	cursor += bytes;
	bytes = sizeof(uint) * res->numIndices;
	memcpy(cursor, res->indices, bytes);

	//Vertices
	cursor += bytes;
	bytes = sizeof(float) * res->numVertices * 3;
	memcpy(cursor, res->vertices, bytes);

	//Normals
	if (res->normals)
	{
		cursor += bytes;
		memcpy(cursor, res->normals, bytes);
	}

	//Colors
	if (res->colors)
	{
		cursor += bytes;
		memcpy(cursor, res->colors, bytes);
	}

	//UVs
	if (res->uvs)
	{
		cursor += bytes;
		bytes = sizeof(float) * res->numVertices * 2;
		memcpy(cursor, res->uvs, bytes);
	}

	//AABB
	cursor += bytes;
	bytes = sizeof(AABB);
	memcpy(cursor, &res->aabb, bytes);

	UID ret = app->resources->GetNewUID();
	outputPath.Set(MESH_SAVE_PATH, std::to_string(ret).c_str(), MESH_EXTENSION);

	if (app->fs->Save(outputPath.GetFullPath(), data, size) != size)
	{
		_LOG("ERRRO: Saving mesh!!");
		ret = 0;
	}

	RELEASE_ARRAY(data);

	return ret;
}

//-----------------------------------------------------------------------------------------------------

bool ImporterMesh::LoadCube(ResourceMesh * res)
{
	if (!res)return false;

	res->originalFile = "*Cube*";
	res->exportedFile = "*Cube*";
	res->name = "Cube";

	//-----------------------

	uint indices[6 * 6] = {
		0,  1,  2,  0,  2,  3,   //front
		4,  5,  6,  4,  6,  7,   //right
		8,  9,  10, 8,  10, 11,  //back
		12, 13, 14, 12, 14, 15,  //left
		16, 17, 18, 16, 18, 19,  //upper
		20, 21, 22, 20, 22, 23	 //bottom
	};

	res->numIndices = 6 * 6;
	res->indices = new uint[res->numIndices];
	memcpy(res->indices, indices, sizeof(uint) * res->numIndices);

	//-----------------------
	float s = 0.5f;

	float vertices[12 * 6] = {
		//Front
		-s, -s, s,	s, -s, s,	s, s, s,	-s, s, s,
		//Right
		s, s, s,	s, s, -s,	s, -s, -s,	s, -s, s,
		//Back
		-s, -s, -s,		s, -s, -s,	s, s, -s,	-s, s, -s,
		//Left
		-s, -s, -s,		-s, -s, s,	-s, s, s,	-s, s, -s,
		//Top
		s, s, s,	-s, s, s,	-s, s, -s,	 s, s, -s,
		//Bot
		-s, -s, -s,		s, -s, -s,	 s, -s, s,	 -s, -s, s
	};

	res->numVertices = 24;
	res->vertices = new float[res->numVertices * 3];
	memcpy(res->vertices, vertices, sizeof(float) * res->numVertices * 3);

	//-----------------------
	float u = 1.0f;

	float normals[12 * 6] = {
		//Front
		0.0f, 0.0f, u,		0.0f, 0.0f, u,		0.0f, 0.0f, u,		0.0f, 0.0f, u,
		//Right
		u, 0.0f, 0.0f,		u, 0.0f, 0.0f,		u, 0.0f, 0.0f,		u, 0.0f, 0.0f,
		//Back
		0.0f, 0.0f, -u,		0.0f, 0.0f, -u,		0.0f, 0.0f, -u,		0.0f, 0.0f, -u,
		//Left
		-u, 0.0f, 0.0f,		-u, 0.0f, 0.0f,		-u, 0.0f, 0.0f,		-u, 0.0f, 0.0f,
		//Top
		0.0f, u, 0.0f,		0.0f, u, 0.0f,		0.0f, u, 0.0f,		0.0f, u, 0.0f,
		//Bot
		0.0f, -u, 0.0f,		0.0f, -u, 0.0f,		0.0f, -u, 0.0f,		0.0f, -u, 0.0f
	};

	res->normals = new float[res->numVertices * 3];
	memcpy(res->normals, normals, sizeof(float) * res->numVertices * 3);

	//-----------------------
	float o = 0.0f;
	float uvs[8 * 6] = {
		//Front
		o, o,		u, o,	u, u,	o, u,
		//Right
		o, u,		u, u,	u, o,	o, o,
		//Back
		u, o,		o, o,	o, u,	u, u,
		//Left
		o, o,		u, o,	u, u,	o, u,
		//Top
		u, o,		o, o,	o, u,	u, u,
		//Bot
		u, u,		o, u,	o, o,	u, o
	};

	res->uvs = new float[res->numVertices * 2];
	memcpy(res->uvs, uvs, sizeof(float) * res->numVertices * 2);

	//-----------------------

	float colors[12 * 6] = {
		//Front		0
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		//Right		6
		0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		//Back		12
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		//Left		18
		1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f,
		//Top		24
		0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,
		//Bot		30
		1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 1.0f
	};

	res->colors = new float[res->numVertices * 3];
	memcpy(res->colors, colors, sizeof(float) * res->numVertices * 3);

	//-----------------------

	res->aabb = AABB(float3(-s, -s, -s), float3(s, s, s));

	//-----------------------

	GenBuffers(res);

	return true;
}

bool ImporterMesh::LoadQuad(ResourceMesh * res)
{
	if (!res) return false;

	res->originalFile = "*Quad*";
	res->exportedFile = "*Quad*";
	res->name = "Quad";

	//-----------------------

	uint indices[6] = {
		0,  1,  2,  0,  2,  3
	};

	res->numIndices = 6;
	res->indices = new uint[res->numIndices];
	memcpy(res->indices, indices, sizeof(uint) * res->numIndices);

	//-----------------------
	float s = 0.5f;

	float vertices[4 * 3] = {
		-s, -s, 0.0f,	s, -s, 0.0f,	s, s, 0.0f,		-s, s, 0.0f
	};

	res->numVertices = 4;
	res->vertices = new float[res->numVertices * 3];
	memcpy(res->vertices, vertices, sizeof(float) * res->numVertices * 3);

	//-----------------------
	float u = 1.0f;

	float normals[4 * 3] = {
		0.0f, 0.0f, u,		0.0f, 0.0f, u,		0.0f, 0.0f, u,		0.0f, 0.0f, u
	};

	res->normals = new float[res->numVertices * 3];
	memcpy(res->normals, normals, sizeof(float) * res->numVertices * 3);

	//-----------------------
	float o = 0.0f;
	float uvs[4 * 3] = {
		o, o,		u, o,	u, u,	o, u
	};

	res->uvs = new float[res->numVertices * 2];
	memcpy(res->uvs, uvs, sizeof(float) * res->numVertices * 2);

	//-----------------------

	float colors[4 * 3] = {
		0.7f, 0.7f, 0.7f,	0.7f, 0.7f, 0.7f,	0.7f, 0.7f, 0.7f,	0.7f, 0.7f, 0.7f
	};

	res->colors = new float[res->numVertices * 3];
	memcpy(res->colors, colors, sizeof(float) * res->numVertices * 3);

	//-----------------------

	res->aabb = AABB(float3(-s, -s, -s), float3(s, s, s));

	//-----------------------

	GenBuffers(res);

	return true;
}

bool ImporterMesh::LoadPlane(ResourceMesh * res) //TODO: Fix
{
	if (!res) return false;

	res->originalFile = "*Plane*";
	res->exportedFile = "*Plane*";
	res->name = "Plane";

	//-----------------------

	float length = 1.0f;
	float width = 1.0f;
	int resX = 2;
	int resZ = 2;

	res->numVertices = resX * resZ;
	res->vertices = new float[res->numVertices * 3];

	for (int z = 0; z < resZ; ++z)
	{
		float zPos = ((float)z / (resZ - 1) - 0.5f) * length;
		for (int x = 0; x < resX; ++x)
		{
			float xPos = ((float)x / (resX - 1) - 0.5f)*width;
			res->vertices[x + z + resX] = xPos;
			res->vertices[x + z + resX + 1] = 0.0f;
			res->vertices[x + z + resX + 2] = zPos;
		}
	}

	//-----------------------

	res->normals = new float[res->numVertices * 3];

	for (int i = 0; i < res->numVertices * 3; i += 3)
	{
		res->normals[i + 0] = 0.0f;
		res->normals[i + 1] = 1.0f;
		res->normals[i + 2] = 0.0f;
	}

	//-----------------------

	res->uvs = new float[res->numVertices * 2];

	for (int v = 0; v < resZ; ++v)
	{
		for (int u = 0; u < resX; ++u)
		{
			res->uvs[u + v * resX] = (float)u / (resX - 1);
			res->uvs[u + v * resX + 1] = (float)v / (resZ - 1);
		}
	}

	//-----------------------

	int nbFaces = (resX - 1) * (resZ - 1);
	res->numIndices = nbFaces * 6;
	res->indices = new uint[res->numIndices];

	int t = 0;
	for (int face = 0; face < nbFaces; ++face)
	{
		int i = face % (resX - 1) + (face / (resZ - 1) * resX);

		res->indices[t++] = i + resX;
		res->indices[t++] = i + 1;
		res->indices[t++] = i;

		res->indices[t++] = i + resX;
		res->indices[t++] = i + resX + 1;
		res->indices[t++] = i + 1;
	}

	//-----------------------

	res->colors = new float[res->numVertices * 3];

	for (int i = 0; i < res->numVertices * 3; ++i)
	{
		res->colors[i] = 0.7f;
	}

	//-----------------------

	//TODO: Calc AABB

	//-----------------------

	GenBuffers(res);

	return true;
}

bool ImporterMesh::LoadCone(ResourceMesh * res)
{
	if (!res) return false;

	res->originalFile = "*Cone*";
	res->exportedFile = "*Cone*";
	res->name = "Cone";

	return true;
}

bool ImporterMesh::LoadCylinder(ResourceMesh * res)
{
	if (!res) return false;

	res->originalFile = "*Cylinder*";
	res->exportedFile = "*Cylinder*";
	res->name = "Cylinder";

	return true;
}

bool ImporterMesh::LoadTorus(ResourceMesh * res)
{
	if (!res) return false;

	res->originalFile = "*Torus*";
	res->exportedFile = "*Torus*";
	res->name = "Torus";

	return true;
}

bool ImporterMesh::LoadSphere(ResourceMesh * res)
{
	if (!res) return false;

	res->originalFile = "*Sphere*";
	res->exportedFile = "*Sphere*";
	res->name = "Sphere";

	return true;
}
