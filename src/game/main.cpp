#define SDL_MAIN_HANDLED

#include "engine/Core.h"
#include "engine/managers/SceneLoader.h"
#include "engine/managers/Manager.h"
#include "engine/managers/PhysicsManager.h"

int main()
{
	MYENGINE::Core::Init();
	MYENGINE::Core::SetWindowTitle("Demo MYENGINE Game");

	MYENGINE::SceneLoader::LoadFromFile("../../data/scenes/space-game.txt");

	MYENGINE::Core::GetManager()->GetPhysicsManager()->Activate(true);

	return MYENGINE::Core::Run();
}
