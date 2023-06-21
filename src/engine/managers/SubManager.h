#ifndef _MYENGINE_SUBMANAGER_H_
#define _MYENGINE_SUBMANAGER_H_

#include "engine/Defines.h"

namespace MYENGINE
{

	struct Manager;

	struct SubManager
	{
	public:
		SubManager(weak(Manager) manager) { m_manager = manager; }
		~SubManager() {}

	protected:
		/**
		* @returns the Manager instance
		**/
		shared(Manager) GetManager() { return m_manager.lock(); }

		/**
		* Initialise the SubManager.
		**/
		virtual void Initialise() {  }

	private:
		weak(Manager) m_manager;
	};

} // namespace MYENGINE

#endif
