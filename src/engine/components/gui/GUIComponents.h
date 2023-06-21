#ifndef _MYENGINE_GUICOMPONENTS_H_
#define _MYENGINE_GUICOMPONENTS_H_

#include "engine/Component.h"
#include "engine/components/Component_Types.h"
#include "engine/gui/GUIDefs.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace MYENGINE
{

	struct TransformGUI : public Transform
	{
	public:
		TransformGUI(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale);
		TransformGUI(weak(GameObject) _parentObject, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, GUIAnchor _posAnchor, GUIAnchor _screenAnchor);
		~TransformGUI() {};

		/**
		* @returns the absolute position of this TransformGUI.
		**/
		glm::vec3 GetAbsolutePosition() override;

		/**
		* @returns the absolute rotation of this TransformGUI.
		**/
		glm::vec3 GetAbsoluteRotation() override;

		/**
		* @returns the absolute scale of this TransformGUI.
		**/
		glm::vec3 GetAbsoluteScale() override;

		/**
		* @returns the TransformGUI's model matrix, updating it if needed.
		**/
		glm::mat4 GetModelMatrix() override;

	protected:
		virtual void UpdateGUI() override;

		// the point on this transform's parent to which this transform is anchored
		GUIAnchor m_parentAnchor;

		// the acting origin point of this transform
		GUIAnchor m_originAnchor;
	};

	struct TextBox : public Component
	{
	public:
		TextBox(weak(GameObject) _parentObject);
		TextBox(weak(GameObject) _parentObject, const char* _text, float _size, GUIAnchor _textAnchor, glm::vec3 _colour, glm::vec4 _backgroundColour);

		/**
		* Set this TextBox's text.
		* 
		* @param _newText : The new text.
		**/
		void SetText(std::string _newText) { m_textAltered = true; m_text = _newText; }

		/**
		* @returns the current text of this TextBox.
		**/
		std::string GetText() { return m_text; }

		/**
		* @returns the current text colour.
		**/
		glm::vec3 GetColour() { return m_colour; }

		/**
		* @returns the current background colour.
		**/
		glm::vec4 GetBackgroundColour() { return m_backgroundColour; }

		/*
		* @returns the bounds of the block of text.
		*/
		glm::vec2 GetTightSize();

		/*
		* @returns the anchor used to position text within this TextBox.
		*/
		GUIAnchor GetTextAnchor();

	protected:
		void UpdateGUI() override;

		void Regenerate();

		bool m_textAltered;
		std::string m_text;
		weak(Visualiser) m_visualiser;
		glm::vec3 m_colour;
		glm::vec4 m_backgroundColour;

		glm::vec2 m_tightSize;
		GUIAnchor m_textAnchor;
		glm::vec2 m_charSize;
		glm::vec2 m_charSpacing = glm::vec2(0.2f, 0.2f);
		
		float m_spaceCharWidth = 0.5f;

		bool m_fillSpaces;
		bool m_allowOverflow;
		bool m_useWordWrap;
		bool m_provideTightSize;
	};

	struct ProgressBar : public Component
	{
	public:
		ProgressBar(weak(GameObject) _parentObject);

		/**
		* Set this ProgressBar's value. _newValue should be normalised, but will be clamped anyway.
		* 
		* @param _newValue : the new normalised value to use.
		**/
		void ChangeValue(float _newValue);

	protected:
		void UpdateGUI() override;

		bool m_valueChanged = true;
		float m_value = 0.0f;

		float m_maxWidth;

		weak(Transform) m_transform;
	};

	struct InputButton : public Component
	{
	public:
		InputButton(weak(GameObject) _parentObject, bool _treatHoverAsActivation, float _hoverLingerTime = 0.0f);

		void InputUpdate(glm::ivec2 _mousePos);

		bool ShouldTriggerInputPressed() { return m_shouldTriggerInputPressed; }
		bool ShouldTriggerInputReleased() { return m_shouldTriggerInputReleased; }

	protected:
		friend struct InputManager;

		void ResetState();

		int m_id = -1;

		// settings

		// If true, the input will be triggered as the cursor hovers over and leaves the button.
		// If false, the input will be triggered in line with the concept "ONSCREEN_BUTTON_PRESS", so long as the cursor is hovering over the button.
		bool m_treatHoverAsActivation = true;

		// The length of time that the cursor must hover over the button before the input is triggered. Has no effect if treatHoverAsActivation is false. Main use case is for eye tracking dwell time.
		float m_lingerTimeBeforeActivation = 0.0f;


		// state
		bool m_cursorInside;
		bool m_conceptHeld;

		bool m_cursorInsideNotYetTriggered;
		float m_timeSinceCursorEntered;

		bool m_shouldTriggerInputPressed;
		bool m_shouldTriggerInputReleased;
	};

	//struct ContainerItem;

	//struct ContainerGUI : public TransformGUI
	//{
	//public:
	//	ContainerGUI(weak(GameObject) _parentObject, std::string _parentTransformObjectName, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, GUIAnchor _posAnchor, GUIAnchor _screenAnchor);
	//	~ContainerGUI();

	//	/**
	//	* @returns the number of items in this Container
	//	**/
	//	size_t GetItemCount() { return m_items.size(); }

	//protected:
	//	void UpdateGUI() override;

	//	void OrganiseItems();

	//	std::vector<weak(TransformGUI)> m_items;

	//	bool m_isReversed;
	//};

	//struct ContainerItem : public Component
	//{
	//public:
	//	ContainerItem();
	//	~ContainerItem();

	//private:

	//};

	//struct Button : public Component
	//{
	//public:
	//	Button(weak(GameObject) _parentObject, glm::vec2 _halfExtents);

	//protected:
	//	bool m_inFocus;

	//	weak(Button) m_nextButton[4];

	//	/*void UpdateEarly() override;
	//	void Update() override {};

	//	void MouseEnter();
	//	void MouseOver();
	//	void MouseLeft();

	//	void ClickDown();
	//	void ClickHeld();
	//	void ClickRelease();

	//	glm::vec2 m_halfExtents;
	//	bool m_mouseInsideExtents;*/

	//};

} // namespace MYENGINE

#endif
