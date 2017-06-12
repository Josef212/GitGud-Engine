#include "Mesh.h"

#include "App.h"
#include "GameObject.h"
#include "ResourceMesh.h"
#include "M_ResourceManager.h"


Mesh::Mesh(GameObject* object) : Component(object, CMP_MESH)
{
}


Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::GetBox(AABB & box) const
{
	ResourceMesh* r = (ResourceMesh*)app->resources->GetResourceFromUID(resource);
	if (r && r->vertices)
		box.Enclose(r->aabb);
}

void Mesh::ClearMesh()
{
	//TODO:
}

void Mesh::OnSaveCmp(JsonFile & sect) const
{
	if (resource)
	{
		sect.AddBool("have_res", true);
		ComponentResource::OnSaveRes(sect);
	}
	else
	{
		sect.AddBool("have_res", false);
	}

	sect.AddBool("wireframe", renderWirefreame);
	sect.AddBool("normals", renderNormals);
}

void Mesh::OnLoadCmp(JsonFile * sect)
{
	if (sect)
	{
		selfActive = sect->GetBool("active", true);

		if (sect->GetBool("have_res", false))
			ComponentResource::OnLoadRes(sect);

		renderWirefreame = sect->GetBool("wireframe", false);
		renderNormals = sect->GetBool("normals", false);
	}
}

//-------------------------------------

bool Mesh::SetResource(UID resUID)
{
	bool ret = false;

	if (resUID != 0)
	{
		Resource* res = app->resources->GetResourceFromUID(resUID);
		if (res && res->GetType() == RES_MESH)
		{
			if (res->LoadToMemory())
			{
				resource = resUID;
				object->RecalcBox();
				ret = true;
			}
		}
	}

	return ret;
}
