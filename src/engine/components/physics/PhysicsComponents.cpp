#include "PhysicsComponents.h"

#include "engine/managers/Manager.h"
#include "engine/GameObject.h"
#include "engine/managers/PhysicsManager.h"
#include "engine/managers/InputManager.h"
#include "engine/components/Component_Types.h"
#include "engine/components/physics/ColliderComponents.h"

#include <glm/glm.hpp>

namespace MYENGINE
{

	PhysicsComponent::PhysicsComponent(weak(GameObject) _parentObject, const char* _name, glm::vec3 _initialVelocity)
		: Component(_parentObject, _name)
	{
		shared(Transform) visualTransform = GetObject()->GetTransform();
		m_localTransform = make_shared(Transform)(GetObject(), visualTransform->GetLocalPosition(), visualTransform->GetLocalRotation(), visualTransform->GetAbsoluteScale(), true);
		GetObject()->AddComponent(m_localTransform);

		m_velocity = _initialVelocity;
		m_mass = 1.0f;

		m_resting = false;

		m_previousLocalPosition = glm::vec3(0.0f);
	}

	void PhysicsComponent::Initialise(weak(Component) _componentSelf)
	{
		Component::Initialise(_componentSelf);
		GetManager()->GetPhysicsManager()->RegisterComponent(std::dynamic_pointer_cast<PhysicsComponent>(m_self.lock()));
	}

	void PhysicsComponent::HandleCollision(weak(CollisionData) _data, int _self)
	{
		if (m_OnCollisionCallbackFunc)
		{
			m_OnCollisionCallbackFunc(_data, _self);
		}
	}


	void DynamicPhysics::ComputeInverseInertiaTensor()
	{
		m_inertiaTensorInverse = m_r * m_bodyInertiaTensorInverse * glm::transpose(m_r);
	}

	DynamicPhysics::DynamicPhysics(weak(GameObject) _parentObject, int _numIntMethod, float _mass, float _friction, glm::vec3 _initialVelocity)
		: PhysicsComponent(_parentObject, "DYNAMIC_PHYSICS", _initialVelocity)
	{
		m_mass = _mass;
		m_friction = _friction;

		m_force = glm::vec3(0.0f, 0.0f, 0.0f);

		m_numIntMethod = _numIntMethod;

		m_ignoreCollisions = false;
		m_ignoreCollisionExceptionNames = std::vector<std::string>{};

		m_gravityMultiplier = 1.0f;

		m_collider = GetObject()->GetCollider();
		if (m_collider.lock() && m_collider.lock()->Rotatable())
		{
			m_torque = glm::vec3(0.0f, 0.0f, 0.0f);
			m_angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
			m_angularMomentum = glm::vec3(0.0f, 0.0f, 0.0f);

			m_r = glm::mat3(
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f
			);

			m_bodyInertiaTensorInverse = m_collider.lock()->GetInverseBodyInertiaTensor(m_mass);

			ComputeInverseInertiaTensor();
			m_angularVelocity = m_inertiaTensorInverse * m_angularMomentum;
		}
	}

	bool WithinMargin(float _value, float _margin)
	{
		return (_value < _margin&& _value > -_margin);
	}

	const float noSmoothThreshold = 0.9f;
	void DynamicPhysics::FixedUpdate(float _timestep, bool _updateLocal)
	{
		if (!_updateLocal && WithinMargin(m_velocity.x, noSmoothThreshold) && WithinMargin(m_velocity.y, noSmoothThreshold) && WithinMargin(m_velocity.z, noSmoothThreshold)) { return; }

		if (_updateLocal)
		{
			m_previousLocalPosition = m_localTransform->GetAbsolutePosition();
		}



		if (GetManager()->GetInputManager()->GetInputConceptState("DEBUG_PHYSICS_ADD_Y_FORCE").value != 0) { AddForce(glm::vec3(0.0f, 20.0f * m_mass, 0.0f)); }


		float restValue = 0.05f;

		int velocityDimensionsResting = 0;
		glm::ivec3 resting(0, 0, 0);
		for (int d = 0; d < 3; d++)
		{
			if ((m_force[d] < restValue && m_force[d] > -restValue) &&
				(m_velocity[d] < restValue && m_velocity[d] > -restValue))
			{
				velocityDimensionsResting++;
				resting[d] = 1;
				m_velocity[d] = 0.0f;
			}
		}

		/*if (resting.y && GetObject()->GetName() == "cameraMain")
		{
			std::cout << "Object " << GetObject()->GetName() << " is resting on the y axis" << std::endl;
		}*/

		glm::vec3 gravity = glm::vec3(0.0f, -9.8f * m_mass, 0.0f) * m_gravityMultiplier;
		AddForce(gravity);

		switch (m_numIntMethod)
		{
		case 0:
			Euler(_timestep, _updateLocal);
			break;

		case 1:
			RK2(_timestep, _updateLocal);
			break;

		case 2:
			RK4(_timestep, _updateLocal);
			break;
		}

		ClearForces();
		ClearTorque();
	}


