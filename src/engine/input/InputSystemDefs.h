#ifndef _MYENGINE_INPUTSYSTEMDEFS_H_
#define _MYENGINE_INPUTSYSTEMDEFS_H_

#include "engine/Defines.h"
#include "engine/input/InputDefs.h"

#include <glm/detail/type_vec2.hpp>

#include <vector>

typedef struct _SDL_Joystick SDL_Joystick;

namespace MYENGINE
{

	struct Manager;
	struct SettingsManager;
	struct ControlContext;

	struct InputDevice
	{
	public:
		InputDevice(int _instanceID, InputDeviceType _deviceType, int _deviceTypeInstanceID, bool _isActive, SDL_Joystick* _joystickPtr);
		~InputDevice();

		int m_instanceID;
		bool m_isActive;
		SDL_Joystick* m_joyStickPtr;

		InputDeviceType m_type;
		int m_instanceOfTypeID;
	};

	struct Input
	{
	public:
		Input(InputDeviceType _inputDevice, InputType _inputType, int _inputCode, bool _useToggleMode = false, short _maxValue = INPUT_VALUE_MAX);
		Input(InputDeviceType _inputDevice, InputType _inputType, int _inputCode, glm::ivec2 _inputDeadzones, bool _passThroughRaw, float _inputSensitivity = 1.0f, bool _useToggleMode = false, short _maxValue = INPUT_VALUE_MAX);

	private:
		friend struct InputManager;

		InputDeviceType inputDevice;
		InputType inputType;
		int inputCode;
		bool isBinary;

		// User settings

		// If true, a change of this input from zero to non-zero will toggle between reporting 0 or INPUT_VALUE_MAX.
		// If false, the regular value will be output.
		bool useToggleMode = false;

		// The deadzones for this input.
		//	E.g.: a value of (5000, 1000) means input below 5000 or above INPUT_VALUE_MAX - 1000 will instead report 0 or INPUT_VALUE_MAX, respectively. A value of (0, 0) means all values are valid.
		glm::ivec2 inputDeadzones = glm::ivec2(3500, 3500);

		// If true, the raw input value will be passed through so long as it is within the deadzones.
		// If false, the raw input value will be scaled from the range defined by the deadzones into the full input range.
		//	E.g.: with an inner deadzone of 50 and a raw input of 50, the value will become 0.
		bool passThroughRawValues = true;

		// Input values will be multiplied by this amount.
		// NOTE: The resulting values will remain clamped between 0 and INPUT_VALUE_MAX (inclusive).
		float sensitivityMultiplier = 1;

		// After all other settings have been applied, the raw input value will only be reported if it is equal to or below maximumValueCap. Otherwise, maximumValueCap will be reported.
		short maximumValueCap = INPUT_VALUE_MAX;
	};

	struct ConceptState
	{
	public:
		int value = 0;
		float normalValue = 0.0f;
		bool changed = false;
		int delta = 0;

		bool changedToNonZero = false;
		bool changedToZero = false;
	};

	struct Concept
	{
		Concept(const char* _name, std::vector<std::string> _contexts, bool _idealInputRangeIsBinary, InputRangeConversionMode _inputRangeConversionMode = InputRangeConversionMode::none);

	private:
		friend struct InputManager;

		void Initialise(shared(InputManager) _inputManager);

		// fixed properties
		std::string name = "";
		std::vector<std::string> tempContextNames;
		std::vector<weak(ControlContext)> controlContexts;
		bool idealInputRangeIsBinary;

		// user-configurable properties
		InputRangeConversionMode inputRangeConversionMode = InputRangeConversionMode::none;

		// state properties
		ConceptState GetState();
		bool IsActive();
		bool NewlyActive();

		int value = 0;
		bool changed = false;
		int delta = 0;
		float timeSinceChanged = 0.0f;

		weak(InputManager) m_inputManager;
	};

	struct InputConceptBinding
	{
	public:
		InputConceptBinding(std::vector<shared(Input)> _inputs, shared(Concept) _concept);

		std::vector<shared(Input)> inputs;
		shared(Concept) concept;
	};

} // namespace MYENGINE



namespace GAME
{

	/**
	* This function should be defined by the game, and should provide the game's own control contexts.
	*
	* @returns a vector of pairs representing the name and starting state of control contexts defined by the game.
	**/
	extern std::vector<shared(MYENGINE::ControlContext)> GetGameControlContexts();

	/**
	* This function should be defined by the game, and should initialise the game's own InputConceptBindings.
	*
	* @returns a vector of InputConceptBindings defined by the game.
	**/
	extern std::vector<MYENGINE::InputConceptBinding> GetGameInputConceptBindings();

} // namespace GAME

#endif
