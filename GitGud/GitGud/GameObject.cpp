#include "GameObject.h"

#include "App.h"
#include "M_GoManager.h"
#include "RandGen.h"
#include "JsonFile.h"

GameObject::GameObject(GameObject * parent, UID uuid) : parent(parent), uuid(uuid)
{
	name.assign("GameObject");
	//TODO: Create transform on creation
}

GameObject::~GameObject()
{
	for (auto cmp : components)
		RELEASE(cmp);

	for (auto obj : childs)
		RELEASE(obj);
}

UID GameObject::GetUuid() const
{
	return uuid;
}

const char * GameObject::GetName() const
{
	return name.c_str();
}

void GameObject::SetName(const char * str)
{
	if (str)name.assign(str);
}

GameObject * GameObject::CreateChild()
{
	GameObject* ret = nullptr;

	ret = new GameObject(this, app->random->GetRandInt());
	childs.push_back(ret);

	return ret;
}

Component * GameObject::GetComponent(COMPONENT_TYPE type)
{
	for (auto cmp : components)
	{
		if (cmp->GetType() == type)
			return cmp;
	}
	return nullptr;
}

std::vector<Component*> GameObject::GetComponents(COMPONENT_TYPE types)
{
	std::vector<Component*> ret;

	for (auto cmp : components)
	{
		if (types & cmp->GetType())
			ret.push_back(cmp);
	}

	return ret;
}

bool GameObject::HasComponent(COMPONENT_TYPE type)
{
	for (auto cmp : components)
	{
		if (cmp->GetType() == type)
			return true;
	}
	return false;
}

uint GameObject::CountComponents(COMPONENT_TYPE type)
{
	uint ret = 0;

	for (auto cmp : components)
	{
		if (cmp->GetType() == type)
			++ret;
	}

	return ret;
}

void GameObject::RemoveChild(GameObject * obj)
{
	if (obj)
	{
		std::vector<GameObject*>::iterator it = std::find(childs.begin(), childs.end(), obj);
		if (it != childs.end())
			childs.erase(it);
	}
}

void GameObject::RemoveAllChilds()
{
	for (auto go : childs)
	{
		app->goManager->FastRemoveGameObject(go);
	}
	childs.clear();
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::Destroy()
{
	app->goManager->RemoveGameObject(this);
}

void GameObject::RemoveComponent(Component * cmp)
{
	componentsToRemove.push_back(cmp);
}

bool GameObject::IsStatic() const
{
	return isStatic;
}

void GameObject::SetStatic(bool set)
{
	if (set != isStatic)
	{
		if (set)
		{
			app->goManager->EraseDynObj(this);
			app->goManager->InsertToTree(this);
		}
		else
		{
			app->goManager->EraseFromTree(this);
			app->goManager->AddDynObject(this);
		}
		isStatic = set;

		for (auto go : childs)
		{
			go->SetStatic(set);
		}
	}
}

bool GameObject::IsActive() const
{
	return selfActive;
}

void GameObject::SetActive(bool set)
{
	if (set != selfActive)
	{
		if (set) Enable();
		else Disable();
	}
}

void GameObject::SwitchActive()
{
	if (selfActive) Disable();
	else Enable();
}

void GameObject::Enable()
{
	if (!selfActive)
	{
		selfActive = true;
		OnEnable();
	}
}

void GameObject::Disable()
{
	if (selfActive)
	{
		selfActive = false;
		OnDisable();
	}
}

void GameObject::PreUpdate()
{
	for (auto cmp : componentsToRemove)
	{
		std::vector<Component*>::iterator it = std::find(components.begin(), components.end(), cmp);
		if (it != components.end())
		{
			components.erase(it);
			cmp->OnFinish();
			RELEASE(cmp);
		}
	}
	componentsToRemove.clear();
}

void GameObject::Update(float dt)
{
	for (auto cmp : components)
		if(cmp->IsActive())
			cmp->OnPreUpdate(dt);

	for (auto cmp : components)
		if (cmp->IsActive())
			cmp->OnUpdate(dt);
}

void GameObject::OnStart()
{
	for (auto cmp : components)
	{
		cmp->OnStart();
	}
}

void GameObject::OnFinish()
{
	if (isStatic)
	{
		app->goManager->EraseFromTree(this);
	}
	else
	{
		app->goManager->EraseDynObj(this);
	}

	for (auto cmp : components)
	{
		cmp->OnFinish();
	}
}

void GameObject::OnEnable()
{
	for (auto cmp : components)
	{
		cmp->OnEnable();
	}

	for (auto go : childs)
	{
		go->Enable();
	}
}

void GameObject::OnDisable()
{
	for (auto cmp : components)
	{
		cmp->OnDisable();
	}

	for (auto go : childs)
	{
		go->Disable();
	}
}

void GameObject::OnPlay()
{
	for (auto cmp : components)
	{
		cmp->OnPlay();
	}
}

void GameObject::OnStop()
{
	for (auto cmp : components)
	{
		cmp->OnStop();
	}
}

void GameObject::OnPause()
{
	for (auto cmp : components)
	{
		cmp->OnPause();
	}
}

void GameObject::OnUnPause()
{
	for (auto cmp : components)
	{
		cmp->OnUnPause();
	}
}

void GameObject::OnGameObjectDestroyed()
{
	for (auto cmp : components)
	{
		cmp->OnGameObjectDestroyed();
	}
}

void GameObject::RecCalcTransform(const float4x4 & parentTrans, bool force)
{
	//TODO
	/*if (transform && transform->localHasChanged || forced)
	{
		if (isStatic)
			SetStatic(false);

		force = true;
		wasDirty = true;
		transform->UpdateTransform(parentTrans);

		for (auto cmp : components)
		{
			cmp->OnTransformUpdate(transform);
		}
	}
	else
	{
		wasDirty = false;
	}

	for (auto go : childs)
	{
		if (go && transform)
		{
			go->RecCalcTransform(transform->GetGlobalTransform(), force);
		}
	}*/
}

void GameObject::RecCalcBoxes()
{
	//TODO
	/*if (wasDirty)
	{
		RecalcBox();

		OBB o = enclosingBox;
		if (o.IsFinite() && transform)
		{
			o.Transform(transform->GetGlobalTransform());
			enclosingBox.SetFrom(o);
		}
	}

	for (auto go : childs)
	{
		if (go && transform)
			go->RecCalcBoxes();
	}*/
}

void GameObject::RecalcBox()
{
	enclosingBox.SetNegativeInfinity();

	for (auto cmp : components)
	{
		if (cmp->IsActive())
			cmp->GetBox(enclosingBox);
	}
}

bool GameObject::OnSaveGo(JsonFile & sect) const
{
	return false;
}

bool GameObject::OnLoadGo(JsonFile * sect)
{
	return false;
}
