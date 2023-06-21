#ifndef _MYENGINE_INPUTMANAGER_H_
#define _MYENGINE_INPUTMANAGER_H_

#include "engine/Defines.h"
#include "engine/managers/SubManager.h"
#include "engine/input/InputDefs.h"
#include "engine/input/InputSystemDefs.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_vec2.hpp>

#include <vector>
#include <map>

union SDL_Event;
struct SDL_Window;
typedef struct _SDL_Joystick SDL_Joystick;

namespace MYENGINE
{

	struct Manager;
	struct InputButton;

	struct InputManager : public SubManager
	{
	public:
		InputManager(weak(Manager) manager);
		~InputManager();

		/**
		* Lock or unlock the user's ability to use the mouse cursor.
		*
		* @param _state : the desired state.
		**/
		void LockMouse(bool _state) { m_mouseLocked = _state; m_mouseLockedLastFrame = true; }

		/**
		* @returns whether the user's ability to use the mouse cursor is locked
		**/
		bool MouseLocked() { return m_mouseLocked; }

		/**
		* @returns the position of the user's mouse cursor.
		**/
		glm::vec2 GetMousePos() { return m_mousePosition; }

		/**
		* @returns the difference between the last frame's mouse position and this frame's.
		**/
		glm::vec2 GetMouseDelta() { return m_mouseDelta; }

		/**
		* @returns a struct containing details of the given concept's state
		**/
		ConceptState GetInputConceptState(std::string _conceptName);

		/*
		* Get the control context with the specified name.
		*
		* @param _contextName : the name of the control context to fetch.
		* 
		* @returns the control context.
		*/
		shared(ControlContext) GetControlContext(std::string _contextName);

		/*
		* Set a named control context to active or inactive.
		* 
		* @param _context : the name of the control context.
		* @param _newState : the desired state of activation.
		*/
		void SetControlContextActive(std::string _context, bool _newState);

		/**
		* Flag or de-flag a specific input device to ignore input from.
		* 
		* @param _instanceID : the input device's instance ID.
		* @param _ignore : the new state for the device.
		**/
		void IgnoreDevice(int _instanceID, bool _ignore);

		/*
		* Safely end the program.
		*/
		void QuitProgram() { m_userWantsQuit = true; }

	private:
		friend class Core;
		friend Manager;
		friend SettingsManager;
		friend struct Concept;
		friend struct OnScreenButton;

		void Initialise(shared(InputManager) _inputManager);
		void Update();
		void HandleSDLEvents(SDL_Event* _event, SDL_Window* _window);

		/**
		* Search the system for any unregistered input devices, and register them.
		* 
		* @returns the number of new devices registered.
		**/
		int SearchAndRegisterNewDevices();
		void RegisterNewDevice(int _index);
		void DeregisterDevice(int _instanceID);
		void CreateNewOnScreenButton(int _id, const char* _name, const char* _iconPath, glm::ivec2 _pos, glm::ivec2 _size, glm::ivec2 _anchor, bool _treatHoverAsActivation, float _hoverLingerTime = 0.0f);

		bool GetUserWantsQuit() { return m_userWantsQuit; }

		void AddInputConceptBinding(InputConceptBinding _newBinding);
		bool AddInputsToConceptBinding(std::string _conceptName, std::vector<Input> _inputs);
		void RegisterInput(int _deviceInstanceID, InputDeviceType _inputDevice, InputType _inputType, int _inputID, short _value);
		int ConvertInputValue(int _incomingValue, bool _incomingRangeIsBinary, bool _outgoingRangeIsBinary, InputRangeConversionMode _outgoingRangeConversionMode);
		void FinaliseInput(InputConceptBinding _binding);
		void FinaliseInputs();

		void UpdateMouseData();
		void UpdateMouseData(int _x, int _y);

		void CleanUp();

		// general
		bool m_userWantsQuit;
		std::vector<shared(ControlContext)> m_controlContexts;;
		std::vector<std::pair<std::string, bool>> m_controlContextChanges = {};

		// mouse
		bool m_mouseLocked;
		bool m_mouseLockedLastFrame;
		bool m_mouseMovedThisFrame;
		glm::vec2 m_mousePosition;
		glm::vec2 m_mouseDelta;
		glm::vec2 m_lastFrameMousePosition;

		// concept bindings
		std::map<std::string, shared(Concept)> m_concepts;
		InputConceptBinding m_onScreenButtonPressBinding = InputConceptBinding({}, nullptr);
		std::vector<InputConceptBinding> m_inputConceptBindings;

		// input devices

		// Each entry is a currently connected device, accessed by its instanceID. Stores its device type, device type instance ID, SDL handle (if appropriate), and whether it is active (not ignored).
		std::map<int, shared(InputDevice)> m_inputDevices;
		std::vector<weak(InputButton)> m_onScreenButtons;

		struct InputState
		{
			short value = 0;
			bool toggled = false;
			bool changedThisFrame = false;
		};
		// input states (device, device instance id, type, id, [InputState])
		std::map<InputDeviceType, std::map<int, std::map<InputType, std::map<int, InputState>>>> m_inputStates;

		shared(InputManager) GetSelf() { return m_self.lock(); }
		weak(InputManager) m_self;
	};

} // namespace MYENGINE

#endif
