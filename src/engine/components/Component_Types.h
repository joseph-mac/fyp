#ifndef _MYENGINE_COMPONENTTYPES_H_
#define _MYENGINE_COMPONENTTYPES_H_

#include "engine/Component.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <vector>

typedef int GLint;

namespace MYENGINE
{

	struct GameObject;
	struct VertexArray;
	struct ShaderProgram;
	struct Texture;

	struct LightSource : public Component
	{
	public:
		/*
		Light types:
			0: Sun
			1: Orb
			2: Spot
		*/
		LightSource(weak(GameObject) _parentObject, int _lightType, const char* _name, glm::vec3 _colour);

		/**
		* Get the type of light this LightSource represents: 0=Sun, 1=Orb, 2=Spot.
		* 
		* @returns the ID of the type of light.
		**/
		int GetLightType() { return m_lightType; }

		/**
		* @returns the colour of this LightSource.
		**/
		glm::vec3 GetColour() { return m_colour; }

		/**
		* @returns this LightSource's distance values in the form of a vec3.
		**/
		glm::vec3 GetDistanceValues() { return m_distanceValues; }

		/**
		* @returns whether this LightSource casts shadows.
		**/
		bool CastsShadows() { return m_castShadows; }

		/**
		* @returns the forward direction vector of this LightSource.
		**/
		glm::vec3 GetForwards();

		/**
		* Retrieve a matrix to transform a world-space point into the LightSource's Camera's space.
		* 
		* @returns the matrix.
		**/
		glm::mat4 GetMatrix();

	protected:
		void Initialise(weak(Component) _componentSelf) override;

		int m_lightType;
		glm::vec3 m_colour;
		glm::vec3 m_distanceValues;
		bool m_castShadows;
	};

	struct SunLight : public LightSource
	{
		SunLight(weak(GameObject) _parentObject, glm::vec3 _colour);
	};

	struct OrbLight : public LightSource
	{
		OrbLight(weak(GameObject) _parentObject, glm::vec3 _colour);
	};

	struct SpotLight : public LightSource
	{
		SpotLight(weak(GameObject) _parentObject, glm::vec3 _colour);
	};


	struct Transform : public Component
	{
	public:
		Transform(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale);
		Transform(weak(GameObject) _parentObject, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, bool _usePerspective);
		~Transform();

		/**
		* Set the position of this Transform.
		*
		* @param _newPosition : the new position.
		**/
		virtual void SetPosition(glm::vec3 _newPosition) { SetAltered(); m_position = _newPosition; }

		/**
		* Set the rotation of this Transform.
		*
		* @param _newRotation : the new rotation.
		**/
		void SetRotation(glm::vec3 _newRotation) { SetAltered(); m_rotation = _newRotation; }

		/**
		* Set the scale of this Transform.
		*
		* @param _newScale : the new scale.
		**/
		virtual void SetScale(glm::vec3 _newScale) { SetAltered(); m_scale = _newScale; }

		/**
		* Translate the Transform by the given amount.
		*
		* @param _translation : the translation value.
		**/
		virtual void Translate(glm::vec3 _translation);

		/**
		* Rotate the Transform by the given amounts on each axis in the vec3.
		*
		* @param _rotation : the rotations.
		**/
		void Rotate(glm::vec3 _rotation);

		/**
		* Rotate the Transform by the given amount on the given axis.
		*
		* @param _angle : the angle to rotate by.
		* @param _axis : the axis to rotate on.
		**/
		void Rotate(float _angle, glm::vec3 _axis);

		/**
		* Scale the Transform by the given amounts on each axis in the vec3.
		*
		* @param _scale : the scale values.
		**/
		virtual void Scale(glm::vec3 _scale);

		/**
		* Scale the Transform uniformly by the given amount.
		*
		* @param _scale : the scale value.
		**/
		virtual void Scale(float _scale);

		/**
		* Translate the Transform by the given amount, after primary transformations and the local rotation have been executed.
		*
		* @param _translation : the translation value.
		**/
		void SecondaryTranslate(glm::vec3 _translation);

		/**
		* Rotate the Transform by the given amounts on each axis in the vec3, after primary transformations have been executed.
		*
		* @param _rotation : the rotations.
		**/
		void SecondaryRotate(glm::vec3 _rotation);

		/**
		* @returns the absolute position of this Transform.
		**/
		virtual glm::vec3 GetAbsolutePosition();

		/**
		* @returns the absolute rotation of this Transform.
		**/
		virtual glm::vec3 GetAbsoluteRotation();

		/**
		* @returns the absolute scale of this Transform.
		**/
		virtual glm::vec3 GetAbsoluteScale();

		/**
		* @returns the local position of this Transform.
		**/
		virtual glm::vec3 GetLocalPosition() { return m_position; }

