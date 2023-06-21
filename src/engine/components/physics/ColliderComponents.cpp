#include "ColliderComponents.h"

#include "engine/managers/Manager.h"
#include "engine/managers/PhysicsManager.h"
#include "engine/GameObject.h"
#include "engine/Component.h"
#include "engine/components/Component_Types.h"
#include "engine/components/physics/PhysicsComponents.h"

#include <glm/glm.hpp>

namespace MYENGINE
{

	bool WithinRange(float _toCheck, float _desired, float _allowanceUpDown)
	{
		return
			_toCheck >= _desired - _allowanceUpDown &&
			_toCheck <= _desired + _allowanceUpDown;
	}


	ColliderComponent::ColliderComponent(weak(GameObject) _parentObject, const char* _name, float _bounciness, bool _rotatable)
		: Component(_parentObject, _name)
	{
		//boundingBox = new Box(GetObject()->GetTransform()->GetAbsolutePosition(), glm::vec3(0.5f, 0.5f, 0.5f));

		m_collisions = std::vector<shared(CollisionData)>{};

		m_physicsComponent = GetObject()->GetPhysics();

		m_rotatable = _rotatable;
		m_bounciness = _bounciness;
		m_active = true;
		if (m_bounciness < 0.0f)
		{
			m_active = false;
		}

		m_frictionMultiplier = 1.0f;
	}

	void ColliderComponent::Initialise(weak(Component) _componentSelf)
	{
		Component::Initialise(_componentSelf);
		GetManager()->GetPhysicsManager()->RegisterComponent(std::dynamic_pointer_cast<ColliderComponent>(m_self.lock()));
	}

	bool ColliderComponent::CollidingWith(weak(ColliderComponent) _otherCollider, weak(CollisionData) _collisionData)
	{
		if (!m_active)
		{
			return false;
		}

		weak(PlaneCollider) asPlane = std::dynamic_pointer_cast<PlaneCollider>(_otherCollider.lock());
		if (asPlane.lock()) { return CollidingWith(asPlane, _collisionData); }

		weak(SphereCollider) asSphere = std::dynamic_pointer_cast<SphereCollider>(_otherCollider.lock());
		if (asSphere.lock()) { return CollidingWith(asSphere, _collisionData); }

		weak(BoxCollider) asBox = std::dynamic_pointer_cast<BoxCollider>(_otherCollider.lock());
		if (asBox.lock()) { return CollidingWith(asBox, _collisionData); }

		std::cout << "ERROR: ColliderComponent::CollidingWith does not recognise the other given collider!";
		return false;
	}

	glm::mat3 SphereCollider::GetInverseBodyInertiaTensor(float _mass)
	{
		glm::mat3 matrix = glm::mat3
		(
			(2.0f / 3.0f) * _mass * m_radius * m_radius, 0.0f, 0.0f,
			0.0f, (2.0f / 3.0f) * _mass * m_radius * m_radius, 0.0f,
			0.0f, 0.0f, (2.0f / 3.0f) * _mass * m_radius * m_radius
		);

		return glm::inverse(matrix);
	}



	PlaneCollider::PlaneCollider(weak(GameObject) _parentObject, glm::vec3 _normal)
		: ColliderComponent(_parentObject, "PLANE_COLLIDER", 0.5f, false)
	{
		m_normal = _normal;
	}

	SphereCollider::SphereCollider(weak(GameObject) _parentObject, float _radius, float _bounciness)
		: ColliderComponent(_parentObject, "SPHERE_COLLIDER", _bounciness, false)
	{
		m_radius = _radius;
	}

	BoxCollider::BoxCollider(weak(GameObject) _parentObject, glm::vec3 _halfExtents, float _bounciness)
		: ColliderComponent(_parentObject, "BOX_COLLIDER", _bounciness, false)
	{
		m_halfExtents = _halfExtents;
	}


	glm::vec3 PlaneCollider::GetSurfaceNormalAtPoint(glm::vec3 _point)
	{
		return m_normal;
	}

	glm::vec3 SphereCollider::GetSurfaceNormalAtPoint(glm::vec3 _point)
	{
		return -glm::normalize(m_physicsComponent.lock()->GetLocalTransform()->GetAbsolutePosition() - _point);
	}

