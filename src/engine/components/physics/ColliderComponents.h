#ifndef _MYENGINE_COLLIDERCOMPONENTS_H_
#define _MYENGINE_COLLIDERCOMPONENTS_H_

#include "engine/Defines.h"
#include "engine/Component.h"

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

#include <vector>

namespace MYENGINE
{

	struct SphereCollider;
	struct BoxCollider;
	struct PlaneCollider;


	struct GameObject;
	struct Transform;
	struct CollisionData;
	struct PhysicsComponent;


	struct ColliderComponent : public Component
	{
	public:
		ColliderComponent(weak(GameObject) _parentObject, const char* _name, float _bounciness, bool _rotatable);

		/**
		* @returns the PhysicsComponent attached to this ColliderComponent's GameObject.
		**/
		shared(PhysicsComponent) GetPhysicsComponent() { return m_physicsComponent.lock(); }

		/**
		* @returns whether this ColliderComponent can be rotated.
		**/
		bool Rotatable() { return m_rotatable; }

		/**
		* @returns the inverse body inertia tensor.
		**/
		virtual glm::mat3 GetInverseBodyInertiaTensor(float _mass) { return glm::mat3(0.0f); }

		/**
		* @returns the ColliderComponent's 'bounciness' value.
		**/
		float GetBounciness() { return m_bounciness; }

		/**
		* @returns the ColliderComponent's friction multiplier.
		**/
		float GetFrictionMultiplier() { return m_frictionMultiplier; }

		/**
		* Get the surface normal of this ColliderComponent at the given point
		* 
		* @param _point : The point to query.
		* @returns the surface normal.
		**/
		virtual glm::vec3 GetSurfaceNormalAtPoint(glm::vec3 _point) { return glm::vec3(1.0f); }

		/**
		* Check for a collision between this ColliderComponent and _otherCollider.
		* 
		* @param _otherCollider : A reference to the other ColliderComponent to check against.
		* @param _collisionData : A structure to be written into with information about the collision, if there was one.
		* @returns whether there was a collision.
		**/
		bool CollidingWith(weak(ColliderComponent) _otherCollider, weak(CollisionData) _collisionData);

		/**
		* A vector of all collisions occuring this frame.
		**/
		std::vector<shared(CollisionData)> m_collisions;

	protected:
		void Initialise(weak(Component) _componentSelf) override;

		weak(PhysicsComponent) m_physicsComponent;

		bool m_rotatable;
		float m_bounciness;
		float m_frictionMultiplier;

		bool m_active;

		virtual bool CollidingWith(weak(PlaneCollider) _otherCollider, weak(CollisionData) _collisionData) { return false; };
		virtual bool CollidingWith(weak(SphereCollider) _otherCollider, weak(CollisionData) _collisionData) { return false; };
		virtual bool CollidingWith(weak(BoxCollider) _otherCollider, weak(CollisionData) _collisionData) { return false; };
	};


	struct PlaneCollider : public ColliderComponent
	{
	public:
		PlaneCollider(weak(GameObject) _parentObject, glm::vec3 _normal);

		/**
		* @returns the PlaneCollider's surface normal.
		**/
		glm::vec3 GetNormal() { return m_normal; }
		glm::vec3 GetSurfaceNormalAtPoint(glm::vec3 _point) override;

	protected:
		glm::vec3 m_normal;

		bool CollidingWith(weak(PlaneCollider) _otherCollider, weak(CollisionData) _collisionData) override { return false; };
		bool CollidingWith(weak(SphereCollider) _otherCollider, weak(CollisionData) _collisionData) override { return false; };
		bool CollidingWith(weak(BoxCollider) _otherCollider, weak(CollisionData) _collisionData) override { return false; };
	};

	struct SphereCollider : public ColliderComponent
	{
	public:
		SphereCollider(weak(GameObject) _parentObject, float _radius, float _bounciness);

		/**
		* @returns the SphereCollider's radius.
		**/
		float GetRadius() { return m_radius; }
		glm::vec3 GetSurfaceNormalAtPoint(glm::vec3 _point) override;

		glm::mat3 GetInverseBodyInertiaTensor(float _mass) override;

	protected:
		float m_radius;

		bool CollidingWith(weak(PlaneCollider) _otherCollider, weak(CollisionData) _collisionData) override;
		bool CollidingWith(weak(SphereCollider) _otherCollider, weak(CollisionData) _collisionData) override;
		bool CollidingWith(weak(BoxCollider) _otherCollider, weak(CollisionData) _collisionData) override;
	};

	struct BoxCollider : public ColliderComponent
	{
	public:
		BoxCollider(weak(GameObject) _parentObject, glm::vec3 _halfExtents, float _bounciness);

		/**
		* @returns the BoxCollider's half extents.
		**/
		glm::vec3 GetHalfExtents() { return m_halfExtents; }
		glm::vec3 GetSurfaceNormalAtPoint(glm::vec3 _point) override;

	protected:
		glm::vec3 m_halfExtents;

		bool CollidingWith(weak(PlaneCollider) _otherCollider, weak(CollisionData) _collisionData) override;
		bool CollidingWith(weak(SphereCollider) _otherCollider, weak(CollisionData) _collisionData) override;
		bool CollidingWith(weak(BoxCollider) _otherCollider, weak(CollisionData) _collisionData) override;
	};

} // namespace MYENGINE

#endif
