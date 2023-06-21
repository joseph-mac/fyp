#include "InputManager.h"

#include "engine/managers/Manager.h"
#include "engine/GameObject.h"
#include "engine/components/gui/GUIComponents.h"
#include "engine/TypeDefs.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_gamecontroller.h>

namespace MYENGINE
{

	InputManager::InputManager(weak(Manager) manager)
		: SubManager(manager)
	{
		m_userWantsQuit = false;

		m_mouseLocked = false;
		m_mouseLockedLastFrame = false;
		m_mouseMovedThisFrame = false;
		m_mousePosition = glm::vec2(0, 0);
		m_mouseDelta = glm::vec2(0, 0);
		m_lastFrameMousePosition = glm::vec2(0, 0);

		// initialise the input concept system
		m_concepts = {};
		m_inputDevices = { {INPUT_DEVICE_INSTANCE_INVALID, make_shared(InputDevice)(INPUT_DEVICE_INSTANCE_INVALID, InputDeviceTypes::Unknown, 0, false, nullptr)} };
		m_onScreenButtons = {};
	}

	void InputManager::Initialise(shared(InputManager) _inputManager)
	{
		m_self = _inputManager;

		m_controlContexts.push_back(make_shared(ControlContext)("CONST", "", true));
		m_controlContexts.back()->Initialise(GetSelf(), GetManager());

		m_controlContexts.push_back(make_shared(ControlContext)("IN_DEBUG", "", true));
		m_controlContexts.back()->Initialise(GetSelf(), GetManager());

		for (shared(ControlContext) context : GAME::GetGameControlContexts())
		{
			m_controlContexts.push_back(context);
			context->Initialise(GetSelf(), GetManager());
		}

		m_inputConceptBindings = {};

		// This binding is a special case- it needs to be 'finalised' before the on-screen button Inputs are registered.
		m_onScreenButtonPressBinding = InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Mouse, InputType::MouseButton, MouseButtons::LEFTCLICK),
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::SPACE),
				make_shared(Input)(InputDeviceTypes::NIN_all, InputType::ControllerButton, ControllerButtons::EAST),
				make_shared(Input)(InputDeviceTypes::AllControllers^ InputDeviceTypes::NIN_all, InputType::ControllerButton, ControllerButtons::SOUTH)
			},
			make_shared(Concept)("ONSCREEN_BUTTON_PRESS", string_vec{ "CONST" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		);
		AddInputConceptBinding(m_onScreenButtonPressBinding);

		// UI CONCEPTS
		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::NIN_all, InputType::ControllerButton, ControllerButtons::EAST),
				make_shared(Input)(InputDeviceTypes::AllControllers^ InputDeviceTypes::NIN_all, InputType::ControllerButton, ControllerButtons::SOUTH)
			},
			make_shared(Concept)("UI_SELECT", string_vec{ "IN_MAIN_MENU" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::BACKSPACE),
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::ESCAPE),
				make_shared(Input)(InputDeviceTypes::NIN_all, InputType::ControllerButton, ControllerButtons::SOUTH),
				make_shared(Input)(InputDeviceTypes::AllControllers^ InputDeviceTypes::NIN_all, InputType::ControllerButton, ControllerButtons::EAST)
			},
			make_shared(Concept)("UI_BACK", string_vec{ "IN_MAIN_MENU" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));


		// DEBUG CONCEPTS
		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::e)
			},
			make_shared(Concept)("DEBUG_ADD_CHAR", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::r)
			},
			make_shared(Concept)("DEBUG_ADD_SPACE", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Mouse, InputType::MouseButton, MouseButtons::RIGHTCLICK)
			},
			make_shared(Concept)("DEBUG_LOCK_MOUSE", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Mouse, InputType::KeyboardKey, KeyboardKeys::UPARROW),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::Axis, InputAxes::RIGHTY_NEG)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_LOOK_UP", string_vec{ "IN_DEBUG" }, false)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::DOWNARROW),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::Axis, InputAxes::RIGHTY)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_LOOK_DOWN", string_vec{ "IN_DEBUG" }, false)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::LEFTARROW),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::Axis, InputAxes::RIGHTX_NEG)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_LOOK_LEFT", string_vec{ "IN_DEBUG" }, false)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::RIGHTARROW),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::Axis, InputAxes::RIGHTX)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_LOOK_RIGHT", string_vec{ "IN_DEBUG" }, false)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::w),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::Axis, InputAxes::LEFTY_NEG)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_FORWARD", string_vec{ "IN_DEBUG" }, false)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::s),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::Axis, InputAxes::LEFTY)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_BACK", string_vec{ "IN_DEBUG" }, false)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::a),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::Axis, InputAxes::LEFTX_NEG)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_LEFT", string_vec{ "IN_DEBUG" }, false)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::d),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::Axis, InputAxes::LEFTX)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_RIGHT", string_vec{ "IN_DEBUG" }, false)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::LSHIFT),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::ControllerButton, ControllerButtons::LEFTBUMPER)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_MODIFIER1", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::RSHIFT),
				make_shared(Input)(InputDeviceTypes::AllControllers, InputType::ControllerButton, ControllerButtons::RIGHTBUMPER)
			},
			make_shared(Concept)("DEBUG_MOVEMENT_MODIFIER2", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::u)
			},
			make_shared(Concept)("DEBUG_NETWORK_CONNECT", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::y)
			},
			make_shared(Concept)("DEBUG_NETWORK_SEND_TEST", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::FULLSTOP)
			},
			make_shared(Concept)("DEBUG_PHYSICS_SPEED_UP", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::COMMA)
			},
			make_shared(Concept)("DEBUG_PHYSICS_SPEED_DOWN", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		AddInputConceptBinding(InputConceptBinding(
			std::vector<shared(Input)>{
				make_shared(Input)(InputDeviceTypes::Keyboard, InputType::KeyboardKey, KeyboardKeys::p)
			},
			make_shared(Concept)("DEBUG_PHYSICS_ADD_Y_FORCE", string_vec{ "IN_DEBUG" }, true, InputRangeConversionMode::scale_to_binary_25pcIsMax)
		));

		for (const InputConceptBinding& binding : GAME::GetGameInputConceptBindings())
		{
			AddInputConceptBinding(binding);
		}

		/*for (const InputConceptBinding& binding : m_inputConceptBindings)
		{
			NOTE("");
			for (const shared(Input)& input : binding.inputs)
			{
				NOTE(InputDeviceTypes::ToString(input->inputDevice));
			}
		}*/

		m_inputStates =
		{
			{ InputDeviceTypes::Unknown,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::KeyboardKey, {}}, {InputType::MouseButton, {}}, {InputType::ControllerButton, {}}, {InputType::Axis, {}}, {InputType::ControllerTrigger, {}}
						}
					}
				}
			},
			{ InputDeviceTypes::Virtual,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::KeyboardKey, {}}, {InputType::MouseButton, {}}, {InputType::ControllerButton, {}}, {InputType::Axis, {}}, {InputType::ControllerTrigger, {}}
						}
					}
				}
			},

			{ InputDeviceTypes::Keyboard,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::KeyboardKey, {}}
						}
					}
				}
			},
			{ InputDeviceTypes::Mouse,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::MouseButton, {}}, {InputType::Axis, {}}, {InputType::OnScreenButton, {}}
						}
					}
				}
			},

			{ InputDeviceTypes::XB360,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::ControllerButton, {}}, {InputType::Axis, {}}, {InputType::ControllerTrigger, {}}
						}
					}
				}
			},
			{ InputDeviceTypes::XB1,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::ControllerButton, {}}, {InputType::Axis, {}}, {InputType::ControllerTrigger, {}}
						}
					}
				}
			},
			{ InputDeviceTypes::XBSX,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::ControllerButton, {}}, {InputType::Axis, {}}, {InputType::ControllerTrigger, {}}
						}
					}
				}
			},

			{ InputDeviceTypes::PS3,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::ControllerButton, {}}, {InputType::Axis, {}}, {InputType::ControllerTrigger, {}}
						}
					}
				}
			},
			{ InputDeviceTypes::PS4,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::ControllerButton, {}}, {InputType::Axis, {}}, {InputType::ControllerTrigger, {}}
						}
					}
				}
			},
			{ InputDeviceTypes::PS5,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::ControllerButton, {}}, {InputType::Axis, {}}, {InputType::ControllerTrigger, {}}
						}
					}
				}
			},

			{ InputDeviceTypes::NINSWpro,
				{
					{ INPUT_DEVICE_INSTANCE_INVALID,
						{
							{InputType::ControllerButton, {}}, {InputType::Axis, {}}, {InputType::ControllerTrigger, {}}
						}
					}
				}
			},
		};

		RegisterNewDevice(INPUT_DEVICE_INSTANCE_MOUSE);
		RegisterNewDevice(INPUT_DEVICE_INSTANCE_KEYBOARD);
		SearchAndRegisterNewDevices();

		NOTE("Connected input devices:");
		for (std::pair<int, shared(InputDevice)> d : m_inputDevices)
		{
			if (d.first != INPUT_DEVICE_INSTANCE_INVALID)
			{
				NOTE("\t" << d.second->m_instanceID << ": " << InputDeviceTypes::ToString(d.second->m_type) << "(" << d.second->m_instanceOfTypeID << ")");
			}
		}

		//NOTE("InputManager has completed setup!");
	}

	InputManager::~InputManager()
	{
		m_inputConceptBindings.clear();

		// detach SDL joysticks/controllers
		m_inputDevices.clear();
	}

	int InputManager::SearchAndRegisterNewDevices()
	{
		int newDevicesRegistered = 0;
		for (int j = 0; j < SDL_NumJoysticks(); j++)
		{
			int newInstanceID = SDL_JoystickGetDeviceInstanceID(j);
			if (m_inputDevices.count(newInstanceID) == 0)
			{
				RegisterNewDevice(j);
				newDevicesRegistered++;
			}
		}

		return newDevicesRegistered;
	}

	void InputManager::RegisterNewDevice(int _index)
	{
		SDL_Joystick* currentJoystick = nullptr;
		InputDeviceType gameControllerType = InputDeviceTypes::Unknown;
		int instanceID = INPUT_DEVICE_INSTANCE_INVALID;

		switch (_index)
		{
		case INPUT_DEVICE_INSTANCE_MOUSE:
			gameControllerType = InputDeviceTypes::Mouse;
			instanceID = _index;
			break;

		case INPUT_DEVICE_INSTANCE_KEYBOARD:
			gameControllerType = InputDeviceTypes::Keyboard;
			instanceID = _index;
			break;

		default:
			if (SDL_JoystickGetDeviceType(_index) == SDL_JoystickType::SDL_JOYSTICK_TYPE_GAMECONTROLLER)
			{
				currentJoystick = SDL_GameControllerGetJoystick(SDL_GameControllerOpen(_index));
				gameControllerType = 1 << SDL_GameControllerTypeForIndex(_index);
			}
			else
			{
				currentJoystick = SDL_JoystickOpen(_index);
			}

			if (currentJoystick)
			{
				instanceID = SDL_JoystickInstanceID(currentJoystick);
			}

			break;
		}


		if (instanceID != INPUT_DEVICE_INSTANCE_INVALID)
		{
			int deviceTypeInstanceID = 0;
			for (std::pair<int, shared(InputDevice)> d : m_inputDevices)
			{
				if (d.second->m_type == gameControllerType)
				{
					deviceTypeInstanceID++;
				}
			}

			m_inputDevices.insert({ instanceID, make_shared(InputDevice)(instanceID, gameControllerType, deviceTypeInstanceID, true, currentJoystick) });
			m_inputStates[gameControllerType][instanceID] = m_inputStates[gameControllerType][INPUT_DEVICE_INSTANCE_INVALID];
		}
	}

	void InputManager::DeregisterDevice(int _instanceID)
	{
		if (m_inputDevices.count(_instanceID) != 0)
		{
			m_inputDevices.erase(_instanceID);
		}
		else
		{
			WARN("Trying to deregister an input device that is not registered!");
		}
	}

	void InputManager::CreateNewOnScreenButton(int _id, const char* _name, const char* _iconPath, glm::ivec2 _pos, glm::ivec2 _size, glm::ivec2 _anchor, bool _treatHoverAsActivation, float _hoverLingerTime)
	{
		shared(GameObject) obj = make_shared(GameObject)(_name, GetManager());
		obj->AddTag("INTERNAL_ONSCREENBUTTON");
		obj->AddComponent(make_shared(TransformGUI)(obj, glm::vec3(_pos, 20), glm::vec3(0), glm::vec3(_size, 1), GUIAnchor(_anchor), GUIAnchor(_anchor)));
		obj->AddComponent(make_shared(Visualiser)(obj, "plane_vertical", _iconPath, "textured-unlit"));
		shared(InputButton) buttonComponent = make_shared(InputButton)(obj, _treatHoverAsActivation, _hoverLingerTime);
		buttonComponent->m_id = _id;
		obj->AddComponent(buttonComponent);
		m_onScreenButtons.push_back(buttonComponent);
		GetManager()->CreateGameObject(obj);

		shared(GameObject) backgroundObj = make_shared(GameObject)((std::string("back_") + _name).c_str(), GetManager(), _name);
		backgroundObj->AddTag("INTERNAL_ONSCREENBUTTONPART");
		backgroundObj->AddComponent(make_shared(TransformGUI)(backgroundObj, glm::vec3(0, 0, -1), glm::vec3(0), glm::vec3(_size, 1), GUIAnchor(GUIAnchorValue::CENTRE, GUIAnchorValue::MIDDLE), GUIAnchor(GUIAnchorValue::CENTRE, GUIAnchorValue::MIDDLE)));
		backgroundObj->AddComponent(make_shared(Visualiser)(backgroundObj, "plane_vertical", "../../data/textures/icons/background.png", "textured-unlit"));
		GetManager()->CreateGameObject(backgroundObj);

		shared(GameObject) subObj = make_shared(GameObject)((std::string("text_") + _name).c_str(), GetManager(), _name);
		subObj->AddTag("INTERNAL_ONSCREENBUTTONPART");
		subObj->AddComponent(make_shared(TransformGUI)(subObj, glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(_size, 1), GUIAnchor(GUIAnchorValue::CENTRE, GUIAnchorValue::BOTTOM), GUIAnchor(GUIAnchorValue::CENTRE, GUIAnchorValue::BOTTOM)));
		subObj->AddComponent(make_shared(Visualiser)(subObj, "plane_vertical", "../../data/textures/pixel-ascii.png", "text"));
		subObj->AddComponent(make_shared(TextBox)(subObj, _name, _size.x / 10.0f, GUIAnchor(GUIAnchorValue::CENTRE, GUIAnchorValue::BOTTOM), glm::vec3(1.0f), glm::vec4(1.0f, 1.0f, 0.75f, 0.0f)));
		GetManager()->CreateGameObject(subObj);

		if (_treatHoverAsActivation)
		{
			shared(GameObject) subObj2 = make_shared(GameObject)((std::string("progress_") + _name).c_str(), GetManager(), _name);
			subObj2->AddTag("INTERNAL_ONSCREENBUTTONPART");
			subObj2->AddComponent(make_shared(TransformGUI)(subObj2, glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(_size.x, _size.y / 10.0f, 1), GUIAnchor(GUIAnchorValue::LEFT, GUIAnchorValue::TOP), GUIAnchor(GUIAnchorValue::LEFT, GUIAnchorValue::TOP)));
			subObj2->AddComponent(make_shared(Visualiser)(subObj2, "plane_vertical", "../../data/textures/black.bmp", "textured-unlit"));
			subObj2->AddComponent(make_shared(ProgressBar)(subObj2));
			GetManager()->CreateGameObject(subObj2);
		}
	}

	void InputManager::Update()
	{

	}

	void InputManager::UpdateMouseData(int _x, int _y)
	{
		glm::vec2 windowDimensions = GetManager()->GetWindowDimensions();

		if (m_mouseLockedLastFrame)
		{
			m_mouseLockedLastFrame = false;
			m_mousePosition = (glm::vec2(_x, windowDimensions.y - _y));
			m_mouseDelta = glm::vec2(0, 0);
			return;
		}

		m_lastFrameMousePosition = (glm::vec2(glm::floor(windowDimensions.x / 2), glm::floor(windowDimensions.y / 2)));
		m_mousePosition = (glm::vec2(_x, windowDimensions.y - _y));
		m_mouseDelta = m_mousePosition - m_lastFrameMousePosition;
	}

	void InputManager::UpdateMouseData()
	{
		glm::vec2 windowDimensions = GetManager()->GetWindowDimensions();

		if (m_mouseLockedLastFrame)
		{
			m_mouseLockedLastFrame = false;
			m_mousePosition = (glm::vec2(m_mousePosition.x, m_mousePosition.y));
			m_mouseDelta = glm::vec2(0, 0);
			return;
		}

		m_lastFrameMousePosition = (glm::vec2(glm::floor(windowDimensions.x / 2), glm::floor(windowDimensions.y / 2)));
		m_mousePosition = (glm::vec2(m_mousePosition.x, m_mousePosition.y));
		m_mouseDelta = m_mousePosition - m_lastFrameMousePosition;
	}



	ConceptState InputManager::GetInputConceptState(std::string _conceptName)
	{
		if (m_concepts.count(_conceptName) != 0)
		{
			if (m_concepts[_conceptName]->IsActive())
			{
				return m_concepts[_conceptName]->GetState();
			}
			else
			{
				WARN("Trying to get the value of a concept that is inactive due to its control context! (" + _conceptName + ")");
				return ConceptState();
			}
		}
		else
		{
			WARN("Trying to get the value of a concept that doesn't exist! (" + _conceptName + ")");
			return ConceptState();
		}
	}

	shared(ControlContext) InputManager::GetControlContext(std::string _contextName)
	{
		for (shared(ControlContext) cc : m_controlContexts)
		{
			if (cc->name == _contextName)
			{
				return cc;
			}
		}
		
		WARN("Trying to access a control context that doesn't exist! (" + _contextName + ")");
		return nullptr;
	}

	void InputManager::SetControlContextActive(std::string _context, bool _newState)
	{
		if (GetControlContext(_context))
		{
			m_controlContextChanges.push_back({_context, _newState});
		}
	}

	void InputManager::IgnoreDevice(int _instanceID, bool _ignore)
	{
		if (m_inputDevices.count(_instanceID) != 0)
		{
			m_inputDevices[_instanceID]->m_isActive = !_ignore;
		}
		else
		{
			WARN("Trying to ignore an input device that is not registered!");
		}
	}

	void InputManager::AddInputConceptBinding(InputConceptBinding _newBinding)
	{
		m_concepts[_newBinding.concept->name] = _newBinding.concept;
		_newBinding.concept->Initialise(GetSelf());
		m_inputConceptBindings.push_back(_newBinding);
	}

	bool InputManager::AddInputsToConceptBinding(std::string _conceptName, std::vector<Input> _inputs)
	{
		for (size_t b = 0; b < m_inputConceptBindings.size(); b++)
		{
			if (m_inputConceptBindings[b].concept->name == _conceptName)
			{
				for (size_t i = 0; i < _inputs.size(); i++)
				{
					m_inputConceptBindings[b].inputs.push_back(make_shared(Input)(_inputs[i]));
				}
				return true;
			}
		}
		return false;
	}

	void InputManager::RegisterInput(int _deviceInstanceID, InputDeviceType _inputDevice, InputType _inputType, int _inputID, short _value)
	{
		if (m_inputDevices.count(_deviceInstanceID) != 0 && !m_inputDevices[_deviceInstanceID]->m_isActive)
		{
			return;
		}

		if (m_inputStates.at(_inputDevice).at(_deviceInstanceID).count(_inputType) == 0)
		{
			WARN("Unhandled input type");
			return;
		}

		if (m_inputStates[_inputDevice][_deviceInstanceID][_inputType][_inputID].value != _value)
		{
			// Toggle the 'changed from zero to non-zero' flag.
			// This detects when an input changes from its resting state, so can be used for toggle-mode inputs.
			if (_value > INPUT_TOGGLEMODE_INNER_DEADZONE && m_inputStates[_inputDevice][_deviceInstanceID][_inputType][_inputID].value <= INPUT_TOGGLEMODE_INNER_DEADZONE)
			{
				m_inputStates[_inputDevice][_deviceInstanceID][_inputType][_inputID].toggled = !m_inputStates[_inputDevice][_deviceInstanceID][_inputType][_inputID].toggled;
			}

			m_inputStates[_inputDevice][_deviceInstanceID][_inputType][_inputID].value = _value;
			m_inputStates[_inputDevice][_deviceInstanceID][_inputType][_inputID].changedThisFrame = true;
		}
	}

	int InputManager::ConvertInputValue(int _incomingValue, bool _incomingRangeIsBinary, bool _outgoingRangeIsBinary, InputRangeConversionMode _outgoingRangeConversionMode)
	{
		if (_incomingRangeIsBinary || _incomingRangeIsBinary == _outgoingRangeIsBinary)
		{
			return _incomingValue;
		}
		else // incoming is scale and outgoing is binary - need to use conversion modes
		{
			switch (_outgoingRangeConversionMode)
			{
			case InputRangeConversionMode::scale_to_binary_100pcIsMax:
				if (_incomingValue == INPUT_VALUE_MAX)
				{
					return INPUT_VALUE_MAX;
				}
				break;
			case InputRangeConversionMode::scale_to_binary_75pcIsMax:
				if (_incomingValue >= INPUT_VALUE_MAX * 0.75f)
				{
					return INPUT_VALUE_MAX;
				}
				break;
			case InputRangeConversionMode::scale_to_binary_50pcIsMax:
				if (_incomingValue >= INPUT_VALUE_MAX * 0.5f)
				{
					return INPUT_VALUE_MAX;
				}
				break;
			case InputRangeConversionMode::scale_to_binary_25pcIsMax:
				if (_incomingValue >= INPUT_VALUE_MAX * 0.25f)
				{
					return INPUT_VALUE_MAX;
				}
				break;
			case InputRangeConversionMode::scale_to_binary_nonZeroIsMax:
				if (_incomingValue != 0)
				{
					return INPUT_VALUE_MAX;
				}
				break;
			}

			return 0;
		}
	}

	void InputManager::FinaliseInput(InputConceptBinding _binding)
	{
		if (!_binding.concept->IsActive())
		{
			return;
		}

		int highestValueThisFrame = 0;
		bool highestValueThisFrameIsNewlyChanged = false;

		// for every binding, we need to find the highest value this frame from all of its inputs.
		for (shared(Input) input : _binding.inputs)
		{
			// cycle through all possible devices and determine if 'input' relates to any
			for (int i = 0; (InputDeviceType)(1 << i) != InputDeviceTypes::LAST; i++)
			{
				InputDeviceType thisDevice = (InputDeviceType)(1 << i);

				// does Input include thisDevice?
				if ((int)(thisDevice & input->inputDevice) > 0)
				{
					// cycle through all instances of devices of type thisDevice
					for (const std::pair<int, shared(InputDevice)>& device : m_inputDevices)
					{
						int thisDeviceInstanceID = device.first;

						// 'input' includes device of type 'thisDevice' and instance id of 'thisDeviceInstanceID'
						// only continue if there is a value stored for this device type, instance, input type, and input id.
						if (thisDeviceInstanceID != INPUT_DEVICE_INSTANCE_INVALID && m_inputStates.count(thisDevice) != 0 && m_inputStates[thisDevice].count(thisDeviceInstanceID) != 0 && m_inputStates[thisDevice][thisDeviceInstanceID].count(input->inputType) != 0 && m_inputStates[thisDevice][thisDeviceInstanceID][input->inputType].count(input->inputCode) != 0)
						{
							// account for binary inputs being used for scale concepts and vise versa
							int value = ConvertInputValue(m_inputStates[thisDevice][thisDeviceInstanceID][input->inputType][input->inputCode].value, input->isBinary, _binding.concept->idealInputRangeIsBinary, _binding.concept->inputRangeConversionMode);

							// apply input settings

							// factor in input's toggle mode
							if (input->useToggleMode)
							{
								if (m_inputStates[thisDevice][thisDeviceInstanceID][input->inputType][input->inputCode].toggled)
								{
									value = INPUT_VALUE_MAX;
								}
								else
								{
									value = 0;
								}
							}
							// factor in input's inner deadzone
							else if (value < input->inputDeadzones.x)
							{
								value = 0;
							}
							// factor in input's outer deadzone
							else if (value > INPUT_VALUE_MAX - input->inputDeadzones.y)
							{
								value = INPUT_VALUE_MAX;
							}
							else
							{
								double preciseValue = value;

								// scale the value if needed
								if (!input->passThroughRawValues && !(input->inputDeadzones.x == 0 && input->inputDeadzones.y == 0))
								{
									double validRangeSize = ((double)INPUT_VALUE_MAX - input->inputDeadzones.y) - input->inputDeadzones.x;
									double fromValidRangeToNormalised = 1 / validRangeSize;
									double normalisedValue = (preciseValue - input->inputDeadzones.x) * fromValidRangeToNormalised;
									preciseValue = normalisedValue * INPUT_VALUE_MAX;
								}

								// factor in input's sensitivity modifier
								preciseValue *= input->sensitivityMultiplier;
								value = (int)std::round(preciseValue);
							}

							if (value < 0)
							{
								value = 0;
							}
							else if (value > input->maximumValueCap)
							{
								value = input->maximumValueCap;
							}

							if (value > highestValueThisFrame)
							{
								highestValueThisFrame = value;
								highestValueThisFrameIsNewlyChanged = m_inputStates[thisDevice][thisDeviceInstanceID][input->inputType][input->inputCode].changedThisFrame;
							}
						}
					}
				}
			}
		}

		_binding.concept->timeSinceChanged += GetManager()->DeltaTime();

		if (highestValueThisFrame != _binding.concept->value)
		{
			bool newlyActiveConcept = _binding.concept->NewlyActive();
			if (!newlyActiveConcept || (newlyActiveConcept && highestValueThisFrameIsNewlyChanged))
			{
				_binding.concept->changed = true;
			}

			
			//std::cout << "Concept " << _binding.concept->name << " has changed to " << highestValueThisFrame << " (from previous of " << _binding.concept->value << ")" << std::endl;

			_binding.concept->delta = highestValueThisFrame - _binding.concept->value;
			_binding.concept->value = highestValueThisFrame;
			_binding.concept->timeSinceChanged = 0.0f;
		}
	}

	void InputManager::FinaliseInputs()
	{
		for (const InputConceptBinding& binding : m_inputConceptBindings)
		{
			if (binding.inputs != m_onScreenButtonPressBinding.inputs || binding.concept != m_onScreenButtonPressBinding.concept)
			{
				FinaliseInput(binding);
			}
		}
	}

	void InputManager::HandleSDLEvents(SDL_Event* _event, SDL_Window* _window)
	{
		if (m_mouseLocked)
		{
			SDL_ShowCursor(0);
			SDL_WarpMouseInWindow(_window, (int)glm::ceil(GetManager()->GetWindowDimensions().x / 2.0f), (int)glm::ceil(GetManager()->GetWindowDimensions().y / 2.0f));
		}
		else
		{
			SDL_ShowCursor(1);
		}

		m_mouseMovedThisFrame = false;

		while (SDL_PollEvent(_event))
		{
			// general
			if (_event->type == SDL_QUIT)
			{
				m_userWantsQuit = true;
			}

			// mouse
			else if (_event->type == SDL_MOUSEMOTION)
			{
				UpdateMouseData(_event->motion.x, _event->motion.y);

				int x = _event->motion.x;
				int y = _event->motion.y;

				if (x > INPUT_VALUE_MAX || x < 0 || y > INPUT_VALUE_MAX || y < 0)
				{
					WARN("Mouse position is outside of the acceptable range- the window is likely too big.");

					x = glm::clamp(x, 0, (int)INPUT_VALUE_MAX);
					y = glm::clamp(y, 0, (int)INPUT_VALUE_MAX);
				}

				RegisterInput(INPUT_DEVICE_INSTANCE_MOUSE, InputDeviceTypes::Mouse, InputType::Axis, InputAxes::MOUSEX, x);
				RegisterInput(INPUT_DEVICE_INSTANCE_MOUSE, InputDeviceTypes::Mouse, InputType::Axis, InputAxes::MOUSEY, y);

				m_mouseMovedThisFrame = true;
			}
			else if (_event->type == SDL_MOUSEBUTTONDOWN)
			{
				RegisterInput(INPUT_DEVICE_INSTANCE_MOUSE, InputDeviceTypes::Mouse, InputType::MouseButton, _event->button.button, INPUT_VALUE_MAX);
			}
			else if (_event->type == SDL_MOUSEBUTTONUP)
			{
				RegisterInput(INPUT_DEVICE_INSTANCE_MOUSE, InputDeviceTypes::Mouse, InputType::MouseButton, _event->button.button, 0);
			}

			// keyboard
			else if (_event->type == SDL_KEYDOWN)
			{
				if (m_inputStates[InputDeviceTypes::Keyboard][INPUT_DEVICE_INSTANCE_KEYBOARD][InputType::KeyboardKey][_event->key.keysym.sym].value == 0)
				{
					RegisterInput(INPUT_DEVICE_INSTANCE_KEYBOARD, InputDeviceTypes::Keyboard, InputType::KeyboardKey, _event->key.keysym.sym, INPUT_VALUE_MAX);
				}
			}
			else if (_event->type == SDL_KEYUP)
			{
				RegisterInput(INPUT_DEVICE_INSTANCE_KEYBOARD, InputDeviceTypes::Keyboard, InputType::KeyboardKey, _event->key.keysym.sym, 0);
			}

			// gamepad
			else if (_event->type == SDL_CONTROLLERBUTTONDOWN)
			{
				RegisterInput(_event->cbutton.which, (InputDeviceType)(1 << SDL_GameControllerGetType(SDL_GameControllerFromInstanceID(_event->cbutton.which))), InputType::ControllerButton, _event->cbutton.button, INPUT_VALUE_MAX);
			}
			else if (_event->type == SDL_CONTROLLERBUTTONUP)
			{
				RegisterInput(_event->cbutton.which, (InputDeviceType)(1 << SDL_GameControllerGetType(SDL_GameControllerFromInstanceID(_event->cbutton.which))), InputType::ControllerButton, _event->cbutton.button, 0);
			}

			// gamepad axes
			else if (_event->type == SDL_CONTROLLERAXISMOTION)
			{
				if (_event->caxis.axis == InputAxes::TRIGGERLEFT || _event->caxis.axis == InputAxes::TRIGGERRIGHT)
				{
					RegisterInput(_event->caxis.which, (InputDeviceType)(1 << SDL_GameControllerGetType(SDL_GameControllerFromInstanceID(_event->caxis.which))), InputType::ControllerTrigger, _event->caxis.axis, _event->caxis.value);
				}
				else
				{
					if (_event->caxis.value > 0)
					{
						RegisterInput(_event->caxis.which, (InputDeviceType)(1 << SDL_GameControllerGetType(SDL_GameControllerFromInstanceID(_event->caxis.which))), InputType::Axis, _event->caxis.axis, _event->caxis.value); // report true value to pos direction
						RegisterInput(_event->caxis.which, (InputDeviceType)(1 << SDL_GameControllerGetType(SDL_GameControllerFromInstanceID(_event->caxis.which))), InputType::Axis, _event->caxis.axis + InputAxes::NEG, 0); // report zero value to neg direction
					}
					else if (_event->caxis.value < 0)
					{
						short clampedValue = (_event->caxis.value <= INPUT_VALUE_MIN) ? INPUT_VALUE_MIN + 1 : _event->caxis.value;
						RegisterInput(_event->caxis.which, (InputDeviceType)(1 << SDL_GameControllerGetType(SDL_GameControllerFromInstanceID(_event->caxis.which))), InputType::Axis, _event->caxis.axis, 0); // report zero value to pos direction
						RegisterInput(_event->caxis.which, (InputDeviceType)(1 << SDL_GameControllerGetType(SDL_GameControllerFromInstanceID(_event->caxis.which))), InputType::Axis, _event->caxis.axis + InputAxes::NEG, clampedValue * -1); // report true value to neg direction
					}
					else
					{
						RegisterInput(_event->caxis.which, (InputDeviceType)(1 << SDL_GameControllerGetType(SDL_GameControllerFromInstanceID(_event->caxis.which))), InputType::Axis, _event->caxis.axis, 0); // report zero value to pos direction
						RegisterInput(_event->caxis.which, (InputDeviceType)(1 << SDL_GameControllerGetType(SDL_GameControllerFromInstanceID(_event->caxis.which))), InputType::Axis, _event->caxis.axis + InputAxes::NEG, 0); // report zero value to neg direction
					}
				}
			}
		}

		if (!m_mouseMovedThisFrame)
		{
			UpdateMouseData();
		}

		// trigger on-screen button inputs
		FinaliseInput(m_onScreenButtonPressBinding);
		for (int b = 0; b < m_onScreenButtons.size(); b++)
		{
			shared(InputButton) button = m_onScreenButtons[b].lock();
			if (button)
			{
				button->InputUpdate(m_mousePosition);
				if (button->ShouldTriggerInputPressed())
				{
					RegisterInput(INPUT_DEVICE_INSTANCE_MOUSE, InputDeviceTypes::Mouse, InputType::OnScreenButton, b, INPUT_VALUE_MAX);
				}
				else if (button->ShouldTriggerInputReleased())
				{
					RegisterInput(INPUT_DEVICE_INSTANCE_MOUSE, InputDeviceTypes::Mouse, InputType::OnScreenButton, b, 0);
				}
			}
			else
			{
				WARN("An on-screen button component has been removed!");
			}
		}

		FinaliseInputs();
	}

	void InputManager::CleanUp()
	{
		for (InputConceptBinding binding : m_inputConceptBindings)
		{
			binding.concept->changed = false;
		}

		for (auto& i1 : m_inputStates)
		{
			for (auto& i2 : i1.second)
			{
				for (auto& i3 : i2.second)
				{
					for (auto& i4 : i3.second)
					{
						if (i4.second.changedThisFrame)
						{
							i4.second.changedThisFrame = false;
						}
					}
				}
			}
		}

		for (size_t c = 0; c < m_controlContexts.size(); c++)
		{
			m_controlContexts[c]->newlyActiveThisFrame = false;
		}

		for (size_t change = 0; change < m_controlContextChanges.size(); change++)
		{
			std::string contextName = m_controlContextChanges[change].first;
			GetControlContext(contextName)->SetActiveState(m_controlContextChanges[change].second);

			//NOTE("Context " + contextName + " changed to " << (int)m_controlContextChanges[change].second);

			// go through all concepts using this context, then through all of its inputs, and set any toggle-mode inputs back to inactive
			for (const InputConceptBinding& binding : m_inputConceptBindings)
			{
				shared(Concept) concept = binding.concept;
				for (weak(ControlContext) currentContext : concept->controlContexts)
				{
					if (currentContext.lock() && currentContext.lock()->name == contextName)
					{
						for (shared(Input) input : binding.inputs)
						{
							if (input->useToggleMode)
							{
								for (int i = 0; (InputDeviceType)(1 << i) != InputDeviceTypes::LAST; i++)
								{
									InputDeviceType thisDevice = (InputDeviceType)(1 << i);

									// does Input include thisDevice?
									if ((int)(thisDevice & input->inputDevice) > 0)
									{
										// cycle through all instances of devices of type thisDevice
										for (const std::pair<int, shared(InputDevice)>& device : m_inputDevices)
										{
											int thisDeviceInstanceID = device.first;

											// only continue if there is a value stored for this device type, instance, input type, and input id.
											if (thisDeviceInstanceID != INPUT_DEVICE_INSTANCE_INVALID && m_inputStates.count(thisDevice) != 0 && m_inputStates[thisDevice].count(thisDeviceInstanceID) != 0 && m_inputStates[thisDevice][thisDeviceInstanceID].count(input->inputType) != 0 && m_inputStates[thisDevice][thisDeviceInstanceID][input->inputType].count(input->inputCode) != 0)
											{
												m_inputStates[thisDevice][thisDeviceInstanceID][input->inputType][input->inputCode].toggled = false;
											}
										}
									}
								}
							}
							if (input->inputType == InputType::OnScreenButton)
							{
								shared(InputButton) button = m_onScreenButtons.at(input->inputCode).lock();
								if (button)
								{
									button->GetObject()->GetComponent<Visualiser>()->SetVisible(m_controlContextChanges[change].second);
									button->ResetState();
								}
							}
						}
						break;
					}
				}
			}
		}
		m_controlContextChanges.clear();
	}

} // namespace MYENGINE