	void DynamicPhysics::HandleCollision(weak(CollisionData) _data, int _self)
	{
		PhysicsComponent::HandleCollision(_data, _self);

		shared(CollisionData) data = _data.lock();

		int other = (_self == 0) ? 1 : 0;

		if (m_ignoreCollisions)
		{
			bool exceptionFound = false;
			for (const std::string& s : m_ignoreCollisionExceptionNames)
			{
				if (data->colliders[other].lock() && data->colliders[other].lock()->GetObject()->GetName() == s)
				{
					exceptionFound = true;
					break;
				}
			}

			if (!exceptionFound)
			{
				return;
			}
		}

		float afterCollisionTime = (1.0f - data->collisionTime) * 0.02f;

		m_localTransform->SetPosition(data->impactPositions[_self]);

		// gather data on the collision
		shared(DynamicPhysics) otherPhysics = data->colliders[other].lock()->GetObject()->GetComponent<DynamicPhysics>();
		glm::vec3 otherVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		if (otherPhysics) { otherVelocity = data->impactVelocities[other]; }
		float oneOverOtherMass = 0.0f;
		if (otherPhysics) { oneOverOtherMass = 1.0f / otherPhysics->GetMass(); }

		glm::vec3 colNorm = glm::normalize(data->collisionNormals[other] - data->collisionNormals[_self]);

		glm::vec3 contactForce = glm::vec3(0, 9.8f * m_mass, 0);
		//AddForce(contactForce);

		glm::vec3 relativeVelocity = data->impactVelocities[_self] - otherVelocity;
		float bounceValue = data->colliders[_self].lock()->GetBounciness();

		float myOneOverMass = 1.0f / m_mass;
		float dot = glm::dot(relativeVelocity, colNorm);
		float jLinear = -(1.0f + bounceValue) * dot / (myOneOverMass + oneOverOtherMass);
		glm::vec3 impulseForce = jLinear * colNorm / afterCollisionTime;

		glm::vec3 forwardRelativeVelocity = relativeVelocity - glm::dot(relativeVelocity, colNorm) * colNorm;
		glm::vec3 forwardDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		if (glm::length(forwardRelativeVelocity) != 0.0f)
		{
			forwardDirection = glm::normalize(forwardRelativeVelocity);
		}

		const float mu = 0.5f;
		glm::vec3 frictionDirection = forwardDirection * -1.0f;
		glm::vec3 frictionForce = frictionDirection * mu * glm::length(contactForce);

		glm::vec3 tempTorque = glm::cross(-1.0f * colNorm, contactForce) + glm::cross(-1.0f * colNorm, frictionForce);
		tempTorque -= m_angularMomentum * 14.0f;

		AddForce(impulseForce);
		AddForce(frictionForce);
		AddTorque(tempTorque);

		float restValue = 0.5f;
		int velocityDimensionsResting = 0;
		if (m_force.x < restValue && m_force.x > -restValue) { velocityDimensionsResting++; m_force.x *= 0.0f; }
		if (m_force.y < restValue && m_force.y > -restValue) { velocityDimensionsResting++; m_force.y *= 0.0f; }
		if (m_force.z < restValue && m_force.z > -restValue) { velocityDimensionsResting++; m_force.z *= 0.0f; }



		// hard coded Euler- collision response didn't play ball with the RK methods for now
		Euler(afterCollisionTime);


		ClearForces();
		ClearTorque();

		/*switch (numIntMethod)
		{
		case 0:
			Euler(afterCollisionTime);
			break;

		case 1:
			RK2(afterCollisionTime);
			break;

		case 2:
			RK4(afterCollisionTime);
			break;
		}*/
	}


	void DynamicPhysics::AddForce(glm::vec3 _force)
	{
		m_force += _force;
	}

