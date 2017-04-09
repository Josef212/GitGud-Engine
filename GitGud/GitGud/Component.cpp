#include "Component.h"

#include "GameObject.h"

Component::Component(GameObject * object, COMPONENT_TYPE type) : object(object), type(type)
{
}

Component::~Component()
{
}

bool Component::IsActive() const
{
	return selfActive;
}

void Component::SetActive(bool set)
{
	if (set != selfActive)
	{
		if (set) Enable();
		else Disable();
	}
}

void Component::SwitchActive()
{
	if (selfActive) Disable();
	else Enable();
}

void Component::Enable()
{
	if (!selfActive)
	{
		selfActive = true;
		OnEnable();
	}
}

void Component::Disable()
{
	if (selfActive)
	{
		selfActive = false;
		OnDisable();
	}
}

COMPONENT_TYPE Component::GetType() const
{
	return type;
}

GameObject * Component::GetGameObject() const
{
	return object;
}

void Component::Destroy()
{
	object->RemoveComponent(this);
}
