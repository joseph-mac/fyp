#ifndef _MYENGINE_MANAGER_H_
#define _MYENGINE_MANAGER_H_

#define MAX_LIGHT_COUNT 5

#include "engine/Defines.h"
#include "engine/gui/GUIDefs.h"
#include "engine/input/ControlContext.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <string>

namespace MYENGINE
{

	class Core;
	struct GameObject;
	struct SettingsManager;
	struct PhysicsManager;
	struct InputManager;
	struct ResourceManager;
	struct VertexArray;
	struct Texture;
	struct ShaderProgram;
	struct Camera;
	struct LightSource;


	struct Manager
	{
	public:
		Manager();
		~Manager();

		/**
		* Register a new GameObject with Manager.
		*
		* @param _newObject : The GameObject reference to register.
		**/
		void CreateGameObject(shared(GameObject) _newObject);

		/**
		* Mark a GameObject for removal at the end of the current frame. It will be set as 'decaying' and turned invisible immediately.
		*
		* @param _object : The GameObject to remove.
		**/
		void RemoveGameObject(shared(GameObject) _object);

		/**
		* Retrieve a GameObject by its name. If there are multiple GameObjects with the same name, the first to be registered will be returned.
		*
		* @param _name : The name of the GameObject to search for.
		* @returns the found GameObject, else nullptr.
		**/
		shared(GameObject) GetGameObject(std::string _name);

		/**
		* Retrieve a vector of GameObjects possessing the given tag.
		*
		* @param _tag : The tag to search for in GameObjects.
		* @returns a vector of GameObjects possessing the given tag.
		**/
		std::vector<shared(GameObject)> GetGameObjectsWithTag(std::string _tag);

		/**
		* @returns the time elapsed since the last frame in seconds.
		**/
		float DeltaTime() { return m_deltaTime * GetAbsoluteGameSpeed(); }

		/**
		* NOTE: This function ignores the current gamespeed, and so should only be used in select circumstances.
		* 
		* @returns the time elapsed since the last frame in seconds.
		**/
		float RawDeltaTime() { return m_deltaTime; }

		/**
		* @returns the current gamespeed.
		**/
		float GetAbsoluteGameSpeed() { float userGamespeed = 1; m_userSettings->GetGamespeed(&userGamespeed); return m_gameSpeed * userGamespeed; }

		/**
		* Set the current gamespeed.
		* 
		* @param _newSpeed : The new speed multiplier to set.
		**/
		void SetGameSpeed(float _newSpeed) { m_gameSpeed = _newSpeed; NOTE("Gamespeed set to " << _newSpeed); }

		/**
		* @returns the Manager's submanager, SettingsManager.
		**/
		shared(SettingsManager) GetSettingsManager() { return m_settingsManager; }

		/**
		* @returns the Manager's submanager, PhysicsManager.
		**/
		shared(PhysicsManager) GetPhysicsManager() { return m_physicsManager; }

		/**
		* @returns the Manager's submanager, InputManager.
		**/
		shared(InputManager) GetInputManager() { return m_inputManager; }

		/**
		* @returns the Manager's submanager, ResourceManager.
		**/
		shared(ResourceManager) GetResourceManager() { return m_resourceManager; }

		/**
		* @returns the number of LightSource components within the scene.
		**/
		int GetLightCount() { return (int)m_lights.size(); }

		/**
		* UNSAFE
		* 
		* @param _pos : The position of the desired LightSource in Manager's internal list
		* @returns the LightSource at _pos.
		**/
		shared(LightSource) GetLight(int _pos) { return m_lights.at(_pos).lock(); }

		/**
		* Register a new LightSource component for the Manager to utilise.
		*
		* @param _newComponent : the component for the Manager to utilise.
		**/
		void RegisterComponent(weak(LightSource) _light) { m_lights.push_back(_light); }

		/**
		* Register a new Camera component for the Manager to utilise.
		*
		* @param _newComponent : the component for the Manager to utilise.
		**/
		void RegisterComponent(weak(Camera) _camera) { m_cameras.push_back(_camera); }

		/**
		* UNSAFE
		*
		* @param _pos : The position of the desired Camera in Manager's internal list
		* @returns the Manager at _pos.
		**/
		shared(Camera) GetCamera(int _pos) { return m_cameras.at(_pos).lock(); }

		/**
		* @returns the number of Camera components within the scene.
		**/
		int GetCameraCount() { return (int)m_cameras.size(); }

		/**
		* UNSAFE 
		* Set the main camera to be used by Manager, by passing in the position of the desired Camera in Manager's internal list
		* 
		* @param _cameraID : The ID of the new camera.
		**/
		void SetMainCamera(int _cameraID) { m_mainCameraID = _cameraID; }

		/**
		* @returns the main camera being used by Manager.
		**/
		shared(Camera) GetMainCamera() { return m_cameras.at(m_mainCameraID).lock(); }

		/**
		* @returns whether any window properties have changed this frame (e.g. dimensions).
		**/
		bool WindowHasChanged() { return m_windowChanged; }

		/**
		* @returns a vec2 of the current window's dimensions.
		**/
		glm::vec2 GetWindowDimensions() { return m_windowDimensions; }

		/**
		* @returns a vec2 of the given GUIAnchor's actual position.
		**/
		glm::vec2 GetGUIAnchorPosition(GUIAnchor _anchor) { return glm::vec2(m_GUIAnchorPositions[(int)_anchor.x].x, m_GUIAnchorPositions[(int)_anchor.y].y); }

		/**
		* @returns the orthographic matrix calculated using the current window's dimensions.
		**/
		glm::mat4 GetOrthoMatrix() { return m_orthoMatrix; }

		/**
		* @returns the currently applied settings based on user configuration.
		**/
		ControlContextSettings GetCurrentUserSettings() { return *m_userSettings.get(); }

	private:
		friend Core;
		friend struct ControlContext;
		friend ControlContextSettings;

		void Initialise(weak(Manager) _managerSelf);

		void UpdateWindowDimensions(int _windowWidth, int _windowHeight);
		void UpdateDeltaTime(int _currentTickAge);

		void UpdateAll();
		void DrawAll();
		void CleanUp();

		int m_lastFrameTickAge;
		float m_deltaTime;
		float m_gameSpeed;

		bool m_windowChanged = false;
		glm::ivec2 m_windowDimensions;
		glm::vec2 m_GUIAnchorPositions[3] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
		float m_GUIscale;

		std::vector<shared(GameObject)> m_gameObjects;
		std::vector<shared(GameObject)> m_decayingGameObjects;

		std::vector<std::string> m_gameObjectTags;

		shared(SettingsManager) m_settingsManager;
		shared(PhysicsManager) m_physicsManager;
		shared(InputManager) m_inputManager;
		shared(ResourceManager) m_resourceManager;

		int m_mainCameraID;
		std::vector<weak(Camera)> m_cameras;
		std::vector<weak(LightSource)> m_lights;

		glm::mat4 m_orthoMatrix;

		// user settings
		shared(ControlContextSettings) m_userSettings;

		weak(Manager) m_self;
	};

} // namespace MYENGINE

#endif
