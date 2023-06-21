#include "GameObject.h"

#include "engine/managers/Manager.h"
#include "engine/input/ControlContext.h"
#include "engine/managers/SettingsManager.h"
#include "engine/managers/ResourceManager.h"
#include "engine/components/Component_Types.h"
#include "engine/components/gui/GUIComponents.h"
#include "engine/components/physics/PhysicsComponents.h"
#include "engine/components/physics/ColliderComponents.h"
#include "engine/graphics/ShaderProgram.h"
#include "engine/graphics/VertexArray.h"
#include "engine/graphics/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl/glew.h>

namespace MYENGINE
{


	GameObject::GameObject(const char* _name, weak(Manager) _manager, std::string _parentObjectName)
	{
		m_name = _name;
		m_decaying = false;

		m_noUpdates = true;

		m_manager = _manager;

		m_components = std::vector<shared(Component)>{};
		m_transform = weak(Transform)();

		if (!_parentObjectName.empty() && _parentObjectName != "NULL")
		{
			shared(GameObject) parent = GetManager()->GetGameObject(_parentObjectName);
			if (parent)
			{
				m_parentObject = parent;
			}
			else
			{
				WARN("GameObject trying to set its parent to an object which doesn't exist! (" << _parentObjectName << " does not exist)");
			}
		}
	}

	GameObject::~GameObject()
	{
		//std::cout << "Game object '" << m_name << "' destructing..." << std::endl;

		if (GetParentGameObject())
		{
			GetParentGameObject()->ForgetChildGameObject(m_self.lock());
		}

		m_components.clear();

		//std::cout << "Game object '" << m_name << "' destructed!" << std::endl << std::endl;
	}

	void GameObject::Initialise(shared(GameObject) _self)
	{
		m_self = _self;

		SetParentGameObject(m_parentObject.lock());
	}


	void GameObject::SetParentGameObject(shared(GameObject) _newParent)
	{
		for (weak(GameObject) c : m_childObjects)
		{
			shared(GameObject) child = c.lock();
			if (child && child->GetTransform())
			{
				for (shared(Transform) transform : child->GetComponents<Transform>())
				{
					transform->SetAltered();
				}
			}
		}


		if (GetParentGameObject() && _newParent != GetParentGameObject())
		{
			GetParentGameObject()->ForgetChildGameObject(m_self.lock());
		}
		m_parentObject = _newParent;

		if (_newParent)
		{
			GetParentGameObject()->AdoptChildGameObject(m_self.lock());
		}
	}

	void GameObject::AdoptChildGameObject(shared(GameObject) _newChild)
	{
		m_childObjects.push_back(_newChild);
	}

	void GameObject::ForgetChildGameObject(shared(GameObject) _oldChild)
	{
		for (size_t c = 0; c < m_childObjects.size(); c++)
		{
			if (m_childObjects[c].lock() == _oldChild)
			{
				m_childObjects[c].lock()->SetParentGameObject(nullptr);
				m_childObjects.erase(m_childObjects.begin() + c);
			}
		}
	}

	bool GameObject::AddTag(std::string _newTag)
	{
		if (!HasTag(_newTag))
		{
			m_tags.push_back(_newTag);
			return true;
		}
		return false;
	}

	bool GameObject::RemoveTag(std::string _tagToRemove)
	{
		for (size_t t = 0; t < m_tags.size(); t++)
		{
			if (m_tags[t] == _tagToRemove)
			{
				m_tags.erase(m_tags.begin() + t);
				return true;
			}
		}
		return false;
	}

	bool GameObject::HasTag(std::string _tag)
	{
		for (std::string t : m_tags)
		{
			if (t == _tag)
			{
				return true;
			}
		}
		return false;
	}

	void GameObject::AddComponent(shared(Component) _component)
	{
		m_components.push_back(_component);
		_component->Initialise(_component);
	}


	shared(PhysicsComponent) GameObject::GetPhysics()
	{
		for (int i = 0; i < GetComponentCount(); i++)
		{
			shared(Component) thisComponent = m_components.at(i);
			if (std::dynamic_pointer_cast<DynamicPhysics>(thisComponent) ||
				std::dynamic_pointer_cast<DummyPhysics>(thisComponent)
				)
			{
				return std::dynamic_pointer_cast<PhysicsComponent>(m_components.at(i));
			}
		}
		return nullptr;
	}

	shared(ColliderComponent) GameObject::GetCollider()
	{
		for (int i = 0; i < GetComponentCount(); i++)
		{
			shared(Component) thisComponent = m_components.at(i);
			if (std::dynamic_pointer_cast<PlaneCollider>(thisComponent) ||
				std::dynamic_pointer_cast<SphereCollider>(thisComponent) ||
				std::dynamic_pointer_cast<BoxCollider>(thisComponent)
				)
			{
				return std::dynamic_pointer_cast<ColliderComponent>(m_components.at(i));
			}
		}
		return nullptr;
	}

