#include "GeneralComponents.h"

#include "game/implementations.h"
#include "game/components/ProjectileComponents.h"

#include "engine/managers/Manager.h"
#include "engine/managers/ResourceManager.h"
#include "engine/managers/InputManager.h"
#include "engine/components/Component_Types.h"
#include "engine/managers/PhysicsManager.h"
#include "engine/components/physics/PhysicsComponents.h"
#include "engine/components/physics/ColliderComponents.h"
#include "engine/components/audio/AudioComponents.h"
#include "engine/components/gui/GUIComponents.h"

#include <string>
#include <random>
#include <chrono>


// Simple helper function to generate a random number between 0.0f and 1.0f
float Rand_0_1()
{
	return (float)rand() / RAND_MAX;
}

GameController::GameController(weak(MYENGINE::GameObject) _parentObject)
	: Component(_parentObject, "GAME_CONTROLLER")
{
	m_state = 1;
	m_playerLost = true;

	shared(MYENGINE::GameObject) player = GetManager()->GetGameObject("player");
	shared(MYENGINE::DynamicPhysics) playerPhysics = std::dynamic_pointer_cast<MYENGINE::DynamicPhysics>(player->GetPhysics());
	if (playerPhysics)
	{
		playerPhysics->SetIgnoreCollisions(true);
		playerPhysics->m_ignoreCollisionExceptionNames.push_back("barrierL");
		playerPhysics->m_ignoreCollisionExceptionNames.push_back("barrierR");
		playerPhysics->SetGravityMultiplier(0.0f);
	}

	m_menuOnlyVisualisers = {};
	m_gameOnlyVisualisers = {};

	srand((unsigned int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
}

void GameController::ObjectFirstUpdate()
{
	m_player = GetManager()->GetGameObject("player")->GetComponent<Player>();
	m_spawner = GetManager()->GetGameObject("spawner")->GetComponent<Spawner>();

	for (shared(MYENGINE::GameObject) go : GetManager()->GetGameObjectsWithTag("gameUI"))
	{
		m_gameOnlyVisualisers.push_back(go->GetComponent<MYENGINE::Visualiser>());
	}

	for (shared(MYENGINE::GameObject) go : GetManager()->GetGameObjectsWithTag("mainMenuUI"))
	{
		m_menuOnlyVisualisers.push_back(go->GetComponent<MYENGINE::Visualiser>());
	}

	EnterMainMenu();
}

void GameController::Update()
{
	shared(MYENGINE::InputManager) inputManager = GetManager()->GetInputManager();

	if (inputManager->GetControlContext("IN_MAIN_MENU")->IsActive())
	{
		// start game
		if (inputManager->GetInputConceptState("UI_SELECT").changedToNonZero)
		{
			inputManager->SetControlContextActive("IN_MAIN_MENU", false);

			// Start the game
			m_player.lock()->Reset();
			m_player.lock()->m_canControl = true;
			m_playerLost = false;
			m_state = 1;
			GetManager()->SetMainCamera(m_state);

			// hide all menu-only objects
			for (const weak(MYENGINE::Visualiser)& v : m_menuOnlyVisualisers)
			{
				if (v.lock())
				{
					v.lock()->SetVisible(false);
				}
			}

			// show all game-only objects
			for (const weak(MYENGINE::Visualiser)& v : m_gameOnlyVisualisers)
			{
				if (v.lock())
				{
					v.lock()->SetVisible(true);
				}
			}

			DestroySpawnedObjects();

			m_spawner.lock()->m_canSpawn = true;

			inputManager->SetControlContextActive("IN_GAME", true);
		}

		// exit game
		if (inputManager->GetInputConceptState("UI_BACK").changedToNonZero)
		{
			inputManager->QuitProgram();
		}
	}
	else if (inputManager->GetControlContext("IN_GAME")->IsActive())
	{
		if (m_playerLost || GetManager()->GetInputManager()->GetInputConceptState("EXIT_TO_MENU").changedToNonZero)
		{
			EnterMainMenu();
		}
		else if (inputManager->GetInputConceptState("TOGGLE_GAME_CONTEXT").changedToNonZero)
		{
			inputManager->SetControlContextActive("IN_GAME_ALT", !inputManager->GetControlContext("IN_GAME_ALT")->IsActive());
		}
	}
}

void GameController::EnterMainMenu()
{
	GetManager()->GetInputManager()->SetControlContextActive("IN_GAME", false);

	// Enter main menu
	m_player.lock()->Reset();
	m_player.lock()->m_canControl = false;
	m_state = 0;
	GetManager()->SetMainCamera(m_state);

	// show all menu-only objects
	for (const weak(MYENGINE::Visualiser)& v : m_menuOnlyVisualisers)
	{
		if (v.lock())
		{
			v.lock()->SetVisible(true);
		}
	}

	// hide all game-only objects
	for (const weak(MYENGINE::Visualiser)& v : m_gameOnlyVisualisers)
	{
		if (v.lock())
		{
			v.lock()->SetVisible(false);
		}
	}

	m_spawner.lock()->m_canSpawn = false;

	DestroySpawnedObjects();

	GetManager()->GetInputManager()->SetControlContextActive("IN_MAIN_MENU", true);
}

void GameController::DestroySpawnedObjects()
{
	for (shared(MYENGINE::GameObject) go : GetManager()->GetGameObjectsWithTag("gameProjectile"))
	{
		shared(Projectile) projectileComponent = go->GetComponent<Projectile>();
		if (projectileComponent && !projectileComponent->IsDestroyed())
		{
			projectileComponent->Destroy();
		}
	}
}


Player::Player(weak(MYENGINE::GameObject) _parentObject)
	: Component(_parentObject, "PLAYER")
{
	m_canControl = false;

	m_maxHealth = 5;
	m_health = m_maxHealth;
	m_maxFleetHealth = 4;
	m_fleetHealth = m_maxFleetHealth;
}

void Player::Update()
{
	if (m_canControl && GetManager()->GetInputManager()->GetControlContext("IN_GAME")->IsActive())
	{
		m_timeSinceLastFire += GetManager()->DeltaTime();

		shared(MYENGINE::DynamicPhysics) dp = GetObject()->GetComponent<MYENGINE::DynamicPhysics>();
		float moveSpeed = 20.0f / dp->GetMass();

		MYENGINE::ConceptState shipRightConcept = GetManager()->GetInputManager()->GetInputConceptState("SHIP_RIGHT");
		MYENGINE::ConceptState shipLeftConcept = GetManager()->GetInputManager()->GetInputConceptState("SHIP_LEFT");
		
		// move right
		if (shipRightConcept.value != 0)
		{
			glm::vec3 forceToApply = glm::vec3(1.0f, 0.0f, 0.0f) * moveSpeed * shipRightConcept.normalValue;
			if (dp->GetVelocity().x < forceToApply.x) { dp->AddForce(forceToApply); }
		}
		// move left
		if (shipLeftConcept.value != 0)
		{
			glm::vec3 forceToApply = glm::vec3(-1.0f, 0.0f, 0.0f) * moveSpeed * shipLeftConcept.normalValue;
			if (dp->GetVelocity().x > forceToApply.x) { dp->AddForce(forceToApply); }
		}

		// fire
		if (m_timeSinceLastFire > m_minTimeToNextFire && GetManager()->GetInputManager()->GetInputConceptState("SHIP_FIRE").value != 0)
		{
			Fire();
		}
	}
}

void Player::Fire()
{
	std::string newObjectName = "bullet_";
	newObjectName += std::to_string(m_nextSpawnedId);
	m_nextSpawnedId++;

	shared(MYENGINE::GameObject) newObject = make_shared(MYENGINE::GameObject)(newObjectName.c_str(), GetManager());
	newObject->AddComponent(make_shared(MYENGINE::Transform)(newObject, GetObject()->GetTransform()->GetAbsolutePosition() + glm::vec3(0, 0.5f, 0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 1.0f), true));
	newObject->AddComponent(make_shared(MYENGINE::Visualiser)(newObject, "box", "../../data/textures/white.bmp", "textured-unlit"));

	shared(MYENGINE::DummyPhysics) phys = make_shared(MYENGINE::DummyPhysics)(newObject, glm::vec3(0.0f, 0.0f, -50.0f));
	phys->m_OnCollisionCallbackFunc = Projectile::Collision;
	newObject->AddComponent(phys);
	newObject->AddComponent(make_shared(MYENGINE::SphereCollider)(newObject, 0.2f, 0.9f));
	newObject->AddComponent(make_shared(Bullet)(newObject));

	newObject->AddTag("gameProjectile");

	GetManager()->CreateGameObject(newObject);

	m_timeSinceLastFire = 0.0f;
	shared(MYENGINE::AudioSource) shootAudio = GetObject()->GetComponents<MYENGINE::AudioSource>().at(3);
	shootAudio->SetPitch(0.8f + (Rand_0_1() * 0.4f));
	shootAudio->PlayOnce();
}

void Player::Damage(int _amount)
{
	m_health -= _amount;

	if (m_health <= 0)
	{
		GetObject()->GetComponents<MYENGINE::AudioSource>().at(2)->PlayOnce();
		GetManager()->GetGameObject("controller")->GetComponent<GameController>()->m_playerLost = true;
	}
	else
	{
		if (_amount > 0)
		{
			GetObject()->GetComponents<MYENGINE::AudioSource>().at(0)->PlayOnce();
		}
		GetManager()->GetGameObject("health")->GetComponent<MYENGINE::TextBox>()->SetText(std::to_string(m_health));
	}
}

void Player::DamageFleet(int _amount)
{
	m_fleetHealth -= _amount;

	if (m_fleetHealth <= 0)
	{
		GetObject()->GetComponents<MYENGINE::AudioSource>().at(2)->PlayOnce();
		GetManager()->GetGameObject("controller")->GetComponent<GameController>()->m_playerLost = true;
	}
	else
	{
		if (_amount > 0)
		{
			GetObject()->GetComponents<MYENGINE::AudioSource>().at(1)->PlayOnce();
		}
		GetManager()->GetGameObject("fleetHealth")->GetComponent<MYENGINE::TextBox>()->SetText(std::to_string(m_fleetHealth));
	}
}

void Player::Reset()
{
	m_timeSinceLastFire = 0.0f;

	m_health = m_maxHealth;
	Damage(0);

	m_fleetHealth = m_maxFleetHealth;
	DamageFleet(0);
}


Spinner::Spinner(weak(MYENGINE::GameObject) _parentObject, float _rotationSpeed)
	: Component(_parentObject, "SPINNER")
{
	m_rotationSpeed = _rotationSpeed;
}

void Spinner::Update()
{
	GetObject()->GetTransform()->Rotate(m_rotationSpeed * GetManager()->DeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
}



Spawner::Spawner(weak(MYENGINE::GameObject) _parentObject, float _minTimeToSpawn, float _maxTimeToSpawn)
	: Component(_parentObject, "SPAWNER")
{
	m_canSpawn = false;

	m_minTimeToSpawn = _minTimeToSpawn;
	m_maxTimeToSpawn = _maxTimeToSpawn;

	m_timeSinceLastSpawn = 0.0f;
	m_nextSpawnTime = m_minTimeToSpawn;
}

void Spawner::Update()
{
	if (m_canSpawn && GetManager()->GetInputManager()->GetControlContext("IN_GAME")->IsActive())
	{
		m_timeSinceLastSpawn += GetManager()->DeltaTime();

		if (m_timeSinceLastSpawn >= m_nextSpawnTime)
		{
			Spawn();
			m_timeSinceLastSpawn = 0.0f;

			m_nextSpawnTime = m_minTimeToSpawn + (Rand_0_1() * (m_maxTimeToSpawn - m_minTimeToSpawn));
		}
	}
}

void Spawner::Spawn()
{
	std::string newObjectName = "asteroid_";
	newObjectName += std::to_string(m_nextSpawnedId);
	m_nextSpawnedId++;

	shared(MYENGINE::GameObject) newObject = make_shared(MYENGINE::GameObject)(newObjectName.c_str(), GetManager());
	newObject->AddComponent(make_shared(MYENGINE::Transform)(newObject, GetObject()->GetTransform()->GetAbsolutePosition() + glm::vec3((-1.0f + (Rand_0_1() * 2.0f)) * 17.0f, 0.0f, 0.0f), glm::vec3(Rand_0_1() * 180.0f, 0.0f, 0.0f), glm::vec3(3.0f, 3.0f, 3.0f), true));
	newObject->AddComponent(make_shared(MYENGINE::Visualiser)(newObject, "box", "../../data/textures/brown.png", "textured-full-lit"));

	shared(MYENGINE::DummyPhysics) phys = make_shared(MYENGINE::DummyPhysics)(newObject, glm::vec3(0.0f, 0.0f, 15.0f + (Rand_0_1() * 10.0f)));
	phys->m_OnCollisionCallbackFunc = Projectile::Collision;
	newObject->AddComponent(phys);

	newObject->AddComponent(make_shared(MYENGINE::SphereCollider)(newObject, 1.5f, 0.9f));
	newObject->AddComponent(make_shared(Spinner)(newObject, 30.0f + (Rand_0_1() * 100.0f)));
	newObject->AddComponent(make_shared(Asteroid)(newObject));

	newObject->AddTag("gameProjectile");
	newObject->AddTag("spawnedBy_" + GetObject()->GetName());

	GetManager()->CreateGameObject(newObject);
}
