#include "ImporterMesh.h"



ImporterMesh::ImporterMesh() : Importer()
{
	_LOG("Mesh importer: Created.");
}


ImporterMesh::~ImporterMesh()
{
}

bool ImporterMesh::LoadResource(Resource * resource)
{
	return false;
}