	shared(Camera) GameObject::GetCamera()
	{
		for (int i = 0; i < GetComponentCount(); i++)
		{
			shared(Component) thisComponent = m_components.at(i);
			if (std::dynamic_pointer_cast<PerspCamera>(thisComponent) ||
				std::dynamic_pointer_cast<OrthoCamera>(thisComponent)
				)
			{
				return std::dynamic_pointer_cast<Camera>(m_components.at(i));
			}
		}
		return nullptr;
	}

	void GameObject::SetTransform(weak(Transform) _transform)
	{
		m_transform = _transform;
	}

	void GameObject::UpdateEarly()
	{
		if (m_noUpdates)
		{
			for (int i = 0; i < GetComponentCount(); i++)
			{
				m_components.at(i)->ObjectFirstUpdate();
			}
			m_noUpdates = false;
		}

		for (int i = 0; i < GetComponentCount(); i++)
		{
			m_components.at(i)->UpdateEarly();
		}
	}

	void GameObject::Update()
	{
		for (int i = 0; i < GetComponentCount(); i++)
		{
			m_components.at(i)->Update();
		}
	}

	void GameObject::UpdateGUI()
	{
		for (int i = 0; i < GetComponentCount(); i++)
		{
			m_components.at(i)->UpdateGUI();
		}
	}

