#include "Component_Types.h"

#include "engine/managers/Manager.h"
#include "engine/managers/InputManager.h"
#include "engine/managers/ResourceManager.h"
#include "engine/GameObject.h"
#include "engine/components/physics/ColliderComponents.h"
#include "engine/components/physics/PhysicsComponents.h"
#include "engine/graphics/VertexBuffer.h"
#include "engine/graphics/ShaderProgram.h"
#include "engine/input/Concepts.h"

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MYENGINE
{

	Transform::Transform(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale)
		: Component(weak(GameObject)(), "TRANSFORM")
	{
		m_model = glm::mat4(1.0f);
		m_perspectiveDraw = false;

		m_position = _position;
		m_rotation = _rotation;
		m_scale = _scale;

		m_secondaryPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		m_secondaryRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	Transform::Transform(weak(GameObject) _parentObject, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, bool _usePerspective)
		: Component(_parentObject, "TRANSFORM")
	{
		m_model = glm::mat4(1.0f);
		m_perspectiveDraw = _usePerspective;

		m_position = _position;
		m_rotation = _rotation;
		m_scale = _scale;

		m_secondaryPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		m_secondaryRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	Transform::~Transform()
	{

	}

	void Transform::Initialise(weak(Component) _componentSelf)
	{
		Component::Initialise(_componentSelf);

		if (!GetObject()->GetTransform())
		{
			GetObject()->SetTransform(std::dynamic_pointer_cast<Transform>(m_self.lock()));
		}
	}

	void Transform::Translate(glm::vec3 _translation)
	{
		SetAltered();
		m_position += _translation;
	}

	void Transform::Rotate(glm::vec3 _rotation)
	{
		SetAltered();
		glm::vec3 newRotation = m_rotation + _rotation;

		if (newRotation.x >= 360) { newRotation.x -= 360; }
		if (newRotation.y >= 360) { newRotation.y -= 360; }
		if (newRotation.z >= 360) { newRotation.z -= 360; }

		m_rotation = newRotation;
	}

	void Transform::Rotate(float _angle, glm::vec3 _axis)
	{
		glm::vec3 fullRotation = _axis;
		fullRotation.x *= _angle;
		fullRotation.y *= _angle;
		fullRotation.z *= _angle;

		Rotate(fullRotation);
	}

	void Transform::Scale(glm::vec3 _scale)
	{
		SetAltered();
		m_scale.x *= _scale.x;
		m_scale.y *= _scale.y;
		m_scale.z *= _scale.z;
	}

	void Transform::Scale(float _scale)
	{
		SetAltered();
		m_scale *= _scale;
	}

	void Transform::SecondaryTranslate(glm::vec3 _translation)
	{
		SetAltered();
		m_secondaryPosition += _translation;
	}

	void Transform::SecondaryRotate(glm::vec3 _rotation)
	{
		SetAltered();
		glm::vec3 newRotation = m_secondaryRotation + _rotation;

		if (newRotation.x >= 360) { newRotation.x -= 360; }
		if (newRotation.y >= 360) { newRotation.y -= 360; }
		if (newRotation.z >= 360) { newRotation.z -= 360; }

		m_secondaryRotation = newRotation;
	}

	glm::vec3 Transform::GetAbsolutePosition()
	{
		return glm::vec3(GetModelMatrix()[3]);
	}

	glm::vec3 Transform::GetAbsoluteRotation()
	{
		glm::vec3 result = GetLocalRotation();

		if (GetObject() && GetObject()->GetParentGameObject() && GetObject()->GetParentGameObject()->GetTransform())
		{
			result += GetObject()->GetParentGameObject()->GetTransform()->GetAbsoluteRotation();
		}

		return result;

		//const float radians = 0.01745329251994329576923690768489f;
		//glm::vec3 result = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::mat4 model = GetModelMatrix();
		//

		//// --------------------------------------------
		//result.y = asin(model[2][0]);
		//float C = cos(result.y);
		//result.y *= radians;

		//float trX = 0.0f;
		//float trY = 0.0f;
		//if (abs(C) > 0.005f)
		//{
		//	trX = model[2][2] / C;
		//	trY = -model[2][1] / C;
		//	result.x = atan2(trY, trX) * radians;

		//	trX = model[0][0] / C;
		//	trY = model[1][0] / C;
		//	result.z = atan2(trY, trX) * radians;
		//}
		//else
		//{
		//	result.x = 0.0f;

		//	trX = model[1][1];
		//	trY = model[0][1];

		//	result.z = atan2(trY, trX) * radians;
		//}

		//if (result.x < 0) { result.x += 360; }
		//if (result.y < 0) { result.y += 360; }
		//if (result.z < 0) { result.z += 360; }
		//// --------------------------------------------


		//return result;
	}

	glm::vec3 Transform::GetAbsoluteScale()
	{
		glm::vec3 result = GetLocalScale();

		if (GetObject() && GetObject()->GetParentGameObject() && GetObject()->GetParentGameObject()->GetTransform())
		{
			result *= GetObject()->GetParentGameObject()->GetTransform()->GetAbsoluteScale();
		}

		return result;
	}

	glm::mat4 Transform::GetModelMatrix()
	{
		shared(GameObject) obj = GetObject();
		shared(ColliderComponent) col = nullptr;
		if (obj)
		{
			col = GetObject()->GetCollider();
		}
		
		if (col && col->Rotatable() && col->GetPhysicsComponent())
		{
			shared(DynamicPhysics) dyn = std::dynamic_pointer_cast<DynamicPhysics>(col->GetPhysicsComponent());
			if (dyn && dyn->GetNumIntMethod() == 0)
			{

			}

			glm::mat4 modelRotation = glm::mat4(dyn->GetRotationMatrix());

			m_model = glm::translate(glm::mat4(1.0f), GetLocalPosition());
			m_model = m_model * modelRotation;
			m_model = glm::scale(m_model, GetLocalScale());
		}
		else if (m_altered)
		{
			m_model = glm::mat4(1.0f);
			m_model = glm::translate(m_model, GetLocalPosition());
			m_model = glm::rotate(m_model, glm::radians(GetLocalRotation().z), glm::vec3(0, 0, 1));
			m_model = glm::rotate(m_model, glm::radians(GetLocalRotation().y), glm::vec3(0, 1, 0));
			m_model = glm::rotate(m_model, glm::radians(GetLocalRotation().x), glm::vec3(1, 0, 0));

			m_model = glm::scale(m_model, GetLocalScale());

			m_model = glm::translate(m_model, m_secondaryPosition);
			m_model = glm::rotate(m_model, glm::radians(m_secondaryRotation.z), glm::vec3(0, 0, 1));
			m_model = glm::rotate(m_model, glm::radians(m_secondaryRotation.y), glm::vec3(0, 1, 0));
			m_model = glm::rotate(m_model, glm::radians(m_secondaryRotation.x), glm::vec3(1, 0, 0));

			m_secondaryPosition = glm::vec3(0.0f);
			m_position = m_model[3];

			if (obj && obj->GetParentGameObject() && obj->GetParentGameObject()->GetTransform())
			{
				m_model = obj->GetParentGameObject()->GetTransform()->GetModelMatrix() * m_model;
			}

			m_altered = false;
		}

		return m_model;
	}

	void Transform::SetAltered()
	{
		m_altered = true;

		shared(GameObject) obj = GetObject();
		if (obj)
		{
			for (weak(GameObject) c : obj->GetChildGameObjects())
			{
				shared(GameObject) child = c.lock();
				if (child && child->GetTransform())
				{
					child->GetTransform()->SetAltered();
				}
			}
		}
	}





	Visualiser::Visualiser(weak(GameObject) _parentObject)
		: Component(_parentObject, "VISUALISER")
	{
		GLuint shaderProgram_GLID = GetShaderProgram()->GetId();
		m_u_Model_Location = glGetUniformLocation(shaderProgram_GLID, "u_Model");
		m_u_View_Location = glGetUniformLocation(shaderProgram_GLID, "u_View");
		m_u_Projection_Location = glGetUniformLocation(shaderProgram_GLID, "u_Projection");

		m_visible = true;
	}

	Visualiser::Visualiser(weak(GameObject) _parentObject, const char* _modelFilePath, const char* _textureFilePath, const char* _shaderProgramName)
		: Component(_parentObject, "VISUALISER")
	{
		m_vertexArray = GetManager()->GetResourceManager()->GetVertexArray(_modelFilePath);
		m_shaderProgram = GetManager()->GetResourceManager()->GetShaderProgram(_shaderProgramName);
		m_texture = GetManager()->GetResourceManager()->GetTexture(_textureFilePath);

		GLuint shaderProgram_GLID = GetShaderProgram()->GetId();
		m_u_Model_Location = glGetUniformLocation(shaderProgram_GLID, "u_Model");
		m_u_View_Location = glGetUniformLocation(shaderProgram_GLID, "u_View");
		m_u_Projection_Location = glGetUniformLocation(shaderProgram_GLID, "u_Projection");

		m_visible = true;
	}

	void Visualiser::SetVisible(bool _newValue, bool _applyToChildren)
	{
		m_visible = _newValue;

		if (_applyToChildren)
		{
			for (weak(GameObject) c : GetObject()->GetChildGameObjects())
			{
				shared(GameObject) child = c.lock();
				if (child && child->GetComponent<Visualiser>())
				{
					child->GetComponent<Visualiser>()->SetVisible(_newValue);
				}
			}
		}
	}

	void Visualiser::SetVertexArray(const char* _filePath)
	{
		m_vertexArray = GetManager()->GetResourceManager()->GetVertexArray(_filePath);
	}


	Camera::Camera(weak(GameObject) _parentObject, int _viewX, int _viewY, float _viewDistance)
		: Component(_parentObject, "CAMERA")
	{
		m_projectionMatrix = glm::mat4(1.0f);

		m_viewSize = glm::vec2(_viewX, _viewY);
		m_viewDistance = _viewDistance;
	}

	void Camera::Initialise(weak(Component) _componentSelf)
	{
		Component::Initialise(_componentSelf);
		GetManager()->RegisterComponent(std::dynamic_pointer_cast<Camera>(m_self.lock()));
	}

	void Camera::UpdateEarly()
	{
		m_inverseViewMatrix = glm::inverse(GetViewMatrix());
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return GetObject()->GetTransform()->GetModelMatrix();
	}

	glm::mat4 Camera::GetInverseViewMatrix()
	{
		return m_inverseViewMatrix;
	}

	glm::vec2 Camera::GetViewSize()
	{
		glm::vec2 windowSize = GetManager()->GetWindowDimensions();
		glm::vec2 returnValue = m_viewSize;
		if (m_viewSize.x < 0)
		{
			returnValue.x = windowSize.x * m_viewSize.x * -1;
		}
		if (m_viewSize.y < 0)
		{
			returnValue.y = windowSize.y * m_viewSize.y * -1;
		}

		return returnValue;
	}


	PerspCamera::PerspCamera(weak(GameObject) _parentObject, int _viewX, int _viewY, float _viewDistance, float _fov)
		: Camera(_parentObject, _viewX, _viewY, _viewDistance)
	{
		m_fov = _fov; // normal is 45.0
	}

	void PerspCamera::UpdateProjectionMatrix(int _windowWidth, int _windowHeight)
	{
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), (float)GetViewSize().x / (float)GetViewSize().y, 0.1f, m_viewDistance);
	}


	OrthoCamera::OrthoCamera(weak(GameObject) _parentObject, int _viewX, int _viewY, float _viewDistance, float _zoom)
		: Camera(_parentObject, _viewX, _viewY, _viewDistance)
	{
		m_zoom = _zoom; // normal is 35.0
	}

	void OrthoCamera::UpdateProjectionMatrix(int _windowWidth, int _windowHeight)
	{
		m_projectionMatrix = glm::ortho(0.0f, (float)GetViewSize().x, 0.0f, (float)GetViewSize().y, 0.1f, m_viewDistance);
		m_projectionMatrix = glm::translate(m_projectionMatrix, glm::vec3(GetViewSize().x * 0.5f, GetViewSize().y * 0.5f, 0));
		m_projectionMatrix = glm::scale(m_projectionMatrix, glm::vec3(m_zoom, m_zoom, 1));
	}




	LightSource::LightSource(weak(GameObject) _parentObject, int _lightType, const char* _name, glm::vec3 _colour)
		: Component(_parentObject, _name)
	{
		m_lightType = _lightType;
		m_colour = _colour;
		m_distanceValues = glm::vec3(1.0f, 0.022f, 0.0019f);
		m_castShadows = GetObject()->GetCamera() != nullptr;
	}

	void LightSource::Initialise(weak(Component) _componentSelf)
	{
		Component::Initialise(_componentSelf);
		GetManager()->RegisterComponent(std::dynamic_pointer_cast<LightSource>(m_self.lock()));
	}

	glm::vec3 LightSource::GetForwards()
	{
		if (m_castShadows)
		{
			return glm::normalize(glm::vec3(GetObject()->GetCamera()->GetInverseViewMatrix()[2]) * glm::vec3(1, 1, -1));
		}
		else
		{
			return glm::vec3(1.0f);
		}
	}

	glm::mat4 LightSource::GetMatrix()
	{
		if (m_castShadows)
		{
			shared(Camera) cam = GetObject()->GetCamera();
			return cam->GetProjectionMatrix() * cam->GetInverseViewMatrix();
		}
		else
		{
			return glm::mat4(1.0f);
		}
	}

	SunLight::SunLight(weak(GameObject) _parentObject, glm::vec3 _colour)
		: LightSource(_parentObject, 0, "SUN_LIGHT", _colour)
	{

	}

	OrbLight::OrbLight(weak(GameObject) _parentObject, glm::vec3 _colour)
		: LightSource(_parentObject, 1, "ORB_LIGHT", _colour)
	{

	}

	SpotLight::SpotLight(weak(GameObject) _parentObject, glm::vec3 _colour)
		: LightSource(_parentObject, 2, "SPOT_LIGHT", _colour)
	{

	}


	FreeCamera::FreeCamera(weak(GameObject) _parentObject, float _movementSpeed, float _rotationSpeed)
		: Component(_parentObject, "FREE_CAMERA")
	{
		m_movementSpeed = _movementSpeed;
		m_rotationSpeed = _rotationSpeed;
	}

	void FreeCamera::UpdateEarly()
	{
		float frameMovementSpeed = m_movementSpeed;
		float frameRotationSpeed = m_rotationSpeed;

		if (GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_MODIFIER1").value != 0)
		{
			frameMovementSpeed *= 0.1f;
			frameRotationSpeed *= 0.1f;
		}

		if (GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_MODIFIER2").value != 0)
		{
			frameMovementSpeed *= 3.0f;
			frameRotationSpeed *= 3.0f;
		}

		ConceptState move_right = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_RIGHT");
		ConceptState move_left = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_LEFT");
		if (move_right.value != 0)
		{
			GetObject()->GetTransform()->SecondaryTranslate(glm::vec3(1.0f, 0.0f, 0.0f) * frameMovementSpeed * GetManager()->DeltaTime() * move_right.normalValue);
		}
		else if (move_left.value != 0)
		{
			GetObject()->GetTransform()->SecondaryTranslate(glm::vec3(-1.0f, 0.0f, 0.0f) * frameMovementSpeed * GetManager()->DeltaTime() * move_left.normalValue);
		}

		ConceptState move_back = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_BACK");
		ConceptState move_forward = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_FORWARD");
		if (move_back.value != 0)
		{
			GetObject()->GetTransform()->SecondaryTranslate(glm::vec3(0.0f, 0.0f, 1.0f) * frameMovementSpeed * GetManager()->DeltaTime() * move_back.normalValue);
		}
		else if (move_forward.value != 0)
		{
			GetObject()->GetTransform()->SecondaryTranslate(glm::vec3(0.0f, 0.0f, -1.0f) * frameMovementSpeed * GetManager()->DeltaTime() * move_forward.normalValue);
		}


		if (GetManager()->GetInputManager()->MouseLocked())
		{
			GetObject()->GetTransform()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f) * -GetManager()->GetInputManager()->GetMouseDelta().x * frameRotationSpeed);
			GetObject()->GetTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f) * GetManager()->GetInputManager()->GetMouseDelta().y * frameRotationSpeed);
		}

		ConceptState look_up = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_LOOK_UP");
		ConceptState look_down = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_LOOK_DOWN");
		if (look_up.value != 0)
		{
			GetObject()->GetTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f) * frameRotationSpeed * 100.0f * GetManager()->DeltaTime() * look_up.normalValue);
			//GetObject()->GetTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f) * frameRotationSpeed * GetManager()->DeltaTime());
		}
		else if (look_down.value != 0)
		{
			GetObject()->GetTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f) * -frameRotationSpeed * 100.0f * GetManager()->DeltaTime() * look_down.normalValue);
			//GetObject()->GetTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f) * frameRotationSpeed * GetManager()->DeltaTime());
		}

		ConceptState look_left = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_LOOK_LEFT");
		ConceptState look_right = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_MOVEMENT_LOOK_RIGHT");
		if (look_left.value != 0)
		{
			GetObject()->GetTransform()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f) * frameRotationSpeed * 100.0f * GetManager()->DeltaTime() * look_left.normalValue);
			//GetObject()->GetTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f) * frameRotationSpeed * GetManager()->DeltaTime());
		}
		else if (look_right.value != 0)
		{
			GetObject()->GetTransform()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f) * -frameRotationSpeed * 100.0f * GetManager()->DeltaTime() * look_right.normalValue);
			//GetObject()->GetTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f) * frameRotationSpeed * GetManager()->DeltaTime());
		}

		ConceptState lockMouseConcept = GetManager()->GetInputManager()->GetInputConceptState("DEBUG_LOCK_MOUSE");
		if (lockMouseConcept.changedToNonZero) { GetManager()->GetInputManager()->LockMouse(true); }
		else if (lockMouseConcept.changed && lockMouseConcept.value == 0) { GetManager()->GetInputManager()->LockMouse(false); }
	}


	//Button::Button(weak(GameObject) _parentObject, glm::vec2 _halfExtents)
	//	: Component(_parentObject, "BUTTON")
	//{
	//	shared(Transform) t = GetObject()->GetTransform();
	//	glm::vec3 scale = t->GetAbsoluteScale();
	//	m_halfExtents = glm::vec2(scale.x, scale.z);
	//	m_mouseInsideExtents = false;

	//	glm::vec3 pos = t->GetAbsolutePosition();
	//	std::cout << "Button position: " << pos.x << "," << pos.y;
	//}

	//void Button::UpdateEarly()
	//{
	//	shared(Transform) t = GetObject()->GetTransform();
	//	shared(InputManager) inputManager = GetManager()->GetInputManager();

	//	glm::vec3 scale = t->GetAbsoluteScale();
	//	m_halfExtents = glm::vec2(scale.x / 2, scale.z / 2);

	//	glm::vec2 mousePos = inputManager->GetMousePos();
	//	glm::vec3 myPos = t->GetAbsolutePosition();

	//	bool insideX = mousePos.x > myPos.x - m_halfExtents.x && mousePos.x < myPos.x + m_halfExtents.x;
	//	bool insideY = mousePos.y > myPos.y - m_halfExtents.y && mousePos.y < myPos.y + m_halfExtents.y;

	//	if (insideX && insideY)
	//	{
	//		if (!m_mouseInsideExtents)
	//		{
	//			MouseEnter();
	//			m_mouseInsideExtents = true;
	//		}
	//		else
	//		{
	//			MouseOver();
	//		}

	//		if (inputManager->GetInputConceptValue(SDL_ControllerButtons::BUTTON_LEFT))
	//		{
	//			ClickDown();
	//		}
	//		else if (inputManager->IsKeyHeld(SDL_ControllerButtons::BUTTON_LEFT))
	//		{
	//			ClickHeld();
	//		}
	//		else if (inputManager->IsKeyUp(SDL_ControllerButtons::BUTTON_LEFT))
	//		{
	//			ClickRelease();
	//		}
	//	}
	//	else if (m_mouseInsideExtents)
	//	{
	//		MouseLeft();
	//		m_mouseInsideExtents = false;
	//	}
	//}

	//void Button::MouseEnter()
	//{
	//	std::cout << "Mouse enter!" << std::endl;
	//}

	//void Button::MouseOver()
	//{
	//	//std::cout << "Mouse inside!" << std::endl;
	//}

	//void Button::MouseLeft()
	//{
	//	std::cout << "Mouse left!" << std::endl;
	//}


	//void Button::ClickDown()
	//{
	//	std::cout << "Click!" << std::endl;
	//	GetManager()->GetPhysicsManager()->Activate();
	//}

	//void Button::ClickHeld()
	//{
	//	//std::cout << "Click held!" << std::endl;
	//}

	//void Button::ClickRelease()
	//{
	//	std::cout << "Unclick!" << std::endl;
	//}

} // namespace MYENGINE
