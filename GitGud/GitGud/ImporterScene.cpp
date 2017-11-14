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
	_LOG(LOG_INFO, "Scene importer: Created.");

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}


ImporterScene::~ImporterScene()
{
	aiDetachAllLogStreams();
}

bool ImporterScene::Import(Path originalFile, Path& exportedFile, UID& resUID)
{
	bool ret = false;

	if (originalFile.Empty())
		return ret;

	char* buffer = nullptr;
	uint size = app->fs->Load(originalFile.GetFullPath(), &buffer);

	if (size > 0 && buffer)
	{
		const aiScene* scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, originalFile.GetExtension());
		RELEASE_ARRAY(buffer);

		if (scene && scene->HasMeshes())
		{
			JsonFile metaFile;
			metaFile.AddString("original_scene_file", originalFile.GetFullPath());

			//Import all meshes at once
			ImportMeshes(scene, originalFile, metaFile);
			//Import all materials at once
			ImportMaterials(scene, originalFile, metaFile);
			
			GameObject* go = app->goManager->CreateGameObject();
			go->SetName(originalFile.GetFileName());

			RecImport(scene, scene->mRootNode, go, originalFile, metaFile);

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
			exportedFile.Set(PREFABS_SAVE_PATH, std::to_string(resUID).c_str(), PREFAB_EXTENSION);

			ret = SaveScene(exportedFile, save, metaFile);

			//go->Destroy();	//TODO

			meshesImported.clear();
			materialsImported.clear();
		}
	}
	else
	{
		_LOG(LOG_ERROR, "Could not load scene: %s.", originalFile);
	}

	return ret;
}

void ImporterScene::RecImport(const aiScene * scene, const aiNode * node, GameObject * parent, Path& file, JsonFile& metaFile)
{
	/*for (uint i = 0; i < node->mMetaData->mNumProperties; ++i)
	{

		aiMetadataEntry* it = &node->mMetaData->mValues[i];
		void* data = it->mData;
		switch (it->mType)
		{
		case AI_BOOL:
			static_cast<bool*>(data);
			break;
		case AI_INT:
			static_cast<int*>(data);
			break;
		case AI_UINT64:
			static_cast<uint64_t*>(data);
			break;
		case AI_FLOAT:
			static_cast<float*>(data);
			break;
		case AI_AISTRING:
			static_cast<aiString*>(data);
			break;
		case AI_AIVECTOR3D:
			static_cast<aiVector3D*>(data);
			break;
		}
	}*/

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
			Material* cMat = (Material*)childGo->CreateComponent(CMP_MATERIAL);
			//cMat->SetResource(materialsImported[mesh->mMaterialIndex]);
		}

		Mesh* cMesh = (Mesh*)childGo->CreateComponent(CMP_MESH);
		cMesh->SetResource(meshesImported[node->mMeshes[i]]);

		//TODO: Resource instances??
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
		RecImport(scene, node->mChildren[i], go, file, metaFile);
	

}

void ImporterScene::ImportMeshes(const aiScene* scene, Path& file, JsonFile& metaFile)
{
	meshesImported.clear();
	
	for(int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		if(mesh) meshesImported.push_back(app->resources->ImportBuf(mesh, RES_MESH, 0, &file));
	}

	metaFile.AddUnsignedIntArray("meshes_ids", meshesImported.data(), meshesImported.size());
}

void ImporterScene::ImportMaterials(const aiScene* scene, Path& file, JsonFile& metaFile)
{
	materialsImported.clear();

	for(int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial* material = scene->mMaterials[i];
		if (material) materialsImported.push_back(app->resources->ImportBuf(material, RES_MATERIAL, 0, &file));
	}

	metaFile.AddUnsignedIntArray("materials_ids", materialsImported.data(), materialsImported.size());
}

void ImporterScene::ImportBones(const aiScene* scene, Path& file, JsonFile& metaFile)
{
	
}

void ImporterScene::ImportAnimations(const aiScene* scene, Path& file, JsonFile& metaFile)
{
	
}

bool ImporterScene::SaveScene(Path& path, JsonFile& scene, JsonFile& metaFile)
{
	bool ret = false;

	char* buffer = nullptr;
	uint size = scene.WriteJson(&buffer, false); // TODO: Set to true

	if (app->fs->Save(path.GetFullPath(), buffer, size) == size) ret = true;

	RELEASE_ARRAY(buffer);


	std::string metaPath = std::string(path.GetFullPath()) + std::string(".meta");
	uint metaSize = metaFile.WriteJson(&buffer, false); // TODO: Set to true
	if (app->fs->Save(metaPath.c_str(), buffer, metaSize) != metaSize) ret = false;

	RELEASE_ARRAY(buffer);

	return ret;
}

bool ImporterScene::LoadResource(Resource * resource)
{
	return false;
}