	void DynamicPhysics::AddTorque(glm::vec3 _torque)
	{
		m_torque += _torque;
	}

	void DynamicPhysics::ClearForces()
	{
		m_force = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void DynamicPhysics::ClearTorque()
	{
		m_torque = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void DynamicPhysics::Euler(float _timestep, bool _updateLocal)
	{
		if (m_mass == 0)
		{
			std::cout << "mass should never be zero, otherwise we will be dividing by zero" << std::endl;
			return;
		}

		float oneOverMass = 1.0f / m_mass;
		m_velocity += (m_force * oneOverMass) * _timestep;
		glm::vec3 diff = m_velocity * _timestep;


		if (_updateLocal)
		{
			m_localTransform->Translate(diff);
			GetObject()->GetTransform()->SetPosition(m_localTransform->GetLocalPosition());

			m_angularMomentum += m_torque * _timestep;
			ComputeInverseInertiaTensor();

			m_angularVelocity = m_inertiaTensorInverse * m_angularMomentum;

			glm::mat3 omegaStar = glm::mat3
			(
				0.0f, -m_angularVelocity.z, m_angularVelocity.y,
				m_angularVelocity.z, 0.0f, -m_angularVelocity.x,
				-m_angularVelocity.y, m_angularVelocity.x, 0.0f
			);

			m_r += omegaStar * m_r * _timestep;
		}
		else
		{
			GetObject()->GetTransform()->Translate(diff);
		}
	}

	void DynamicPhysics::RK2(float _timestep, bool _updateLocal)
	{
		float oneOverMass = 1.0f / m_mass;
		glm::vec3 _force, _acceleration;
		glm::vec3 k0, k1;

		// k0
		_force = m_force;
		_acceleration = _force / m_mass;
		k0 = _acceleration * _timestep;

		// half deltaTs
		_force = m_force + (k0 * 0.5f);
		_acceleration = _force / m_mass;
		k1 = _acceleration * _timestep;

		m_velocity += k1;
		glm::vec3 diff = m_velocity * _timestep;

		if (_updateLocal)
		{
			m_localTransform->Translate(diff);
			GetObject()->GetTransform()->SetPosition(m_localTransform->GetLocalPosition());
		}
		else
		{
			GetObject()->GetTransform()->Translate(diff);
		}
	}

	void DynamicPhysics::RK4(float _timestep, bool _updateLocal)
	{
		float oneOverMass = 1.0f / m_mass;
		glm::vec3 _force, _acceleration;
		glm::vec3 k0, k1, k2, k3;

		// k0
		_force = m_force;
		_acceleration = _force / m_mass;
		k0 = _acceleration * _timestep;

		// k1: half deltaTs from k0
		_force = m_force + (k0 * 0.5f);
		_acceleration = _force / m_mass;
		k1 = _acceleration * _timestep;

		// k2: half deltaTs from k1
		_force = m_force + (k1 * 0.5f);
		_acceleration = _force / m_mass;
		k2 = _acceleration * _timestep;

		// k3: deltaTs from k0
		_force = m_force + k2;
		_acceleration = _force / m_mass;
		k3 = _acceleration * _timestep;


		m_velocity += (k0 + 2.0f * k1 + 2.0f * k2 + k3) / 6.0f;

		glm::vec3 diff = m_velocity * _timestep;

		if (_updateLocal)
		{
			m_localTransform->Translate(diff);
			GetObject()->GetTransform()->SetPosition(m_localTransform->GetLocalPosition());
		}
		else
		{
			GetObject()->GetTransform()->Translate(diff);
		}
	}



	DummyPhysics::DummyPhysics(weak(GameObject) _parentObject, glm::vec3 _constantVelocity)
		: PhysicsComponent(_parentObject, "DUMMY_PHYSICS", _constantVelocity)
	{
		m_mass = 1.0f;
	}

	void DummyPhysics::FixedUpdate(float _timestep, bool _updateLocal)
	{
		if (_updateLocal)
		{
			m_previousLocalPosition = m_localTransform->GetAbsolutePosition();
		}

		glm::vec3 diff = m_velocity * _timestep;

		if (_updateLocal)
		{
			m_localTransform->Translate(diff);
			GetObject()->GetTransform()->SetPosition(m_localTransform->GetLocalPosition());
		}
		else
		{
			GetObject()->GetTransform()->Translate(diff);
		}
	}

} // namespace MYENGINE
