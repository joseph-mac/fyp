#include "InputSystemDefs.h"

#include "engine/managers/Manager.h"
#include "engine/managers/SettingsManager.h"
#include "engine/managers/InputManager.h"
#include "engine/input/ControlContext.h"

#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_gamecontroller.h>

namespace MYENGINE
{

	InputDevice::InputDevice(int _instanceID, InputDeviceType _deviceType, int _deviceTypeInstanceID, bool _isActive, SDL_Joystick* _joystickPtr)
	{
		m_instanceID = _instanceID;
		m_isActive = _isActive;
		m_joyStickPtr = _joystickPtr;

		m_type = _deviceType;
		m_instanceOfTypeID = _deviceTypeInstanceID;
	}

	InputDevice::~InputDevice()
	{
		if (m_joyStickPtr)
		{
			if (SDL_JoystickGetType(m_joyStickPtr) == 1)
			{
				SDL_GameControllerClose(SDL_GameControllerFromInstanceID(m_instanceID));
			}
			else
			{
				SDL_JoystickClose(m_joyStickPtr);
			}
		}
	}


	bool IsInputTypeBinary(InputType inputType)
	{
		switch (inputType)
		{
		case InputType::KeyboardKey:
		case InputType::MouseButton:
		case InputType::ControllerButton:
		case InputType::OnScreenButton:
			return true;
			break;
		case InputType::Axis:
		case InputType::ControllerTrigger:
			return false;
			break;
		default:
			return true;
			break;
		}
	}

	Input::Input(InputDeviceType _inputDevice, InputType _inputType, int _inputCode, bool _useToggleMode, short _maxValue)
	{
		inputDevice = _inputDevice;
		inputType = _inputType;
		inputCode = _inputCode;
		isBinary = IsInputTypeBinary(_inputType);

		useToggleMode = _useToggleMode;
		maximumValueCap = _maxValue;
	}

	Input::Input(InputDeviceType _inputDevice, InputType _inputType, int _inputCode, glm::ivec2 _inputDeadzones, bool _passThroughRaw, float _inputSensitivity, bool _useToggleMode, short _maxValue)
	{
		inputDevice = _inputDevice;
		inputType = _inputType;
		inputCode = _inputCode;
		isBinary = IsInputTypeBinary(_inputType);

		inputDeadzones = _inputDeadzones;
		passThroughRawValues = _passThroughRaw;
		sensitivityMultiplier = _inputSensitivity;

		useToggleMode = _useToggleMode;
		maximumValueCap = _maxValue;
	}


	Concept::Concept(const char* _name, std::vector<std::string> _contexts, bool _idealInputRangeIsBinary, InputRangeConversionMode _inputRangeConversionMode)
	{
		name = _name;
		idealInputRangeIsBinary = _idealInputRangeIsBinary;
		inputRangeConversionMode = _inputRangeConversionMode;
		tempContextNames = _contexts;
	}

	void Concept::Initialise(shared(InputManager) _inputManager)
	{
		m_inputManager = _inputManager;

		for (std::string contextName : tempContextNames)
		{
			controlContexts.push_back(m_inputManager.lock()->GetControlContext(contextName));
		}

		tempContextNames.clear();
	}

	bool Concept::IsActive()
	{
		for (weak(ControlContext) cc : controlContexts)
		{
			if (cc.lock() && cc.lock()->isActive)
			{
				return true;
			}
		}
		return false;
	}

	bool Concept::NewlyActive()
	{
		for (weak(ControlContext) cc : controlContexts)
		{
			if (cc.lock() && cc.lock()->newlyActiveThisFrame)
			{
				return true;
			}
		}
		return false;
	}

	ConceptState Concept::GetState()
	{
		ConceptState state;
		state.changed = changed;
		state.value = value;
		state.normalValue = (0.0f + value) / INPUT_VALUE_MAX;
		state.delta = delta;

		state.changedToNonZero = changed && value != 0;
		state.changedToZero = changed && value == 0;
		return state;
	}

	InputConceptBinding::InputConceptBinding(std::vector<shared(Input)> _inputs, shared(Concept) _concept)
	{
		inputs = _inputs;
		concept = _concept;
	}

} // namespace MYENGINE
