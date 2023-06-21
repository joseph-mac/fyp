#ifndef _MYENGINE_SETTINGSMANAGER_H_
#define _MYENGINE_SETTINGSMANAGER_H_

#include "engine/Defines.h"
#include "engine/managers/SubManager.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>
#include <vector>
#include <map>

#define NEW_SETTING_TYPE(t, alias) virtual bool GetData(t& _resultAddr) { WARN("The current setting was not stored as type " << #alias); return false; }

typedef int InputID;

namespace MYENGINE
{

	struct Manager;
	struct InputManager;
	struct Input;

	struct Setting
	{
		Setting() {}

		NEW_SETTING_TYPE(int, Int)
		NEW_SETTING_TYPE(float, Float)
		NEW_SETTING_TYPE(bool, Bool)
		NEW_SETTING_TYPE(glm::vec2, Vec2)
		NEW_SETTING_TYPE(glm::vec3, Vec3)
		NEW_SETTING_TYPE(glm::vec4, Vec4)
		NEW_SETTING_TYPE(std::vector<Input>, Inputs)
	};

	template <class T>
	struct _Setting : public Setting
	{
		_Setting(T _data)
		{
			m_data = _data;
		}

		T m_data;

		bool GetData(T& _resultAddr) override
		{
			_resultAddr = m_data;
			return true;
		}
	};

	struct SettingsManager : public SubManager
	{
	public:
		SettingsManager(weak(Manager) _manager);
		~SettingsManager();

		/**
		* Load Settings into the engine from a settings text file.
		*
		* @param _filePath : The file path of the settings text file.
		* @param _contextBased : Whether the given settings file will contain control context details.
		* @param _inputManager : If provided, the given settings file will be treated as containing input binding details.
		* @returns Whether the loading was successful.
		**/
		bool LoadFromFile(const char* _filePath, bool _contextBased = false, shared(InputManager) _inputManager = nullptr);

		/**
		* Get a setting's data by name
		**/
		template <class T> bool GetSettingData(std::string _name, T& _output)
		{
			if (m_settings.count(_name) < 1)
			{
				return false;
			}

			return m_settings[_name]->GetData(_output);
		}

	private:
		std::map<std::string, shared(Setting)> m_settings;
	};


	namespace UTIL
	{

		struct Maps
		{
			static std::map<std::string, InputID> inputIdMap;
		};

	} // namespace UTIL

} // namespace MYENGINE

#endif
