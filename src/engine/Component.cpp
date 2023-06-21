#include "Component.h"

#include "engine/GameObject.h"

#include <iostream>

namespace MYENGINE
{

	Component::Component(weak(GameObject) _parentObject, const char* _name)
	{
		m_parentObject = _parentObject;
		m_name = _name;
	}

	void Component::Initialise(weak(Component) _componentSelf)
	{
		m_self = _componentSelf;
	}

	Component::~Component()
	{
		//std::cout << "  >Component '" << m_name << "' destructed!" << std::endl;
	}

	shared(Manager) Component::GetManager()
	{
		if (shared(GameObject) obj = GetObject())
		{
			return obj->GetManager();
		}
		else
		{
			return nullptr;
		}
	}

} // namespace MYENGINE