		/**
		* @returns the local rotation of this Transform.
		**/
		glm::vec3 GetLocalRotation() { return m_rotation; }

		/**
		* @returns the local scale of this Transform.
		**/
		glm::vec3 GetLocalScale() { return m_scale; }

		/**
		* @returns the Transform's model matrix, updating it if needed.
		**/
		virtual glm::mat4 GetModelMatrix();

		/**
		* @returns whether the Transform is made for use with a perspective projection matrix, as opposed to orthographic.
		**/
		bool UsingPerspective() { return m_perspectiveDraw; }

		/**
		* @returns whether the Transform's properties have changed since the model matrix was last updated.
		**/
		bool HasChanged() { return m_altered; }

	protected:
		friend GameObject;

		void Initialise(weak(Component) _componentSelf) override;

		void SetAltered();

		glm::mat4 m_model;
		bool m_altered = true;
		bool m_perspectiveDraw;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

		glm::vec3 m_secondaryPosition;
		glm::vec3 m_secondaryRotation;
	};

	struct Camera : public Component
	{
	public:
		Camera(weak(GameObject) _parentObject, int _viewX, int _viewY, float _viewDistance);

		glm::mat4 GetViewMatrix();
		glm::mat4 GetInverseViewMatrix();
		glm::vec2 GetViewSize();

		/**
		* @returns the projection matrix used by this Camera.
		**/
		virtual glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }

	protected:
		friend Manager;

		void Initialise(weak(Component) _componentSelf) override;
		void UpdateEarly() override;
		virtual void UpdateProjectionMatrix(int _windowWidth, int _windowHeight) {}

		glm::mat4 m_inverseViewMatrix;
		glm::vec2 m_viewSize;
		float m_viewDistance;
		glm::mat4 m_projectionMatrix;
	};

	struct PerspCamera : public Camera
	{
	public:
		PerspCamera(weak(GameObject) _parentObject, int _viewX, int _viewY, float _viewDistance, float _fov);

	private:
		void UpdateProjectionMatrix(int _windowWidth, int _windowHeight) override;

		float m_fov;
	};

	struct OrthoCamera : public Camera
	{
	public:
		OrthoCamera(weak(GameObject) _parentObject, int _viewX, int _viewY, float _viewDistance, float _zoom);

	private:
		void UpdateProjectionMatrix(int _windowWidth, int _windowHeight) override;

		float m_zoom;
	};


	struct Visualiser : public Component
	{
	public:
		Visualiser(weak(GameObject) _parentObject);
		Visualiser(weak(GameObject) _parentObject, const char* _modelFilePath, const char* _textureFilePath, const char* _shaderProgramName);
		~Visualiser() {};

		/**
		* Set the VertexArray resource to be used by this Visualiser
		* 
		* @param _filePath : The file path of the new resource.
		**/
		void SetVertexArray(const char* _filePath);

		/**
		* Set whether this Visualiser should allow its GameObject to be drawn.
		*
		* @param _newValue : the desired visibility state.
		**/
		void SetVisible(bool _newValue, bool _applyToChildren = true);

		/**
		* @returns the visibility state of this Visualiser.
		**/
		bool IsVisible() { return m_visible; }

		/**
		* @returns the VertexArray resource used by this Visualiser.
		**/
		shared(VertexArray) GetVertexArray() { return m_vertexArray.lock(); }

		/**
		* @returns the ShaderProgram resource used by this Visualiser.
		**/
		shared(ShaderProgram) GetShaderProgram() { return m_shaderProgram.lock(); }

		/**
		* @returns the Texture resource used by this Visualiser.
		**/
		shared(Texture) GetTexture() { return m_texture.lock(); }

		/**
		* @returns the OpenGL uniform location for the model matrix.
		**/
		GLint Get_U_Model_Location() { return m_u_Model_Location; }

		/**
		* @returns the OpenGL uniform location for the view matrix.
		**/
		GLint Get_U_View_Location() { return m_u_View_Location; }

		/**
		* @returns the OpenGL uniform location for the projection matrix.
		**/
		GLint Get_U_Projection_Location() { return m_u_Projection_Location; }

	private:

		weak(VertexArray) m_vertexArray;
		weak(ShaderProgram) m_shaderProgram;
		weak(Texture) m_texture;

		GLint m_u_Model_Location;
		GLint m_u_View_Location;
		GLint m_u_Projection_Location;

		bool m_visible;
	};

	struct FreeCamera : public Component
	{
	public:
		FreeCamera(weak(GameObject) _parentObject, float _movementSpeed, float _rotationSpeed);

		void UpdateEarly() override;

	private:
		float m_movementSpeed;
		float m_rotationSpeed;

		glm::vec2 m_movementValues[4] = { glm::vec2(1, 0), glm::vec2(-1, 0), glm::vec2(0, -1), glm::vec2(0, 1) };
	};

} // namespace MYENGINE

#endif
