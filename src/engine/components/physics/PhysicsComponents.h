#ifndef _MYENGINE_PHYSICSCOMPONENTS_H_
#define _MYENGINE_PHYSICSCOMPONENTS_H_

#include "engine/Defines.h"
#include "engine/Component.h"

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

#include <vector>
#include <string>

namespace MYENGINE
{

	struct GameObject;
	struct Transform;
	struct ColliderComponent;
	struct CollisionData;
	struct PhysicsManager;

	struct PhysicsComponent : public Component
	{
	public:
		PhysicsComponent(weak(GameObject) _parentObject, const char* _name, glm::vec3 _initialVelocity);

		/**
		* @returns the detached Transform component used by this PhysicsComponent.
		**/
		shared(Transform) GetLocalTransform() { return m_localTransform; }

		/**
		* @returns the PhysicsComponent's current velocity.
		**/
		glm::vec3 GetVelocity() { return m_velocity; }

		/**
		* @returns the PhysicsComponent's mass.
		**/
		float GetMass() { return m_mass; }

		/**
		* @returns the PhysicsComponent's previous local position.
		**/
		glm::vec3 GetPreviousLocalPosition() { return m_previousLocalPosition; }

		void (*m_OnCollisionCallbackFunc)(weak(CollisionData) _data, int _self) = nullptr;

	protected:
		friend PhysicsManager;

		shared(Transform) m_localTransform;
		weak(ColliderComponent) m_collider;

		glm::vec3 m_velocity;
		float m_mass;

		bool m_resting;

		glm::vec3 m_previousLocalPosition;

		void Initialise(weak(Component) _componentSelf) override;

		virtual void FixedUpdate(float _timestep, bool _updateLocal = true) { }
		virtual void HandleCollision(weak(CollisionData) _data, int _self);
	};

	struct DynamicPhysics : public PhysicsComponent
	{
	public:
		DynamicPhysics(weak(GameObject) _parentObject, int _numIntMethod, float _mass, float _friction, glm::vec3 _initialVelocity);

		/**
		* Set the DynamicPhysics component's 'ignore collisions' state.
		* 
		* @param _shouldIgnore: Whether this component should ignore collisions.
		**/
		void SetIgnoreCollisions(bool _shouldIgnore) { m_ignoreCollisions = _shouldIgnore; }

		/**
		* Change how gravity is applied to this DynamicPhysics component. 1=Earth gravity.
		*
		* @param _value: The value to multiply by Earth's gravity.
		**/
		void SetGravityMultiplier(float _value) { m_gravityMultiplier = _value; }

		/**
		* @returns the DynamicPhysics's rotation matrix.
		**/
		glm::mat3 GetRotationMatrix() { return m_r; }

		/**
		* Get the integration method used by this DynamicPhysics component. 0=Euler, 1=RK2, 2=RK4.
		* 
		* @returns the ID of this DynamicPhysics's integration method.
		**/
		int GetNumIntMethod() { return m_numIntMethod; }

		/**
		* Add a force to this DynamicPhysics component, to be used during the next physics timestep.
		* 
		* @param _force : The force to add on each axis, as a vec3.
		**/
		void AddForce(glm::vec3 _force);

		/**
		* Add torque to this DynamicPhysics component, to be used during the next physics timestep.
		*
		* @param _torque : The torque to add on each axis, as a vec3.
		**/
		void AddTorque(glm::vec3 _torque);

		std::vector<std::string> m_ignoreCollisionExceptionNames;

	protected:
		friend PhysicsManager;

		glm::vec3 m_force;

		glm::vec3 m_torque;
		glm::vec3 m_angularVelocity;
		glm::vec3 m_angularMomentum;

		glm::mat3 m_bodyInertiaTensorInverse;
		glm::mat3 m_inertiaTensorInverse;
		glm::mat3 m_r;

		glm::vec3 m_centerOfMass;
		float m_friction;

		int m_numIntMethod;

		bool m_ignoreCollisions;

		float m_gravityMultiplier;

		void ClearForces();
		void ClearTorque();

		void FixedUpdate(float _timestep, bool _updateLocal = true) override;
		void HandleCollision(weak(CollisionData) _data, int _self) override;

		void ComputeInverseInertiaTensor();

		void Euler(float _timestep, bool _updateLocal = true);
		void RK2(float _timestep, bool _updateLocal = true);
		void RK4(float _timestep, bool _updateLocal = true);
	};


	struct DummyPhysics : public PhysicsComponent
	{
	public:
		DummyPhysics(weak(GameObject) _parentObject, glm::vec3 _constantVelocity);

	private:
		void FixedUpdate(float _timestep, bool _updateLocal = true) override;
	};

} // namespace MYENGINE

#endif
