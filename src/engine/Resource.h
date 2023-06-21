#ifndef _MYENGINE_RESOURCE_H_
#define _MYENGINE_RESOURCE_H_

#include "Defines.h"

#include <string>

namespace MYENGINE
{

	struct ResourceManager;

	struct Resource
	{
	public:
		Resource(weak(ResourceManager) resourceManager, std::string name) { m_resourceManager = resourceManager; m_name = name; }
		~Resource() {};

		virtual void Initialise() {};

		std::string GetName() { return m_name; }
		virtual unsigned int GetId() { return m_id; }

	protected:
		shared(ResourceManager) GetResourceManager() { return m_resourceManager.lock(); }

		std::string m_name;
		unsigned int m_id = -1;

	private:
		weak(ResourceManager) m_resourceManager;
	};

} // namespace MYENGINE

#endif
