#include "ImporterScene.h"

#include "App.h"
#include "M_FileSystem.h"
#include "JsonFile.h"

#include "M_ResourceManager.h"
#include "M_GoManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"
#include "ResourceScene.h"

#include <cimport.h>
#include <scene.h>
#include <postprocess.h>
#include <cfileio.h>

ImporterScene::ImporterScene()
{
	_LOG("Scene importer: Created.");

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}


ImporterScene::~ImporterScene()
{
	aiDetachAllLogStreams();
}

bool ImporterScene::Import(const char * originalFile, std::string & exportedFile, const char * originalFileExt, UID & resUID)
{
	bool ret = false;

	if (!originalFile || !originalFileExt)
		return ret;
	//TODO: Paths??
	char* buffer = nullptr;
	uint size = app->fs->Load(originalFile, &buffer);

	if (size > 0 && buffer)
	{
		const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, originalFileExt);
		RELEASE_ARRAY(buffer);

		if (scene && scene->HasMeshes())
		{
			std::string path, file;
			app->fs->SplitPath(originalFile, &path, &file);

			meshesImported.clear();

			GameObject* go = app->goManager->CreateGameObject();
			go->SetName(file.c_str());

			RecImport(scene, scene->mRootNode, go, path, file);

			//TODO: bones

			//TODO: Animations

			aiReleaseImport(scene);

			JsonFile save;
			save.AddArray("game_objects");

			for (auto child : go->childs)
			{
				go->OnSaveGo(save);
			}

			resUID = app->resources->GetNewUID();

			exportedFile = (std::to_string(resUID) + PREFAB_EXTENSION);

			char* buff = nullptr;
			uint s = save.WriteJson(&buff, false); //TODO: Fast

			if (app->fs->Save((PREFABS_SAVE_PATH + exportedFile).c_str(), buff, s) == s)
				ret = true;

			RELEASE_ARRAY(buff);
			go->Destroy();
		}
	}
	else
	{
		_LOG("ERROR: Could not load scene: %s.", originalFile);
	}

	return ret;
}

void ImporterScene::RecImport(const aiScene * scene, const aiNode * node, GameObject * parent, const std::string & basePath, const std::string & file)
{
	static std::string name;
	name = (node->mName.length > 0) ? node->mName.C_Str() : "unnamed";

	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scl(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	// Name analysis to handle FBX dummy nodes
	// check bottom of http://g3d.cs.williams.edu/g3d/G3D10/assimp.lib/code/FBXImportSettings.h
	static const char* dummies[5] = {
		"$AssimpFbx$_PreRotation", "$AssimpFbx$_Rotation", "$AssimpFbx$_PostRotation",
		"$AssimpFbx$_Scaling", "$AssimpFbx$_Translation" };

	for (int i = 0; i < 5; ++i)
	{
		if (name.find(dummies[i]) != std::string::npos && node->mNumChildren == 1)
		{
			node = node->mChildren[0];

			node->mTransformation.Decompose(scaling, rotation, translation);

			pos += float3(translation.x, translation.y, translation.z);
			scl += float3(scaling.x, scaling.y, scaling.z);
			rot = rot * Quat(rotation.x, rotation.y, rotation.z, rotation.w);

			name = node->mName.C_Str();
			i = -1;
		}
	}

	GameObject* go = parent->CreateChild();
	go->SetName(name.c_str());

	Transform* trans = go->transform;

	trans->SetLocalPosition(pos);
	trans->SetLocalScale(scl);
	trans->SetLocalRotation(rot);

	//TODO: Load metadata

	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		GameObject* childGo = nullptr;

		if (node->mNumMeshes > 1)
		{
			name = mesh->mName.C_Str();
			if (name.length() == 0)
			{
				name = node->mName.C_Str();
				name += "_submesh";
			}
			childGo = go->CreateChild();
			childGo->SetName(name.c_str());
		}
		else
		{
			childGo = go;
		}

		//TODO: Material
		if (scene->HasMaterials())
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			Material* cMat = (Material*)childGo->CreateComponent(CMP_MATERIAL);
			cMat->SetResource(app->resources->ImportBuf(mat, 0, RES_MATERIAL, (basePath).c_str()));
		}

		Mesh* cMesh = (Mesh*)childGo->CreateComponent(CMP_MESH);
		cMesh->SetResource(app->resources->ImportBuf(mesh, 0, RES_MESH, (basePath + file).c_str()));
		//TODO: Resource instances??
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
		RecImport(scene, node->mChildren[i], go, basePath, file);
	

}

bool ImporterScene::LoadResource(Resource * resource)
{
	return false;
}