#include "ControlContext.h"

#include "engine/managers/Manager.h"
#include "engine/managers/SettingsManager.h"
#include "engine/managers/InputManager.h"

#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_gamecontroller.h>

namespace MYENGINE
{

	ControlContextSettings::ControlContextSettings()
	{

	}

	void ControlContextSettings::Initialise(std::string _contextName, shared(SettingsManager) _settingsManager)
	{
		std::string contextAddition = "";
		if (!_contextName.empty())
		{
			contextAddition = "__" + _contextName;
		}
		_settingsManager->GetSettingData("gamespeed" + contextAddition, m_Gamespeed);
		_settingsManager->GetSettingData("textcolour" + contextAddition, m_TextColour);
		_settingsManager->GetSettingData("textbackground" + contextAddition, m_TextBackground);
	}

#define ACTIVATE_SETTING(alias) if (Get##alias()) { _manager->GetCurrentUserSettings().Get##alias(&_tempSettings.m_##alias); _manager->m_userSettings->m_##alias = m_##alias; }

	void ControlContextSettings::Activate(shared(Manager) _manager, ControlContextSettings& _tempSettings)
	{
		ACTIVATE_SETTING(Gamespeed)
		ACTIVATE_SETTING(TextColour)
		ACTIVATE_SETTING(TextBackground)
	}

#define DEACTIVATE_SETTING(alias) if (Get##alias()) { _manager->m_userSettings->m_##alias = _tempSettings.m_##alias; }

	void ControlContextSettings::Deactivate(shared(Manager) _manager, ControlContextSettings& _tempSettings)
	{
		DEACTIVATE_SETTING(Gamespeed)
		DEACTIVATE_SETTING(TextColour)
		DEACTIVATE_SETTING(TextBackground)
	}


	ControlContext::ControlContext(const char* _name, const char* _parentName, bool _startActive)
	{
		name = _name;
		m_parentName = _parentName;
		initialisedAsActive = _startActive;
		isActive = false;
		isActiveButParentNot = false;
		newlyActiveThisFrame = false;

		m_settings = ControlContextSettings();
		m_tempSettings = ControlContextSettings();
	}

	void ControlContext::Initialise(shared(InputManager) _inputManager, shared(Manager) _manager)
	{
		m_inputManager = _inputManager;
		m_manager = _manager;

		m_settings.Initialise(name, _manager->GetSettingsManager());

		isActive = initialisedAsActive;

		if (!m_parentName.empty())
		{
			shared(ControlContext) parent = _manager->GetInputManager()->GetControlContext(m_parentName);
			if (parent)
			{
				parent->m_childContextNames.push_back(name);

				if (isActive && !parent->IsActive())
				{
					isActiveButParentNot = true;
					isActive = false;
				}
			}
		}

		if (isActive)
		{
			Activate();
		}
	}

	bool ControlContext::IsActive()
	{
		return isActive;
	}

	void ControlContext::SetActiveState(bool _newState, bool _fromParent)
	{
		if (_fromParent)
		{
			//std::cout << "Child ControlContext " << name << " changing from " << isActive << " to ";

			// if parent has been activated and if this context is locally activated, activate it properly again.
			if (_newState && isActiveButParentNot)
			{
				isActiveButParentNot = false;
				isActive = true;
			}
			// if parent has been deactivated and if this context is activated, deactivate it but mark it as locally activated.
			else if (!_newState && isActive)
			{
				isActiveButParentNot = true;
				isActive = false;
			}
			else if (_newState && !isActive)
			{
				return;
			}

			//std::cout << isActive << std::endl;
		}
		else
		{
			if (_newState == isActive)
			{
				return;
			}

			//NOTE("ControlContext " << name << " changing from " << isActive << " to " << _newState << "!");
			isActive = _newState;
		}

		if (isActive)
		{
			// newly activated
			Activate();
		}

		for (std::string s : m_childContextNames)
		{
			shared(ControlContext) child = m_inputManager.lock()->GetControlContext(s);
			if (child)
			{
				child->SetActiveState(_newState, true);
			}
		}

		if (!isActive)
		{
			// newly deactivated
			Deactivate();
		}
	}

	void ControlContext::Activate()
	{
		newlyActiveThisFrame = true;
		m_settings.Activate(m_manager.lock(), m_tempSettings);
	}

	void ControlContext::Deactivate()
	{
		m_settings.Deactivate(m_manager.lock(), m_tempSettings);
	}

} // namespace MYENGINE
