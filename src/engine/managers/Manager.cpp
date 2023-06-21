#include "Manager.h"

#include "engine/Exception.h"
#include "engine/GameObject.h"
#include "engine/managers/SettingsManager.h"
#include "engine/managers/PhysicsManager.h"
#include "engine/managers/InputManager.h"
#include "engine/managers/ResourceManager.h"
#include "engine/Component.h"
#include "engine/components/Component_Types.h"
#include "engine/components/physics/PhysicsComponents.h"
#include "engine/components/physics/ColliderComponents.h"
#include "engine/components/gui/GUIComponents.h"
#include "engine/graphics/Texture.h"
#include "engine/graphics/ShaderProgram.h"
#include "engine/graphics/Primitives.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>

#include <fstream>

namespace MYENGINE
{

	Manager::Manager()
	{
		m_lastFrameTickAge = 0;
		m_deltaTime = 0.0f;
		m_gameSpeed = 1;

		m_windowDimensions = glm::ivec2(0, 0);
		m_GUIscale = 1;

		m_gameObjects = std::vector<shared(GameObject)>{};
		m_gameObjectTags = std::vector<std::string>{};

		m_mainCameraID = 0;
		m_cameras = std::vector<weak(Camera)>{};
		m_lights = std::vector<weak(LightSource)>{};

		m_orthoMatrix = glm::mat4(1.0f);

		m_userSettings = make_shared(ControlContextSettings)();
	}

	Manager::~Manager()
	{
		//std::cout << "Manager destructing..." << std::endl;

		m_gameObjects.clear();

		//std::cout << "Manager destructed!" << std::endl;
	}

	void Manager::Initialise(weak(Manager) _managerSelf)
	{
		m_self = _managerSelf;

		m_resourceManager = make_shared(ResourceManager)(m_self);

		// init engine-level vert arrays
		Primitives::CreatePlane(m_self);
		Primitives::CreateVerticalPlane(m_self);
		Primitives::CreateTextureFlippedVerticalPlane(m_self);
		Primitives::CreateBox(m_self);
		Primitives::CreateSkyBox(m_self);
		//Primitives::CreateTriangle(m_self);
		//Primitives::CreateEqTriangle(m_self);

		m_settingsManager = make_shared(SettingsManager)(m_self);
		m_settingsManager->LoadFromFile("../../savedata/settings/generic.txt");
		m_settingsManager->LoadFromFile("../../savedata/settings/generic_per_context.txt", true);
		m_userSettings->Initialise("", m_settingsManager);

		m_physicsManager = make_shared(PhysicsManager)(m_self);

		m_inputManager = make_shared(InputManager)(m_self);
		m_inputManager->Initialise(m_inputManager);
		m_settingsManager->LoadFromFile("../../savedata/settings/input_bindings.txt", true, m_inputManager);
	}

	void Manager::CreateGameObject(shared(GameObject) _newObject)
	{
		m_gameObjects.push_back(_newObject);
		_newObject->Initialise(_newObject);
	}

	void Manager::RemoveGameObject(shared(GameObject) _object)
	{
		m_decayingGameObjects.push_back(_object);
		if (shared(Visualiser) vis = _object->GetComponent<Visualiser>())
		{
			vis->SetVisible(false);
		}
		_object->m_decaying = true;
	}

	shared(GameObject) Manager::GetGameObject(std::string _name)
	{
		for (int o = 0; o < m_gameObjects.size(); o++)
		{
			if (m_gameObjects[o]->m_name == _name)
			{
				return m_gameObjects[o];
			}
		}
		return nullptr;
	}

	std::vector<shared(GameObject)> Manager::GetGameObjectsWithTag(std::string _tag)
	{
		std::vector<shared(GameObject)> results = {};

		for (size_t o = 0; o < m_gameObjects.size(); o++)
		{
			if (m_gameObjects[o]->HasTag(_tag))
			{
				results.push_back(m_gameObjects[o]);
			}
		}

		return results;
	}

