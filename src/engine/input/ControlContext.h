#ifndef _MYENGINE_CONTROLCONTEXT_H_
#define _MYENGINE_CONTROLCONTEXT_H_

#include "engine/Defines.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>
#include <vector>

#define DEF_SETTING(type, alias, initValue, acc) private: type m_##alias = initValue; public: bool Get##alias(type* _output = nullptr) { if (m_##alias##acc >= 0) { if (_output) { *_output = m_##alias; } return true; } return false; }

namespace MYENGINE
{

	struct Manager;
	struct SettingsManager;
	struct InputManager;

	struct ControlContextSettings
	{
	public:
		ControlContextSettings();

		void Initialise(std::string _contextName, shared(SettingsManager) _settingsManager);

		void Activate(shared(Manager) _manager, ControlContextSettings& _tempSettings);
		void Deactivate(shared(Manager) _manager, ControlContextSettings& _tempSettings);

		DEF_SETTING(float, Gamespeed, -1,)
		DEF_SETTING(glm::vec3, TextColour, glm::vec3(-1.0f, 0.0f, 0.0f), .x)
		DEF_SETTING(glm::vec4, TextBackground, glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f), .x)

		friend ControlContextSettings;
	};

	struct ControlContext
	{
	public:
		ControlContext(const char* _name, const char* _parentName, bool _startActive);

		bool IsActive();

	private:
		friend struct Concept;
		friend struct InputManager;

		std::string m_parentName;
		std::vector<std::string> m_childContextNames;

		void Initialise(shared(InputManager) _inputManager, shared(Manager) _manager);
		void SetActiveState(bool _newState, bool _fromParent = false);

		void Activate();
		void Deactivate();

		// fixed properties
		std::string name;

		// state properties
		bool initialisedAsActive;
		bool isActive;
		bool isActiveButParentNot;
		bool newlyActiveThisFrame;

		// user settings
		ControlContextSettings m_settings;
		ControlContextSettings m_tempSettings;

		weak(Manager) m_manager;
		weak(InputManager) m_inputManager;
	};

} // namespace MYENGINE

#endif
