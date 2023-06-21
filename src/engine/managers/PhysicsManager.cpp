#include "PhysicsManager.h"

#include "engine/components/physics/ColliderComponents.h"
#include "engine/components/physics/PhysicsComponents.h"
#include "engine/managers/Manager.h"
#include "engine/managers/InputManager.h"

namespace MYENGINE
{

	CollisionData::CollisionData(weak(ColliderComponent) _selfCollider, weak(ColliderComponent) _otherCollider)
	{
		selfCollider = _selfCollider;
		otherCollider = _otherCollider;

		colliders[0] = selfCollider;
		colliders[1] = otherCollider;

		collisionTime = 0.0f;
		contactPoint = glm::vec3(0.0f, 0.0f, 0.0f);

		impactVelocities[0] = glm::vec3(0.0f);
		impactVelocities[1] = glm::vec3(0.0f);

		impactPositions[0] = glm::vec3(0.0f);
		impactPositions[1] = glm::vec3(0.0f);

		collisionNormals[0] = glm::vec3(0.0f);
		collisionNormals[1] = glm::vec3(0.0f);

		collisionNormal = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void CollisionData::SetData(glm::vec3 _contactPoint)
	{
		contactPoint = _contactPoint;
		normalA = selfCollider.lock()->GetSurfaceNormalAtPoint(contactPoint);
		normalB = otherCollider.lock()->GetSurfaceNormalAtPoint(contactPoint);
		CalculateNormal();
	}

	void CollisionData::CalculateNormal()
	{
		collisionNormal = glm::normalize(normalA - normalB);
	}

	PhysicsManager::PhysicsManager(weak(Manager) _manager)
		: SubManager(_manager)
	{
		m_active = false;
		m_timescale = 1.0f;
		m_age = 0.0f;
		m_accumulator = 0.0f;

		m_physicsComponents = std::vector<weak(PhysicsComponent)>{};
		m_colliderComponents = std::vector<weak(ColliderComponent)>{};
	}

	void PhysicsManager::UpdateAll()
	{
		ConceptState speedUpConcept = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_PHYSICS_SPEED_UP");
		if (speedUpConcept.changed && speedUpConcept.value != 0)
		{
			m_timescale += 0.1f;
			std::cout << "PHYSICS TIMESCALE: " << m_timescale << std::endl;
		}

		ConceptState speedDownConcept = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_PHYSICS_SPEED_DOWN");
		if (speedDownConcept.changed && speedDownConcept.value != 0)
		{
			m_timescale -= 0.1f;
			std::cout << "PHYSICS TIMESCALE: " << m_timescale << std::endl;
		}

		if (!m_active)
		{
			return;
		}

		m_accumulator += GetManager()->DeltaTime() * m_timescale;

		while (m_accumulator >= m_fixedTimestep)
		{
			// start of physics step

			m_age += m_fixedTimestep;

			// Perform fixed updates on each physics component (apply gravity, forces etc.)-
			// includes response forces from collisions that occured in the previous physics step
			for (int c = 0; c < m_physicsComponents.size(); c++)
			{
				shared(PhysicsComponent) current = m_physicsComponents.at(c).lock();

				if (current)
				{
					current->FixedUpdate(m_fixedTimestep);
				}
				else
				{
					// this component is dead, so take this opportunity to remove it
					m_physicsComponents.erase(m_physicsComponents.begin() + c);
					c--;
				}
			}

			// clear all lists of collisions generated in the previous physics step
			for (int c = 0; c < m_colliderComponents.size(); c++)
			{
				shared(ColliderComponent) current = m_colliderComponents.at(c).lock();

				if (current)
				{
					current->m_collisions.clear();
				}
				else
				{
					// this component is dead, so take this opportunity to remove it
					m_colliderComponents.erase(m_colliderComponents.begin() + c);
					c--;
				}
			}


			// Assemble lists of all collisions that have occured during this physics step,
			// and correct objects' positions based on these
			for (int a = 0; a < m_colliderComponents.size(); a++)
			{
				for (int b = 0; b < m_colliderComponents.size(); b++)
				{
					if (a != b)
					{
						shared(ColliderComponent) colA = m_colliderComponents.at(a).lock();
						shared(ColliderComponent) colB = m_colliderComponents.at(b).lock();

						if (colA && colB)
						{
							shared(CollisionData) colData;
							for (int c = 0; c < colB->m_collisions.size(); c++)
							{
								if (colB->m_collisions.at(c)->otherCollider.lock() == colA)
								{
									colData = colB->m_collisions.at(c);
								}
							}

							if (!colData)
							{
								colData = make_shared(CollisionData)(colA, colB);
								if (colA->CollidingWith(colB, colData))
								{
									colA->m_collisions.push_back(colData);
									if (shared(PhysicsComponent) colAPhysics = colA->GetPhysicsComponent())
									{
										colAPhysics->HandleCollision(colData, 0);
									}

									colB->m_collisions.push_back(colData);
									if (shared(PhysicsComponent) colBPhysics = colB->GetPhysicsComponent())
									{
										colBPhysics->HandleCollision(colData, 1);
									}
								}
							}
						}
					}
				}
			}

			m_accumulator -= m_fixedTimestep;
		}

		// Apply the object's current velocity VISUALLY ONLY so that objects do not appear juddery
		// should the user's framerate and physics' fixed timestep not align
		const float visualRemainder = m_accumulator / m_fixedTimestep * m_timescale * GetManager()->GetAbsoluteGameSpeed();
		for (int c = 0; c < m_physicsComponents.size(); c++)
		{
			shared(PhysicsComponent) current = m_physicsComponents.at(c).lock();

			if (current)
			{
				current->FixedUpdate(m_fixedTimestep * visualRemainder, false);
			}
		}
	}

	void PhysicsManager::Activate(bool _shouldActivate)
	{
		m_active = _shouldActivate;
		//std::cout << "PHYSICS ACTIVE: " << ((m_active) ? "TRUE" : "FALSE") << std::endl;
	}

} // namespace MYENGINE
