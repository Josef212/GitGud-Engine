#include "Component.h"

#include "GameObject.h"

Component::Component(GameObject * object, COMPONENT_TYPE type) : object(object), type(type)
{
}

Component::~Component()
{
}

/** Component - IsActive: Return true if GO is active, false if not. */
bool Component::IsActive() const
{
	return selfActive;
}

/** Component - SetActive: Set the GO active to true or false. */
void Component::SetActive(bool set)
{
	if (set != selfActive)
	{
		if (set) Enable();
		else Disable();
	}
}

/** Component - SwitchActive: Swaps the GO active state. */
void Component::SwitchActive()
{
	if (selfActive) Disable();
	else Enable();
}

/** Component - Enable: Sets the GO to active. */
void Component::Enable()
{
	if (!selfActive)
	{
		selfActive = true;
		OnEnable();
	}
}

/** Component - Disable: Sets the GO to active off. */
void Component::Disable()
{
	if (selfActive)
	{
		selfActive = false;
		OnDisable();
	}
}

/** Component - GetType: Return the component type. */
COMPONENT_TYPE Component::GetType() const
{
	return type;
}

/** Component - GetGameObject: Return the linked GO. */
GameObject * Component::GetGameObject() const
{
	return object;
}

/** Component - Destroy: Mark the component to delete it on a save moment. */
void Component::Destroy()
{
	object->RemoveComponent(this);
}