	glm::vec3 BoxCollider::GetSurfaceNormalAtPoint(glm::vec3 _point)
	{
		glm::vec3 n = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 myPos = GetObject()->GetTransform()->GetAbsolutePosition();

		for (int i = 0; i < 3; i++)
		{
			if (_point[i] > myPos[i] + m_halfExtents[i])
			{
				n[i] = 1.0f;
			}
			else if (_point[i] < myPos[i] - m_halfExtents[i])
			{
				n[i] = -1.0f;
			}
		}

		return n;
	}


	float DistanceToPlane(glm::vec3 _planeNormal, glm::vec3 _point, glm::vec3 _pointOnPlane)
	{
		_planeNormal = glm::normalize(_planeNormal);
		float d = glm::dot(glm::vec3(_point.x - _pointOnPlane.x, _point.y - _pointOnPlane.y, _point.z - _pointOnPlane.z), _planeNormal);
		return (d);
	}

	bool SphereCollider::CollidingWith(weak(PlaneCollider) _otherCollider, weak(CollisionData) _collisionData)
	{
		shared(PhysicsComponent) dynamicPhysicsComponent = m_physicsComponent.lock();
		shared(PlaneCollider) otherCollider = _otherCollider.lock();
		shared(CollisionData) collisionData = _collisionData.lock();

		glm::vec3 myPrevPos = dynamicPhysicsComponent->GetPreviousLocalPosition();
		glm::vec3 myPos = dynamicPhysicsComponent->GetLocalTransform()->GetAbsolutePosition();

		glm::vec3 pointOnPlane = otherCollider->GetObject()->GetTransform()->GetAbsolutePosition();
		float distTo0 = DistanceToPlane(otherCollider->GetNormal(), myPrevPos, pointOnPlane);
		float distTo1 = DistanceToPlane(otherCollider->GetNormal(), myPos, pointOnPlane);

		glm::vec3 myTravelNormal = glm::normalize(myPos - myPrevPos);
		glm::vec3 otherTravelNormal = glm::vec3(0.0f, 0.0f, 0.0f);
		float u = 0;
		if (glm::abs(distTo0) <= m_radius)
		{
			collisionData->collisionTime = u;

			collisionData->impactVelocities[0] = dynamicPhysicsComponent->GetVelocity();
			collisionData->impactVelocities[1] = glm::vec3(0.0f, 0.0f, 0.0f);

			collisionData->impactPositions[0] = myPrevPos;
			collisionData->impactPositions[1] = pointOnPlane;

			collisionData->collisionNormals[0] = myTravelNormal;
			collisionData->collisionNormals[1] = myTravelNormal - otherCollider->GetNormal();

			return true;
		}
		else if (distTo0 > m_radius && distTo1 < m_radius)
		{
			u = (distTo0 - m_radius) / (distTo0 - distTo1);
			collisionData->collisionTime = u;

			collisionData->impactVelocities[0] = dynamicPhysicsComponent->GetVelocity();
			collisionData->impactVelocities[1] = glm::vec3(0.0f, 0.0f, 0.0f);

			collisionData->impactPositions[0] = (1.0f - u) * myPrevPos + u * myPos;
			collisionData->impactPositions[1] = pointOnPlane;

			collisionData->collisionNormals[0] = myTravelNormal;
			collisionData->collisionNormals[1] = myTravelNormal - otherCollider->GetNormal();

			return true;
		}

		return false;
	}


	bool QuadraticFormula(float a, float b, float c, float* result1, float* result2)
	{
		const float q = b * b - 4 * a * c;
		if (q >= 0)
		{
			const float sq = sqrt(q);
			const float d = 1 / (2 * a);
			*result1 = (-b + sq) * d;
			*result2 = (-b - sq) * d;
			return true;
		}
		else
		{
			return false;
		}

	}