	void GameObject::Draw(weak(Camera) _camera, GLuint _shaderProgramGLID)
	{
		shared(Manager) manager = m_manager.lock();
		shared(Visualiser) visualiser = GetComponent<Visualiser>();
		shared(Transform) transform = GetTransform();
		if (manager && transform && visualiser && visualiser->IsVisible())
		{
			glUseProgram(_shaderProgramGLID);
			glUniform1i(glGetUniformLocation(_shaderProgramGLID, "u_Texture"), 0);
			glUniform1i(glGetUniformLocation(_shaderProgramGLID, "u_MaterialMap"), 1); // unused :(

			glBindVertexArray(visualiser->GetVertexArray()->GetId());

			// activate texture0 and bind to the object's main texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, visualiser->GetTexture()->GetId());

			// activate texture1 and bind to the object's material texture
			//glActiveTexture(GL_TEXTURE0 + 1);
			//glBindTexture(GL_TEXTURE_2D, visualiser->GetTexture()->GetId());

			shared(TextBox) attachedText = GetComponent<TextBox>();
			if (attachedText)
			{
				glm::vec3 colour = attachedText->GetColour();
				manager->GetCurrentUserSettings().GetTextColour(&colour);
				glUniform3f(glGetUniformLocation(_shaderProgramGLID, "u_Colour"), colour.x, colour.y, colour.z);

				glm::vec4 backColour = attachedText->GetBackgroundColour();
				manager->GetCurrentUserSettings().GetTextBackground(&backColour);
				glUniform4f(glGetUniformLocation(_shaderProgramGLID, "u_BackColour"), backColour.x, backColour.y, backColour.z, backColour.w);
			}

			shared(LightSource) attachedLight = GetComponent<SunLight>();
			attachedLight = attachedLight ? attachedLight : GetComponent<OrbLight>();
			attachedLight = attachedLight ? attachedLight : GetComponent<SpotLight>();
			if (attachedLight)
			{
				glm::vec3 lightCol = attachedLight->GetColour();
				glUniform3f(glGetUniformLocation(_shaderProgramGLID, "u_Colour"), lightCol.x, lightCol.y, lightCol.z);
			}


			// Upload the light positions
			int l_types[MAX_LIGHT_COUNT];
			glm::vec3 l_positions[MAX_LIGHT_COUNT];
			glm::vec3 l_forwards[MAX_LIGHT_COUNT];
			glm::vec3 l_colours[MAX_LIGHT_COUNT];
			glm::vec3 l_distances[MAX_LIGHT_COUNT];
			glm::mat4 l_matrices[MAX_LIGHT_COUNT];
			int l_mapIDs[MAX_LIGHT_COUNT];

			for (int l = 0; l < manager->GetLightCount(); l++)
			{
				shared(LightSource) currentLight = manager->GetLight(l);
				shared(GameObject) currentLightObj = currentLight->GetObject();

				l_types[l] = currentLight->GetLightType();
				l_positions[l] = currentLightObj->GetTransform()->GetAbsolutePosition();
				l_forwards[l] = currentLight->GetForwards();
				l_colours[l] = currentLight->GetColour();
				l_distances[l] = currentLight->GetDistanceValues();
				l_matrices[l] = currentLight->GetMatrix();
				l_mapIDs[l] = 0;
				if (currentLight->CastsShadows())
				{
					// texture2 to texture2 + MAX_LIGHT_COUNT will be used for shadow maps
					l_mapIDs[l] = 2 + l;
					glActiveTexture(GL_TEXTURE0 + 2 + l);
					glBindTexture(GL_TEXTURE_2D, manager->GetResourceManager()->GetTexture(currentLightObj->GetName().c_str())->GetId());
				}
			}

			glUniform1i(glGetUniformLocation(_shaderProgramGLID, "u_LightCount"), manager->GetLightCount());
			glUniform1iv(glGetUniformLocation(_shaderProgramGLID, "u_LightTypes"), manager->GetLightCount(), &l_types[0]);
			glUniform3fv(glGetUniformLocation(_shaderProgramGLID, "u_LightPositions"), manager->GetLightCount(), glm::value_ptr(l_positions[0]));
			glUniform3fv(glGetUniformLocation(_shaderProgramGLID, "u_LightForwards"), manager->GetLightCount(), glm::value_ptr(l_forwards[0]));
			glUniform3fv(glGetUniformLocation(_shaderProgramGLID, "u_LightColours"), manager->GetLightCount(), glm::value_ptr(l_colours[0]));
			glUniform3fv(glGetUniformLocation(_shaderProgramGLID, "u_LightDistances"), manager->GetLightCount(), glm::value_ptr(l_distances[0]));
			glUniformMatrix4fv(glGetUniformLocation(_shaderProgramGLID, "u_LightTransformMatrices"), manager->GetLightCount(), GL_FALSE, glm::value_ptr(l_matrices[0]));
			glUniform1iv(glGetUniformLocation(_shaderProgramGLID, "u_ShadowMaps"), manager->GetLightCount(), &l_mapIDs[0]);
			

			// Upload the model matrix
			glm::mat4 model = transform->GetModelMatrix();
			glUniformMatrix4fv(glGetUniformLocation(_shaderProgramGLID, "u_Model"), 1, GL_FALSE, glm::value_ptr(model));

			// Upload the view matrix
			glm::mat4 view(1.0f);
			if (transform->UsingPerspective()) { view = _camera.lock()->GetViewMatrix(); }
			glUniformMatrix4fv(glGetUniformLocation(_shaderProgramGLID, "u_View"), 1, GL_FALSE, glm::value_ptr(glm::inverse(view))); // TODO: really shouldn't be using an inverse function for each object each frame...

			// Upload the projection matrix
			glm::mat4 projection = _camera.lock()->GetProjectionMatrix();
			if (!transform->UsingPerspective())
			{
				projection = manager->GetOrthoMatrix();
			}
			glUniformMatrix4fv(glGetUniformLocation(_shaderProgramGLID, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projection));


			// Draw vertices
			glDrawArrays(GL_TRIANGLES, 0, visualiser->GetVertexArray()->GetVertCount());
		}
		else if (manager && visualiser && visualiser->IsVisible() && _shaderProgramGLID != 3)
		{
			glDepthFunc(GL_LEQUAL);

			glUseProgram(visualiser->GetShaderProgram()->GetId());
			glUniform1i(glGetUniformLocation(visualiser->GetShaderProgram()->GetId(), "u_Skybox"), 0);

			glm::vec3 pos = _camera.lock()->GetObject()->GetTransform()->GetAbsolutePosition();
			glUniform3f(glGetUniformLocation(visualiser->GetShaderProgram()->GetId(), "in_Position"), pos.x, pos.y, pos.z);

			glBindVertexArray(visualiser->GetVertexArray()->GetId());

			// activate texture0 and bind to the object's main texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, visualiser->GetTexture()->GetId());



			// Upload the view matrix
			glm::mat4 view = _camera.lock()->GetViewMatrix();
			glUniformMatrix4fv(glGetUniformLocation(_shaderProgramGLID, "u_View"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(glm::inverse(view))))); // TODO: really shouldn't be using an inverse function for each object each frame...

			// Upload the projection matrix
			glm::mat4 projection = _camera.lock()->GetProjectionMatrix();
			glUniformMatrix4fv(glGetUniformLocation(_shaderProgramGLID, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projection));


			// Draw vertices
			glDrawArrays(GL_TRIANGLES, 0, visualiser->GetVertexArray()->GetVertCount());

			glDepthFunc(GL_LESS);
		}
	}

	void GameObject::Draw()
	{
		shared(Manager) manager = m_manager.lock();
		shared(Visualiser) visualiser = GetComponent<Visualiser>();
		if (manager && visualiser)
		{
			Draw(manager->GetMainCamera(), visualiser->GetShaderProgram()->GetId());
		}
	}

} // namespace MYENGINE
