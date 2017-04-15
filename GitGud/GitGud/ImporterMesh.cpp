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

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}


ImporterMesh::~ImporterMesh()
{
	aiDetachAllLogStreams();
}

bool ImporterMesh::ImportMesh(const aiMesh * mesh, std::string & output, UID & id)
{
	if (!mesh) return false;

	ResourceMesh m(0);

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
	if(!resource || resource->GetType() != RES_MESH || resource->exportedFile.empty())
		return false;

	bool ret = false;

	ResourceMesh* res = (ResourceMesh*)resource;

	char* buffer = nullptr;
	uint size = app->fs->Load((MESH_SAVE_PATH + res->exportedFile).c_str(), &buffer);

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

		//TODO: Gen gl buffers
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

			//Indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->idIndices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * res->numIndices, res->indices, GL_STATIC_DRAW);

			//Vertices
			glBindBuffer(GL_ARRAY_BUFFER, res->idVertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * res->numVertices * 3, res->vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
			glEnableVertexAttribArray(0);


			//Normals
			if (res->idNormals > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, res->idNormals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * res->numVertices * 3, res->normals, GL_STATIC_DRAW);

				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);
			}

			//UVs
			if (res->idUvs > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, res->idUvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * res->numVertices * 2, res->uvs, GL_STATIC_DRAW);

				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(6 * sizeof(float)));
				glEnableVertexAttribArray(2);
			}


			//Colors
			if (res->idColors> 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, res->idColors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * res->numVertices * 3, res->colors, GL_STATIC_DRAW);

				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * sizeof(float)));
				glEnableVertexAttribArray(3);
			}

			glBindVertexArray(0);

		}
	}
}

UID ImporterMesh::SaveResource(ResourceMesh * res, std::string& outputName)
{
	uint ranges[5] = {
		res->numIndices,
		res->numVertices,
		(res->normals) ? res->numVertices : 0,
		(res->colors) ? res->numVertices : 0,
		(res->uvs) ? res->numVertices : 0,
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
	outputName.assign(std::to_string(ret) + MESH_EXTENSION);

	if (app->fs->Save((MESH_SAVE_PATH + outputName).c_str(), data, size) != size)
	{
		_LOG("ERRRO: Saving mesh!!");
		ret = 0;
	}

	RELEASE_ARRAY(data);

	return ret;
}

bool ImporterMesh::LoadCube(ResourceMesh * res)
{
	if (!res)return false;

	res->originalFile = "*Cube*";
	res->exportedFile = "*Cube*";

	//-----------------------

	uint indices[6 * 6] = {
		//Bottom
		3, 1, 0,	3, 2, 1,
		//Left
		3 + 4 * 1, 1 + 4 * 1, 0 + 4 * 1,
		3 + 4 * 1, 2 + 4 * 1, 1 + 4 * 1,

		// Front
		3 + 4 * 2, 1 + 4 * 2, 0 + 4 * 2,
		3 + 4 * 2, 2 + 4 * 2, 1 + 4 * 2,

		// Back
		3 + 4 * 3, 1 + 4 * 3, 0 + 4 * 3,
		3 + 4 * 3, 2 + 4 * 3, 1 + 4 * 3,

		// Right
		3 + 4 * 4, 1 + 4 * 4, 0 + 4 * 4,
		3 + 4 * 4, 2 + 4 * 4, 1 + 4 * 4,

		// Top
		3 + 4 * 5, 1 + 4 * 5, 0 + 4 * 5,
		3 + 4 * 5, 2 + 4 * 5, 1 + 4 * 5
	};

	res->numIndices = 6 * 6;
	res->indices = new uint[res->numIndices];
	memcpy(res->indices, indices, sizeof(uint) * res->numIndices);

	//-----------------------
	float s = 0.5f;

	float vertices[12 * 6] = {
		//Bottom
		-s, -s, s,   s, -s, s,   s, -s, -s,   -s, -s, -s,
		//Left
		-s, s, -s,   -s, s, s,   -s, -s, s,   -s, -s, -s,
		//Front
		-s, s, s,   s, s, s,   s, -s, s,   -s, -s, s,
		//Back
		s, s, -s,   -s, s, -s,   -s, -s, -s,   s, -s, -s,
		//Right
		s, s, s,   s, s, -s,   s, -s,   -s, s, -s, s,
		//Top
		-s, s, -s,   s, s, -s,   s, s, s,   -s, s, s
	};

	res->numVertices = 24;
	res->vertices = new float[res->numVertices * 3];
	memcpy(res->vertices, vertices, sizeof(float) * res->numVertices * 3);

	//-----------------------
	float u = 1.0f;

	float normals[12 * 6] = {
		//Bottom
		0.0f, -u, 0.0f,		0.0f, -u, 0.0f,		0.0f, -u, 0.0f,		0.0f, -u, 0.0f,
		//Left
		-u, 0.0f, 0.0f,		-u, 0.0f, 0.0f,		-u, 0.0f, 0.0f,		-u, 0.0f, 0.0f,
		//Front
		0.0f, 0.0f, u,		0.0f, 0.0f, u,		0.0f, 0.0f, u,		0.0f, 0.0f, u,
		//Back
		0.0f, 0.0f, -u,		0.0f, 0.0f, -u,		0.0f, 0.0f, -u,		0.0f, 0.0f, -u,
		//Right
		u, 0.0f, 0.0f,		u, 0.0f, 0.0f,		u, 0.0f, 0.0f,		u, 0.0f, 0.0f,
		//Top
		0.0f, u, 0.0f,		0.0f, u, 0.0f,		0.0f, u, 0.0f,		0.0f, u, 0.0f,
	};

	res->normals = new float[res->numVertices * 3];
	memcpy(res->normals, normals, sizeof(float) * res->numVertices * 3);

	//-----------------------

	float uvs[8 * 6] = {
		//Bottom
		u, u,	0.f, u,		0.0f, 0.0f,		u, 0.0f,
		//Left
		u, u,	0.f, u,		0.0f, 0.0f,		u, 0.0f,
		//Front
		u, u,	0.f, u,		0.0f, 0.0f,		u, 0.0f,
		//Back
		u, u,	0.f, u,		0.0f, 0.0f,		u, 0.0f,
		//Right
		u, u,	0.f, u,		0.0f, 0.0f,		u, 0.0f,
		//Top
		u, u,	0.f, u,		0.0f, 0.0f,		u, 0.0f,
	};

	res->uvs = new float[res->numVertices * 2];
	memcpy(res->uvs, uvs, sizeof(float) * res->numVertices * 2);

	//-----------------------

	float colors[12 * 6] = {
		//Bottom
		0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,
		//Left
		0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,
		//Front
		0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,
		//Back
		0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,
		//Right
		0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,
		//Top
		0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f,	0.51f, 0.51f, 0.51f
	};

	res->colors = new float[res->numVertices * 3];
	memcpy(res->colors, colors, sizeof(float) * res->numVertices * 3);

	//-----------------------

	res->aabb = AABB(float3(-0.5f, -0.5f, -0.5f), float3(0.5f, 0.5, 0.5));

	//-----------------------

	GenBuffers(res);

	return true;
}