	bool SphereCollider::CollidingWith(weak(SphereCollider) _otherCollider, weak(CollisionData) _collisionData)
	{
		shared(PhysicsComponent) dynamicPhysicsComponent = m_physicsComponent.lock();
		shared(SphereCollider) otherCollider = _otherCollider.lock();
		shared(CollisionData) collisionData = _collisionData.lock();

		glm::vec3 myPrevPos = dynamicPhysicsComponent->GetPreviousLocalPosition();
		glm::vec3 myPos = dynamicPhysicsComponent->GetLocalTransform()->GetAbsolutePosition();

		shared(PhysicsComponent) otherPhysics = otherCollider->GetPhysicsComponent();
		glm::vec3 otherPrevPos = otherPhysics->GetPreviousLocalPosition();
		glm::vec3 otherPos = otherPhysics->GetLocalTransform()->GetAbsolutePosition();
		float otherRadius = otherCollider->m_radius;

		float u0;
		float u1;

		glm::vec3 myTravelVec = myPos - myPrevPos;
		glm::vec3 otherTravelVec = otherPos - otherPrevPos;
		glm::vec3 previousToPreviousVec = (otherPrevPos - myPrevPos);

		glm::vec3 relativeVelocity = otherTravelVec - myTravelVec;
		float radiiSum = m_radius + otherRadius;

		float a = glm::dot(relativeVelocity, relativeVelocity);

		float b = 2 * glm::dot(relativeVelocity, previousToPreviousVec);

		float previousToPreviousSelfDot = glm::dot(previousToPreviousVec, previousToPreviousVec);
		float c = previousToPreviousSelfDot - radiiSum * radiiSum;

		glm::vec3 myTravelNormal = glm::normalize(myTravelVec);
		glm::vec3 otherTravelNormal = glm::normalize(otherTravelVec);
		if (previousToPreviousSelfDot <= radiiSum * radiiSum)
		{
			u0 = 0;
			u1 = 0;

			collisionData->collisionTime = u0;

			collisionData->impactVelocities[0] = dynamicPhysicsComponent->GetVelocity();
			collisionData->impactVelocities[1] = otherPhysics->GetVelocity();

			collisionData->impactPositions[0] = myPrevPos + (myTravelVec * -m_radius);
			collisionData->impactPositions[1] = otherPrevPos + (otherTravelVec * -otherRadius);

			collisionData->collisionNormals[0] = myTravelNormal;
			collisionData->collisionNormals[1] = otherTravelNormal;

			return true;
		}

		if (QuadraticFormula(a, b, c, &u0, &u1))
		{

			if (u0 > u1)
			{
				float temp = u0;
				u0 = u1;
				u1 = temp;
			}

			if (0 <= u0 && u0 <= 1)
			{
				collisionData->collisionTime = u0;

				collisionData->impactVelocities[0] = dynamicPhysicsComponent->GetVelocity();
				collisionData->impactVelocities[1] = otherPhysics->GetVelocity();

				collisionData->impactPositions[0] = myPrevPos + (myTravelVec * u0);
				collisionData->impactPositions[1] = otherPrevPos + (otherTravelVec * u0);

				collisionData->collisionNormals[0] = myTravelNormal;
				collisionData->collisionNormals[1] = otherTravelNormal;

				return true;
			}
		}

		return false;
	}

	bool SphereCollider::CollidingWith(weak(BoxCollider) _otherCollider, weak(CollisionData) _collisionData)
	{
		return false;

		shared(PhysicsComponent) dynamicPhysicsComponent = m_physicsComponent.lock();
		shared(BoxCollider) otherCollider = _otherCollider.lock();
		shared(CollisionData) collisionData = _collisionData.lock();

		glm::vec3 spherePos = dynamicPhysicsComponent->GetLocalTransform()->GetAbsolutePosition();
		glm::vec3 boxPos = otherCollider->GetPhysicsComponent()->GetLocalTransform()->GetAbsolutePosition();
		float s, d = 0.0f;

		for (int i = 0; i < 3; i++)
		{
			float min = boxPos[i] - otherCollider->GetHalfExtents()[i];
			float max = boxPos[i] + otherCollider->GetHalfExtents()[i];

			if (spherePos[i] < min)
			{
				s = spherePos[i] - min;
				d += s * s;
			}
			else if (spherePos[i] > max)
			{
				s = spherePos[i] - max;
				d += s * s;
			}
		}

		bool result = d <= m_radius * m_radius;

		if (result)
		{
			glm::vec3 contactPoint = glm::normalize(spherePos - boxPos) * m_radius;
			collisionData->SetData(contactPoint);

			collisionData->normalB = otherCollider->GetSurfaceNormalAtPoint(collisionData->contactPoint);
			collisionData->normalA = -collisionData->normalB;
			collisionData->CalculateNormal();
		}

		return result;
	}

