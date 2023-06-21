#include "ProjectileComponents.h"

#include "game/components/GeneralComponents.h"

#include "engine/managers/Manager.h"
#include "engine/managers/ResourceManager.h"
#include "engine/components/Component_Types.h"
#include "engine/managers/PhysicsManager.h"
#include "engine/components/physics/PhysicsComponents.h"
#include "engine/components/physics/ColliderComponents.h"
#include "engine/components/audio/AudioComponents.h"


Projectile::Projectile(weak(MYENGINE::GameObject) _parentObject, const char* _name, glm::vec3 _velocity)
	: Component(_parentObject, _name)
{
	m_velocity = _velocity;
}

void Projectile::Collision(weak(MYENGINE::CollisionData) _data, int _self)
{
	shared(MYENGINE::GameObject) selfObj = _data.lock()->colliders[_self].lock()->GetObject();
	selfObj->GetComponent<Projectile>()->OnCollision(_data, _self);
}

Asteroid::Asteroid(weak(MYENGINE::GameObject) _parentObject)
	: Projectile(_parentObject, "ASTEROID", glm::vec3(0.0f, 0.0f, 30.0f))
{
	
}

void Asteroid::Update()
{
	if (GetObject()->GetTransform()->GetAbsolutePosition().z > 40)
	{
		GetManager()->GetGameObject("player")->GetComponent<Player>()->DamageFleet(GetSize());
		Destroy();
	}
}

void Asteroid::Destroy()
{
	m_isDestroyed = true;
	GetManager()->RemoveGameObject(GetObject());
}

void Asteroid::DestroyLoudly()
{
	GetManager()->GetGameObject("controller")->GetComponent<MYENGINE::AudioSource>()->PlayOnce();
	Destroy();
}

void Asteroid::OnCollision(weak(MYENGINE::CollisionData) _data, int _self)
{
	shared(MYENGINE::CollisionData) data = _data.lock();

	int other = (_self == 0) ? 1 : 0;

	shared(MYENGINE::GameObject) selfObj = data->colliders[_self].lock()->GetObject();
	shared(MYENGINE::GameObject) otherObj = data->colliders[other].lock()->GetObject();

	shared(Asteroid) asteroidSelf = selfObj->GetComponent<Asteroid>();

	shared(Player) playerOther = otherObj->GetComponent<Player>();

	if (asteroidSelf && playerOther && !asteroidSelf->IsDestroyed())
	{
		asteroidSelf->Destroy();
		playerOther->Damage(m_size);
	}
}

Bullet::Bullet(weak(MYENGINE::GameObject) _parentObject)
	: Projectile(_parentObject, "BULLET", glm::vec3(0.0f, 0.0f, -50.0f))
{

}

void Bullet::Update()
{
	if (GetObject()->GetTransform()->GetAbsolutePosition().z < -100)
	{
		Destroy();
	}
}

void Bullet::Destroy()
{
	GetManager()->RemoveGameObject(GetObject());
}

void Bullet::OnCollision(weak(MYENGINE::CollisionData) _data, int _self)
{
	shared(MYENGINE::CollisionData) data = _data.lock();

	int other = (_self == 0) ? 1 : 0;

	shared(MYENGINE::GameObject) selfObj = data->colliders[_self].lock()->GetObject();
	shared(MYENGINE::GameObject) otherObj = data->colliders[other].lock()->GetObject();

	shared(Bullet) bulletSelf = selfObj->GetComponent<Bullet>();

	shared(Asteroid) asteroidOther = otherObj->GetComponent<Asteroid>();

	if (bulletSelf && asteroidOther && !asteroidOther->IsDestroyed())
	{
		bulletSelf->Destroy();
		asteroidOther->DestroyLoudly();
	}
}
