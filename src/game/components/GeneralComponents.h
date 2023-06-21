#ifndef _SPACEGAME_GENERALCOMPONENTS_H_
#define _SPACEGAME_GENERALCOMPONENTS_H_

#include "engine/Defines.h"
#include "engine/GameObject.h"
#include "engine/Component.h"

#include <vector>

namespace MYENGINE
{
	struct Visualiser;
	struct CollisionData;
}

struct Player;
struct Spawner;

struct GameController : public MYENGINE::Component
{
public:
	GameController(weak(MYENGINE::GameObject) _parentObject);

	bool m_playerLost = false;

private:
	void ObjectFirstUpdate() override;
	void Update() override;
	
	void EnterMainMenu();
	void DestroySpawnedObjects();

	weak(Player) m_player;
	weak(Spawner) m_spawner;
	std::vector<weak(MYENGINE::Visualiser)> m_menuOnlyVisualisers;
	std::vector<weak(MYENGINE::Visualiser)> m_gameOnlyVisualisers;

	unsigned int m_state;
};


struct Player : public MYENGINE::Component
{
public:
	Player(weak(MYENGINE::GameObject) _parentObject);

	bool m_canControl;

	void Damage(int _amount);
	void DamageFleet(int _amount);

	void Reset();

private:
	void Update() override;
	void Fire();

	int m_maxHealth;
	int m_health;
	int m_maxFleetHealth;
	int m_fleetHealth;

	float m_timeSinceLastFire = 0.0f;
	float m_minTimeToNextFire = 0.5f;
	unsigned int m_nextSpawnedId = 0;
};

struct Spinner : public MYENGINE::Component
{
	Spinner(weak(MYENGINE::GameObject) _parentObject, float _rotationSpeed);

	void Update() override;

private:
	float m_rotationSpeed;
};



struct Spawner : public MYENGINE::Component
{
public:
	Spawner(weak(MYENGINE::GameObject) _parentObject, float _minTimeToSpawn, float _maxTimeToSpawn);

	bool m_canSpawn;

private:
	void Update() override;
	void Spawn();

	// settings
	float m_minTimeToSpawn;
	float m_maxTimeToSpawn;

	// variables
	unsigned int m_nextSpawnedId = 0;
	float m_timeSinceLastSpawn;
	float m_nextSpawnTime;
};

#endif