	bool BoxCollider::CollidingWith(weak(PlaneCollider) _otherCollider, weak(CollisionData) _collisionData)
	{
		shared(PhysicsComponent) dynamicPhysicsComponent = m_physicsComponent.lock();
		shared(PlaneCollider) otherCollider = _otherCollider.lock();
		shared(CollisionData) collisionData = _collisionData.lock();

		glm::vec3 myPrevPos = dynamicPhysicsComponent->GetPreviousLocalPosition();
		glm::vec3 myPos = dynamicPhysicsComponent->GetLocalTransform()->GetAbsolutePosition();

		glm::vec3 pointOnPlane = otherCollider->GetObject()->GetTransform()->GetAbsolutePosition();
		float distTo0 = DistanceToPlane(otherCollider->GetNormal(), myPrevPos, pointOnPlane);
		float distTo1 = DistanceToPlane(otherCollider->GetNormal(), myPos, pointOnPlane);

		float f =
			m_halfExtents.x * glm::dot(otherCollider->GetNormal(), glm::vec3(1.0f, 0.0f, 0.0f)) +
			m_halfExtents.y * glm::dot(otherCollider->GetNormal(), glm::vec3(0.0f, 1.0f, 0.0f)) +
			m_halfExtents.z * glm::dot(otherCollider->GetNormal(), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 myTravelNormal = glm::normalize(myPos - myPrevPos);
		glm::vec3 otherTravelNormal = glm::vec3(0.0f, 0.0f, 0.0f);
		float u = 0;
		if (glm::abs(distTo0) <= f)
		{
			collisionData->collisionTime = u;

			collisionData->impactVelocities[0] = dynamicPhysicsComponent->GetVelocity();
			collisionData->impactVelocities[1] = glm::vec3(0.0f, 0.0f, 0.0f);

			collisionData->impactPositions[0] = myPrevPos;
			collisionData->impactPositions[1] = pointOnPlane;

			collisionData->collisionNormals[0] = myTravelNormal;
			collisionData->collisionNormals[1] = myTravelNormal - otherCollider->GetNormal();

			return true;
		}
		else if (distTo0 > f && distTo1 < f)
		{
			u = (distTo0 - f) / (distTo0 - distTo1);
			collisionData->collisionTime = u;

			collisionData->impactVelocities[0] = dynamicPhysicsComponent->GetVelocity();
			collisionData->impactVelocities[1] = glm::vec3(0.0f, 0.0f, 0.0f);

			collisionData->impactPositions[0] = (1.0f - u) * myPrevPos + u * myPos;
			collisionData->impactPositions[1] = pointOnPlane;

			collisionData->collisionNormals[0] = myTravelNormal;
			collisionData->collisionNormals[1] = myTravelNormal - otherCollider->GetNormal();

			return true;
		}

		return false;
	}


	bool BoxCollider::CollidingWith(weak(SphereCollider) _otherCollider, weak(CollisionData) _collisionData)
	{
		return false;

		shared(PhysicsComponent) dynamicPhysicsComponent = m_physicsComponent.lock();
		shared(SphereCollider) otherCollider = _otherCollider.lock();
		shared(CollisionData) collisionData = _collisionData.lock();

		glm::vec3 spherePos = dynamicPhysicsComponent->GetLocalTransform()->GetAbsolutePosition();
		glm::vec3 boxPos = otherCollider->GetPhysicsComponent()->GetLocalTransform()->GetAbsolutePosition();
		float s, d = 0.0f;

		for (int i = 0; i < 3; i++)
		{
			float min = boxPos[i] - m_halfExtents[i];
			float max = boxPos[i] + m_halfExtents[i];

			if (spherePos[i] < min)
			{
				s = spherePos[i] - min;
				d += s * s;
			}
			else if (spherePos[i] > max)
			{
				s = spherePos[i] - max;
				d += s * s;
			}
		}

		bool result = d <= otherCollider->GetRadius() * otherCollider->GetRadius();

		if (result)
		{
			glm::vec3 contactPoint = glm::normalize(spherePos - boxPos) * otherCollider->GetRadius();
			collisionData->SetData(contactPoint);

			collisionData->normalA = otherCollider->GetSurfaceNormalAtPoint(collisionData->contactPoint);
			collisionData->normalB = -collisionData->normalA;
			collisionData->CalculateNormal();
		}

		return result;
	}

	bool BoxCollider::CollidingWith(weak(BoxCollider) _otherCollider, weak(CollisionData) _collisionData)
	{
		return false;

		//shared(CollisionData) collisionData = _collisionData.lock();

		//const float ts = 0.02f;

		//glm::vec3 myPos = GetPhysicsComponent()->GetLocalTransform()->GetAbsolutePosition();
		//glm::vec3 myNextPos = myPos + GetPhysicsComponent()->GetVelocity() * ts;
		//glm::vec3 myDisplacement = myNextPos - myPos;

		//shared(BoxCollider) otherCollider = _otherCollider.lock();
		//shared(DynamicPhysics) otherPhysics = otherCollider->GetPhysicsComponent();
		//glm::vec3 otherPos = otherPhysics->GetLocalTransform()->GetAbsolutePosition();
		//glm::vec3 otherNextPos = otherPhysics->GetLocalTransform()->GetAbsolutePosition() + otherPhysics->GetVelocity() * ts;
		//glm::vec3 otherDisplacement = otherNextPos - otherPos;

		//glm::vec3 relativePosition = otherPos - myPos;
		//glm::vec3 rd = (otherDisplacement - myDisplacement);
		//glm::vec3 relativeDisplacement = glm::vec3(rd.x * ts, rd.y * ts, rd.z * ts);

		//float earliestOverlapTime = 0.0f;
		//float latestOverlapTime = 0.0f;
		//glm::vec3 latestOverlapTimes = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 earliestOverlapTimes = glm::vec3(1.0f, 1.0f, 1.0f);

		//if (glm::abs(relativePosition.x) <= (m_halfExtents.x + otherCollider->m_halfExtents.x) &&
		//	glm::abs(relativePosition.y) <= (m_halfExtents.y + otherCollider->m_halfExtents.y) &&
		//	glm::abs(relativePosition.z) <= (m_halfExtents.z + otherCollider->m_halfExtents.z))
		//{
		//	earliestOverlapTime = 0.0f;
		//	latestOverlapTime = 0.0f;

		//	collisionData->SetData(myPos);
		//	collisionData->normalA = otherCollider->GetSurfaceNormalAtPoint(collisionData->contactPoint);
		//	collisionData->normalB = -collisionData->normalA;
		//	collisionData->CalculateNormal();

		//	return true;
		//}

		//glm::vec3 myMins = glm::vec3(myPos.x - m_halfExtents.x, myPos.y - m_halfExtents.y, myPos.z - m_halfExtents.z);
		//glm::vec3 myMaxes = glm::vec3(myPos.x + m_halfExtents.x, myPos.y + m_halfExtents.y, myPos.z + m_halfExtents.z);
		//glm::vec3 otherMins = glm::vec3(otherPos.x - otherCollider->m_halfExtents.x, otherPos.y - otherCollider->m_halfExtents.y, otherPos.z - otherCollider->m_halfExtents.z);
		//glm::vec3 otherMaxes = glm::vec3(otherPos.x + otherCollider->m_halfExtents.x, otherPos.y + otherCollider->m_halfExtents.y, otherPos.z + otherCollider->m_halfExtents.z);
		//for (int i = 0; i < 3; i++)
		//{
		//	if (myMaxes[i] < otherMins[i] && relativeDisplacement[i] < 0.0f)
		//	{
		//		earliestOverlapTimes[i] = (myMaxes[i] - otherMins[i]) / relativeDisplacement[i];
		//	}
		//	else if (otherMaxes[i] < myMins[i] && relativeDisplacement[i] > 0.0f)
		//	{
		//		earliestOverlapTimes[i] = (myMins[i] - otherMaxes[i]) / relativeDisplacement[i];
		//	}

		//	if (otherMaxes[i] > myMins[i] && relativeDisplacement[i] < 0.0f)
		//	{
		//		latestOverlapTimes[i] = (myMins[i] - otherMaxes[i]) / relativeDisplacement[i];
		//	}
		//	else if (myMaxes[i] > otherMins[i] && relativeDisplacement[i] > 0.0f)
		//	{
		//		latestOverlapTimes[i] = (myMaxes[i] - otherMins[i]) / relativeDisplacement[i];
		//	}
		//}

		//earliestOverlapTime = glm::max(earliestOverlapTimes.x, glm::max(earliestOverlapTimes.y, earliestOverlapTimes.z));
		//latestOverlapTime = glm::min(latestOverlapTimes.x, glm::min(latestOverlapTimes.y, latestOverlapTimes.z));

		//collisionData->SetData(myPos);
		///*collisionData->normalA = otherCollider->GetSurfaceNormalAtPoint(collisionData->contactPoint);
		//collisionData->normalB = -collisionData->normalA;
		//collisionData->CalculateNormal();*/

		//return earliestOverlapTime <= latestOverlapTime;
	}

} // namespace MYENGINE