	void Manager::UpdateWindowDimensions(int _windowWidth, int _windowHeight)
	{
		if (m_windowDimensions.x != _windowWidth || m_windowDimensions.y != _windowHeight)
		{
			m_windowChanged = true;

			m_windowDimensions = glm::vec2(_windowWidth, _windowHeight);

			m_GUIAnchorPositions[(int)GUIAnchorValue::LEFT].x = GUIAnchorValues[(int)GUIAnchorValue::LEFT] * m_windowDimensions.x;
			m_GUIAnchorPositions[(int)GUIAnchorValue::CENTRE].x = GUIAnchorValues[(int)GUIAnchorValue::CENTRE] * m_windowDimensions.x;
			m_GUIAnchorPositions[(int)GUIAnchorValue::RIGHT].x = GUIAnchorValues[(int)GUIAnchorValue::RIGHT] * m_windowDimensions.x;

			m_GUIAnchorPositions[(int)GUIAnchorValue::TOP].y = GUIAnchorValues[(int)GUIAnchorValue::TOP] * m_windowDimensions.y;
			m_GUIAnchorPositions[(int)GUIAnchorValue::MIDDLE].y = GUIAnchorValues[(int)GUIAnchorValue::MIDDLE] * m_windowDimensions.y;
			m_GUIAnchorPositions[(int)GUIAnchorValue::BOTTOM].y = GUIAnchorValues[(int)GUIAnchorValue::BOTTOM] * m_windowDimensions.y;

			m_orthoMatrix = glm::ortho(0.0f, (float)m_windowDimensions.x, 0.0f, (float)m_windowDimensions.y, -50.0f, 50.0f);

			for (int c = 0; c < m_cameras.size(); c++)
			{
				m_cameras.at(c).lock()->UpdateProjectionMatrix(_windowWidth, _windowHeight);
			}
		}
		else
		{
			m_windowChanged = false;
		}
	}

	void Manager::UpdateDeltaTime(int _currentTickAge)
	{
		m_deltaTime = (_currentTickAge - m_lastFrameTickAge) / 1000.0f;
		m_lastFrameTickAge = _currentTickAge;
	}

