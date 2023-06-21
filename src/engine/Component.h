#ifndef _MYENGINE_COMPONENT_H_
#define _MYENGINE_COMPONENT_H_

#include "engine/Defines.h"

namespace MYENGINE
{

	struct GameObject;
	struct Manager;

	struct Component
	{
	public:
		Component(weak(GameObject) _parentObject, const char* _name);
		~Component();

		virtual void Blank() { };

		/**
		* @returns the name of the Component type.
		**/
		const char* GetName() { return m_name; }

		/**
		* @returns the GameObject this Component is attached to.
		**/
		shared(GameObject) GetObject() { return m_parentObject.lock(); }

		/**
		* @returns the Manager instance this Component is indirectly managed by.
		**/
		shared(Manager) GetManager();

	protected:
		friend struct SceneLoader;
		friend GameObject;

		//virtual void ObjectLoaded() {}; // After every object has been loaded from a scene, this function is called
		virtual void ObjectFirstUpdate() {}; // Called immediately before the object's first update

		virtual void UpdateEarly() {};

		/**
		* This function is called on all Components once per frame, and is often overriden by child classes.
		**/
		virtual void Update() {};

		/**
		* This function is called on all Components once per frame, after all Components have had their Update() functions called. It is often overriden by child classes, but it should ONLY be used for components implementing GUI functionality.
		**/
		virtual void UpdateGUI() {};

		/**
		* This function is called when the Component is added to a GameObject.
		* 
		* @param _componentSelf : a weak reference of itself.
		**/
		virtual void Initialise(weak(Component) _componentSelf);

		const char* m_name = "COMPONENT_BASE";
		weak(GameObject) m_parentObject;
		weak(Component) m_self;
	};

} // namespace MYENGINE

#endif
