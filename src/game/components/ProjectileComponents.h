#ifndef _SPACEGAME_PROJECTILECOMPONENTS_H_
#define _SPACEGAME_PROJECTILECOMPONENTS_H_

#include "engine/Defines.h"
#include "engine/GameObject.h"
#include "engine/Component.h"

#include <glm/glm.hpp>

namespace MYENGINE
{
	struct CollisionData;
}

struct Projectile : public MYENGINE::Component
{
public:
	Projectile(weak(MYENGINE::GameObject) _parentObject, const char* _name, glm::vec3 _velocity);

	virtual void Destroy() {}
	bool IsDestroyed() { return m_isDestroyed; }

	static void Collision(weak(MYENGINE::CollisionData) _data, int _self);

protected:
	virtual void OnCollision(weak(MYENGINE::CollisionData) _data, int _self) {};

	glm::vec3 m_velocity;
	bool m_isDestroyed = false;
};

struct Asteroid : public Projectile
{
public:
	Asteroid(weak(MYENGINE::GameObject) _parentObject);

	void Destroy() override;
	void DestroyLoudly();

	int GetSize() { return m_size; }

private:
	int m_size = 1;

	void Update() override;

	void OnCollision(weak(MYENGINE::CollisionData) _data, int _self) override;
};

struct Bullet : public Projectile
{
public:
	Bullet(weak(MYENGINE::GameObject) _parentObject);

	void Destroy() override;

private:
	void Update() override;

	void OnCollision(weak(MYENGINE::CollisionData) _data, int _self) override;
};

#endif