	void Manager::UpdateAll()
	{
		m_inputManager->Update();

		if (m_cameras.size() < 1)
		{
			throw Exception("No cameras were found for an Update pass. This is not supported behaviour!");
		}

		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			if (m_gameObjects.at(i) != nullptr)
			{
				m_gameObjects.at(i)->UpdateEarly();
			}
		}

		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			if (m_gameObjects.at(i) != nullptr)
			{
				m_gameObjects.at(i)->Update();
			}
		}

		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			if (m_gameObjects.at(i) != nullptr)
			{
				m_gameObjects.at(i)->UpdateGUI();
			}
		}

		m_physicsManager->UpdateAll();
	}

	float SumVec3(glm::vec3 _vec)
	{
		float result = 0.0f;
		result += _vec.x;
		result += _vec.y;
		result += _vec.z;
		return result;
	}

	/**
	* Utility function to sort objects by distance to given point, considering the given axes.
	**/
	void SortObjectsByDistanceToPoint(std::vector<shared(GameObject)>& _objects, glm::vec3 _point, glm::vec3 _axes = glm::vec3(1.0f), bool _reverse = false)
	{
		glm::vec3 p = _point * _axes;

		int changes = 0;
		do
		{
			changes = 0;
			for (size_t go = 0; go + 1 < _objects.size(); go++)
			{
				shared(GameObject) thisObj = _objects[go];
				glm::vec3 thisObjDistVec = p - (thisObj->GetTransform()->GetAbsolutePosition() * _axes);
				float thisObjDistFromPoint = SumVec3(thisObjDistVec * thisObjDistVec);

				shared(GameObject) nextObj = _objects[go + 1];
				glm::vec3 nextObjDistVec = p - (nextObj->GetTransform()->GetAbsolutePosition() * _axes);
				float nextObjDistFromPoint = SumVec3(nextObjDistVec * nextObjDistVec);

				if (_reverse)
				{
					thisObjDistFromPoint *= -1.0f;
					nextObjDistFromPoint *= -1.0f;
				}

				if (thisObjDistFromPoint < nextObjDistFromPoint)
				{
					_objects[go].swap(_objects[go + 1]);
					changes++;
				}
			}

		} while (changes > 0);
	}

	void Manager::DrawAll()
	{
		// sort objects into groups, which will determine the order in which they are rendered
		std::vector<shared(GameObject)> opaqueObjects = {};
		std::vector<shared(GameObject)> transparentObjects = {};

		std::vector<shared(GameObject)> opaqueGUIobjects = {};
		std::vector<shared(GameObject)> transparentGUIobjects = {};

		std::vector<shared(GameObject)> sortedObjects = {};

		for (int go = 0; go < m_gameObjects.size(); go++)
		{
			shared(GameObject) thisObj = m_gameObjects[go];
			shared(Visualiser) thisVis = thisObj->GetComponent<Visualiser>();
			if (thisVis && thisVis->GetTexture() && thisObj->GetTransform() && thisObj->GetName() != "skybox")
			{
				if (!thisObj->GetTransform()->UsingPerspective())
				{
					if (thisVis->GetTexture()->HasTransparency())
					{
						transparentGUIobjects.push_back(thisObj);
					}
					else
					{
						opaqueGUIobjects.push_back(thisObj);
					}
				}
				else if (!thisVis->GetTexture()->HasTransparency())
				{
					opaqueObjects.push_back(thisObj);
				}
				else
				{
					transparentObjects.push_back(thisObj);
				}
			}
		}

		// for transparent objects, the order in which they are rendered is important: further objects must be rendered before closer ones
		// transparent GUI objects are always relative to z position 0, so do not need to be re-sorted for each camera being used
		SortObjectsByDistanceToPoint(transparentGUIobjects, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), true);

		sortedObjects.insert(sortedObjects.end(), opaqueObjects.begin(), opaqueObjects.end());
		sortedObjects.insert(sortedObjects.end(), opaqueGUIobjects.begin(), opaqueGUIobjects.end());
		sortedObjects.push_back(GetGameObject("skybox"));
		size_t transparentOffset = sortedObjects.size();
		sortedObjects.insert(sortedObjects.end(), transparentObjects.begin(), transparentObjects.end());
		sortedObjects.insert(sortedObjects.end(), transparentGUIobjects.begin(), transparentGUIobjects.end());

		// render to all RenderTextures
		for (int rt = 0; rt < GetResourceManager()->m_renderTextures.size(); rt++)
		{
			shared(RenderTexture) tex = GetResourceManager()->m_renderTextures.at(rt).lock();
			shared(Camera) texCam = tex->GetLinkedCamera().lock();

			glViewport(0, 0, (GLsizei)texCam->GetViewSize().x, (GLsizei)texCam->GetViewSize().y);

			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glBindFramebuffer(GL_FRAMEBUFFER, tex->GetFrameBufferID());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			SortObjectsByDistanceToPoint(transparentObjects, texCam->GetObject()->GetTransform()->GetAbsolutePosition());
			std::copy(transparentObjects.begin(), transparentObjects.end(), sortedObjects.begin() + transparentOffset);

			for (int go = 0; go < sortedObjects.size(); go++)
			{
				if (sortedObjects.at(go) != nullptr)
				{
					shared(GameObject) obj = sortedObjects.at(go);
					shared(Visualiser) goVisualiser = obj->GetComponent<Visualiser>();
					if (goVisualiser && (!obj->GetTransform() || (obj->GetTransform() && obj->GetTransform()->UsingPerspective())) && goVisualiser->GetTexture()->GetId() != tex->GetId()) // this line stops GUI objects from being drawn to render textures, as well as objects using the current render texture
					{
						shared(ShaderProgram) shaderProgram = tex->GetShaderProgram();
						if (!shaderProgram)
						{
							shaderProgram = goVisualiser->GetShaderProgram();
						}

						obj->Draw(texCam, shaderProgram->GetId());
					}
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shared(Camera) mainCam = m_cameras.at(m_mainCameraID).lock();
		glViewport(0, 0, (GLsizei)mainCam->GetViewSize().x, (GLsizei)mainCam->GetViewSize().y);

		SortObjectsByDistanceToPoint(transparentObjects, mainCam->GetObject()->GetTransform()->GetAbsolutePosition());
		std::copy(transparentObjects.begin(), transparentObjects.end(), sortedObjects.begin() + transparentOffset);
		for (int i = 0; i < sortedObjects.size(); i++)
		{
			if (sortedObjects.at(i) != nullptr)
			{
				sortedObjects.at(i)->Draw();
			}
		}
	}

	void Manager::CleanUp()
	{
		m_inputManager->CleanUp();

		while (!m_decayingGameObjects.empty())
		{
			shared(GameObject) objToRemove = m_decayingGameObjects.back();

			//std::cout << "CLEANUP: Destroying object " << objToRemove->m_name << std::endl;

			auto removed = std::remove(m_gameObjects.begin(), m_gameObjects.end(), objToRemove);
			m_gameObjects.erase(removed, m_gameObjects.end());
			m_decayingGameObjects.pop_back();
		}
	}

} // namespace MYENGINE
