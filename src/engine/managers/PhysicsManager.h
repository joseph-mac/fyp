#ifndef _MYENGINE_PHYSICSMANAGER_H_
#define _MYENGINE_PHYSICSMANAGER_H_

#include "engine/Defines.h"
#include "engine/managers/SubManager.h"

#include <glm/vec3.hpp>

#include <vector>

namespace MYENGINE
{

	struct Manager;
	struct PhysicsComponent;
	struct ColliderComponent;

	struct CollisionData
	{
	public:
		CollisionData(weak(ColliderComponent) _selfCollider, weak(ColliderComponent) _otherCollider);

		void SetData(glm::vec3 _contactPoint);
		void CalculateNormal();

		weak(ColliderComponent) selfCollider;
		weak(ColliderComponent) otherCollider;

		weak(ColliderComponent) colliders[2];

		float collisionTime;
		glm::vec3 contactPoint;

		glm::vec3 impactVelocities[2];
		glm::vec3 impactPositions[2];
		glm::vec3 collisionNormals[2];

		glm::vec3 impactPosA;
		glm::vec3 impactPosB;


		glm::vec3 normalA;//
		glm::vec3 normalB;//
		glm::vec3 collisionNormal;//

		glm::vec3 collisionNormalA;
		glm::vec3 collisionNormalB;
	};

	struct PhysicsManager : public SubManager
	{
	public:
		PhysicsManager(weak(Manager) _manager);
		~PhysicsManager() {};

		/**
		* Set whether physics should be activated or deactivated.
		*
		* @param _shouldActivate : the desired activation state for physics.
		**/
		void Activate(bool _shouldActivate);
		/**
		* Toggle whether physics should be activated or deactivated.
		**/
		void Activate() { Activate(!m_active); }

		/**
		* Register a new PhysicsComponent for the PhysicsManager to utilise and check on.
		*
		* @param _newComponent : the component for the PhysicsManager to utilise and check on.
		**/
		void RegisterComponent(weak(PhysicsComponent) _newComponent) { m_physicsComponents.push_back(_newComponent); }
		/**
		* Register a new ColliderComponent for the PhysicsManager to utilise and check on.
		*
		* @param _newComponent : the component for the PhysicsManager to utilise and check on.
		**/
		void RegisterComponent(weak(ColliderComponent) _newComponent) { m_colliderComponents.push_back(_newComponent); }

		/**
		* @returns whether collisions will be printed to the console when they occur.
		**/
		bool ShouldPrintCollisions() { return printCollisions; }

	private:
		friend Manager;

		void UpdateAll();

		bool m_active;
		float m_age;
		float m_timescale;
		const float m_fixedTimestep = 0.02f;
		float m_accumulator;

		std::vector<weak(PhysicsComponent)> m_physicsComponents;
		std::vector<weak(ColliderComponent)> m_colliderComponents;

		// CONFIG
		bool printCollisions = true;
	};

} // namespace MYENGINE

#endif
