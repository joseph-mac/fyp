#include "implementations.h"

#include "game/components/GeneralComponents.h"
#include "game/components/ProjectileComponents.h"

#include "engine/Defines.h"
#include "engine/TypeDefs.h"
#include "engine/managers/SceneLoader.h"
#include "engine/managers/InputManager.h"
#include "engine/input/InputDefs.h"
#include "engine/input/ControlContext.h"

namespace GAME
{

	// This function should be implemented to allow custom component types to be loaded from scene files.
	bool CheckGameComponents(shared(MYENGINE::GameObject) _newObject, std::vector<std::string> _componentCells)
	{
		if (_componentCells[0] == "GAMECONTROLLER")
		{
			_newObject->AddComponent(make_shared(GameController)(_newObject));
		}
		else if (_componentCells[0] == "PLAYER")
		{
			_newObject->AddComponent(make_shared(Player)(_newObject));
		}
		else if (_componentCells[0] == "SPI")
		{
			_newObject->AddComponent(make_shared(Spinner)(_newObject, MYENGINE::UTIL::FloatFromString(_componentCells.at(1))));
		}
		else if (_componentCells[0] == "SPW")
		{
			_newObject->AddComponent(make_shared(Spawner)(_newObject, MYENGINE::UTIL::FloatFromString(_componentCells.at(1)), MYENGINE::UTIL::FloatFromString(_componentCells.at(2))));
		}
		else
		{
			return false;
		}

		return true;
	}

	// This function should be implemented to utilise custom control contexts.
	std::vector<shared(MYENGINE::ControlContext)> GetGameControlContexts()
	{
		return std::vector<shared(MYENGINE::ControlContext)>
		{
			make_shared(MYENGINE::ControlContext)("IN_GAME", "", false),
			make_shared(MYENGINE::ControlContext)("IN_GAME_ALT", "IN_GAME", false),
			make_shared(MYENGINE::ControlContext)("IN_MAIN_MENU", "", true)
		};
	}

	// This function should be implemented to utilise custom input concepts.
	std::vector<MYENGINE::InputConceptBinding> GetGameInputConceptBindings()
	{
		std::vector<MYENGINE::InputConceptBinding> result = {};

		result.push_back(MYENGINE::InputConceptBinding(
			std::vector<shared(MYENGINE::Input)>
			{
				make_shared(MYENGINE::Input)(MYENGINE::InputDeviceTypes::AllControllers, MYENGINE::InputType::Axis, MYENGINE::InputAxes::LEFTX_NEG),
				make_shared(MYENGINE::Input)(MYENGINE::InputDeviceTypes::AllControllers, MYENGINE::InputType::ControllerButton, MYENGINE::ControllerButtons::LEFT, true, MYENGINE::INPUT_VALUE_MAX / 2)
			},
			make_shared(MYENGINE::Concept)("SHIP_LEFT", string_vec{ "IN_GAME" }, false, MYENGINE::InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		result.push_back(MYENGINE::InputConceptBinding(
			std::vector<shared(MYENGINE::Input)>
			{
				make_shared(MYENGINE::Input)(MYENGINE::InputDeviceTypes::AllControllers, MYENGINE::InputType::Axis, MYENGINE::InputAxes::LEFTX),
				make_shared(MYENGINE::Input)(MYENGINE::InputDeviceTypes::AllControllers, MYENGINE::InputType::ControllerButton, MYENGINE::ControllerButtons::RIGHT, true, MYENGINE::INPUT_VALUE_MAX / 2)
			},
			make_shared(MYENGINE::Concept)("SHIP_RIGHT", string_vec{ "IN_GAME" }, false, MYENGINE::InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		result.push_back(MYENGINE::InputConceptBinding(
			std::vector<shared(MYENGINE::Input)>{},
			make_shared(MYENGINE::Concept)("SHIP_FIRE", string_vec{ "IN_GAME" }, true, MYENGINE::InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		result.push_back(MYENGINE::InputConceptBinding(
			std::vector<shared(MYENGINE::Input)>
			{
				make_shared(MYENGINE::Input)(MYENGINE::InputDeviceTypes::Keyboard, MYENGINE::InputType::KeyboardKey, MYENGINE::KeyboardKeys::t),
				make_shared(MYENGINE::Input)(MYENGINE::InputDeviceTypes::AllControllers, MYENGINE::InputType::ControllerButton, MYENGINE::ControllerButtons::NORTH)
			},
			make_shared(MYENGINE::Concept)("TOGGLE_GAME_CONTEXT", string_vec{ "IN_GAME", "IN_GAME_ALT" }, true, MYENGINE::InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		result.push_back(MYENGINE::InputConceptBinding(
			std::vector<shared(MYENGINE::Input)>{},
			make_shared(MYENGINE::Concept)("EXIT_TO_MENU", string_vec{ "IN_GAME" }, true, MYENGINE::InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		return result;
	}

} // namespace GAME
