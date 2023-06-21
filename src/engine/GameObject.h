#ifndef _MYENGINE_GAMEOBJECT_H_
#define _MYENGINE_GAMEOBJECT_H_

#include "engine/Defines.h"

#include <vector>
#include <string>

typedef unsigned int GLuint;

namespace MYENGINE
{

	struct Manager;
	struct Component;
	struct Transform;
	struct Camera;
	struct ColliderComponent;
	struct PhysicsComponent;
	struct SceneLoader;

	struct GameObject
	{
	public:
		GameObject(const char* _name, weak(Manager) _manager, std::string _parentObjectName = "NULL");
		~GameObject();

		/**
		* Set the given GameObject to act as a parent for this GameObject.
		*
		* @param _newParent : the new parent GameObject.
		**/
		void SetParentGameObject(shared(GameObject) _newParent);

		/**
		* @returns the name of this GameObject.
		**/
		std::string GetName() { return m_name; }

		/*
		* Add a tag to this GameObject.
		* 
		* @param _newTag : The new tag to add.
		* @returns true if the tag was added, or false if it was already present.
		*/
		bool AddTag(std::string _newTag);

		/*
		* Remove a tag from this GameObject.
		*
		* @param _tagToRemove : The tag to remove.
		* @returns true if the tag was removed, or false if it was not present.
		*/
		bool RemoveTag(std::string _tagToRemove);

		/*
		* Test for a tag possessed by this GameObject.
		*
		* @param _tag : The tag to test for.
		* @returns whether this GameObject possesses the given tag.
		*/
		bool HasTag(std::string _tag);

		/**
		* Register a new Component with the GameObject.
		*
		* @param _component : The Component reference to register.
		**/
		void AddComponent(shared(Component) _component);

		/**
		* @returns the number of Components owned by this GameObject.
		**/
		int GetComponentCount() { return (int)m_components.size(); }

		/**
		* Get the GameObject set as this GameObject's parent.
		*
		* @returns the parent GameObject.
		**/
		shared(GameObject) GetParentGameObject() { return m_parentObject.lock(); }

		/**
		* Get the GameObjects set as this GameObject's children.
		*
		* @returns a vector of the child GameObjects.
		**/
		std::vector<weak(GameObject)> GetChildGameObjects() { return m_childObjects; }
		
		/**
		* Retrieve the first Component owned by this GameObject of type T.
		* 
		* @param T : The Component type to find.
		* @returns the found Component, else nullptr.
		**/
		template <class T> shared(T) GetComponent()
		{
			for (int c = 0; c < GetComponentCount(); c++)
			{
				shared(T) casted = std::dynamic_pointer_cast<T>(m_components.at(c));
				if (casted)
				{
					return casted;
				}
			}
			
			return nullptr;
		}

		/**
		* Retrieve all Components owned by this GameObject of type T.
		*
		* @param T : The Component type to find.
		* @returns a vector of all found Components.
		**/
		template <class T> std::vector<shared(T)> GetComponents()
		{
			std::vector<shared(T)> result = {};
			for (int c = 0; c < GetComponentCount(); c++)
			{
				shared(T) casted = std::dynamic_pointer_cast<T>(m_components.at(c));
				if (casted)
				{
					result.push_back(casted);
				}
			}

			return result;
		}

		/**
		* @returns the first PhysicsComponent attached to this GameObject, else nullptr.
		**/
		shared(PhysicsComponent) GetPhysics();

		/**
		* @returns the first ColliderComponent attached to this GameObject, else nullptr.
		**/
		shared(ColliderComponent) GetCollider();

		/**
		* @returns the first Camera component attached to this GameObject, else nullptr.
		**/
		shared(Camera) GetCamera();

		/**
		* @returns the Manager instance this GameObject is managed by.
		**/
		shared(Manager) GetManager() { return m_manager.lock(); }

		/**
		* Register a new Transform as the primary Transform of this GameObject.
		*
		* @param _transform : The new Transform to register.
		**/
		void SetTransform(weak(Transform) _transform);

		/**
		* @returns the GameObject's primary Transform component, else nullptr.
		**/
		shared(Transform) GetTransform() { return m_transform.lock(); }

	private:
		friend Manager;
		friend SceneLoader;

		std::string m_name;
		bool m_decaying;
		std::vector<std::string> m_tags;

		/**
		* Register a GameObject as a child of this GameObject.
		*
		* @param _newChild : the new child GameObject.
		**/
		void AdoptChildGameObject(shared(GameObject) _newChild);

		/**
		* Remove the given GameObject from the list of this GameObject's children.
		*
		* @param _oldChild : the child GameObject to forget.
		**/
		void ForgetChildGameObject(shared(GameObject) _oldChild);

		weak(GameObject) m_parentObject;
		std::vector<weak(GameObject)> m_childObjects = {};

		void Initialise(shared(GameObject) _self);
		void UpdateEarly();
		void Update();
		void UpdateGUI();
		void Draw();
		void Draw(weak(Camera) _camera, GLuint _shaderProgramGLID);

		bool m_noUpdates;

		shared(Component) GetComponent(int _pos) { return m_components.at(_pos); }
		std::vector<shared(Component)> m_components;

		weak(Manager) m_manager;
		weak(Transform) m_transform;
		weak(GameObject) m_self;
	};

} // namespace MYENGINE

#endif
