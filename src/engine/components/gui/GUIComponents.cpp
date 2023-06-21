#include "GUIComponents.h"

#include "engine/managers/Manager.h"
#include "engine/managers/ResourceManager.h"
#include "engine/managers/InputManager.h"
#include "engine/GameObject.h"
#include "engine/graphics/VertexArray.h"
#include "engine/graphics/VertexBuffer.h"
#include "engine/components/physics/ColliderComponents.h"
#include "engine/components/physics/PhysicsComponents.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MYENGINE
{

	TransformGUI::TransformGUI(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale)
		: Transform(_position, _rotation, _scale)
	{
		
	}

	TransformGUI::TransformGUI(weak(GameObject) _parentObject, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, GUIAnchor _posAnchor, GUIAnchor _screenAnchor)
		: Transform(_parentObject, _position, _rotation, _scale, false)
	{
		m_originAnchor = _posAnchor;
		m_parentAnchor = _screenAnchor;
	}

	void TransformGUI::UpdateGUI()
	{
		if (GetManager()->WindowHasChanged())
		{
			SetAltered();
		}
	}

	glm::vec3 TransformGUI::GetAbsolutePosition()
	{
		// take the local position of this transform
		glm::vec2 GUIpos = glm::vec2(GetLocalPosition().x, GetLocalPosition().y);

		shared(Transform) parentTransform = nullptr;
		if (GetObject() && GetObject()->GetParentGameObject() && GetObject()->GetParentGameObject()->GetTransform())
		{
			parentTransform = GetObject()->GetParentGameObject()->GetTransform();
		}

		if (!parentTransform)
		{
			// if there isn't a parent transform, get the position of the anchor relative to the screen
			GUIpos += GetManager()->GetGUIAnchorPosition(m_parentAnchor);
		}
		else
		{
			// if there is a parent transform, find the position of the desired anchor on it and apply the offset
			glm::vec2 parentSize = parentTransform->GetAbsoluteScale();
			glm::vec2 parentAnchorOffset = glm::vec2(0.0f, 0.0f);

			shared(TextBox) parentTextBox = parentTransform->GetObject()->GetComponent<TextBox>();
			if (parentTextBox)
			{
				GUIAnchor parentTextAnchor = parentTextBox->GetTextAnchor();
				parentAnchorOffset += glm::vec2(parentSize.x * (GUIAnchorValues[(int)parentTextAnchor.x] - 0.5f), parentSize.y * (GUIAnchorValues[(int)parentTextAnchor.y] - 0.5f));

				parentSize = parentTextBox->GetTightSize() * parentSize;
				parentAnchorOffset -= glm::vec2(parentSize.x * (GUIAnchorValues[(int)parentTextAnchor.x] - 0.5f), parentSize.y * (GUIAnchorValues[(int)parentTextAnchor.y] - 0.5f));
			}

			parentAnchorOffset += glm::vec2(parentSize.x * (GUIAnchorValues[(int)m_parentAnchor.x] - 0.5f), parentSize.y * (GUIAnchorValues[(int)m_parentAnchor.y] - 0.5f));
			GUIpos += parentAnchorOffset;
		}

		// account for this transform's origin anchor
		GUIpos -= glm::vec2(GetLocalScale().x * (GUIAnchorValues[(int)m_originAnchor.x] - 0.5f), GetLocalScale().y * (GUIAnchorValues[(int)m_originAnchor.y] - 0.5f));

		// add position of parent
		glm::vec3 result = glm::vec3(GUIpos, GetLocalPosition().z);
		if (parentTransform)
		{
			result += parentTransform->GetAbsolutePosition();
		}
		return result;
	}

	glm::vec3 TransformGUI::GetAbsoluteRotation()
	{
		glm::vec3 result = GetLocalRotation();

		if (GetObject() && GetObject()->GetParentGameObject() && GetObject()->GetParentGameObject()->GetTransform())
		{
			result += GetObject()->GetParentGameObject()->GetTransform()->GetAbsoluteRotation();
		}

		return result;
	}

	glm::vec3 TransformGUI::GetAbsoluteScale()
	{
		glm::vec3 result = GetLocalScale();

		if (GetObject() && GetObject()->GetParentGameObject() && GetObject()->GetParentGameObject()->GetTransform())
		{
			//result += GetObject()->GetParentGameObject()->GetTransform()->GetAbsoluteScale(); // TODO
		}

		return result;
	}

	glm::mat4 TransformGUI::GetModelMatrix()
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
			m_model = glm::translate(m_model, GetAbsolutePosition());
			m_model = glm::rotate(m_model, glm::radians(GetAbsoluteRotation().z), glm::vec3(0, 0, 1));
			m_model = glm::rotate(m_model, glm::radians(GetAbsoluteRotation().y), glm::vec3(0, 1, 0));
			m_model = glm::rotate(m_model, glm::radians(GetAbsoluteRotation().x), glm::vec3(1, 0, 0));

			m_model = glm::scale(m_model, GetAbsoluteScale());

			m_model = glm::translate(m_model, m_secondaryPosition);
			m_model = glm::rotate(m_model, glm::radians(m_secondaryRotation.z), glm::vec3(0, 0, 1));
			m_model = glm::rotate(m_model, glm::radians(m_secondaryRotation.y), glm::vec3(0, 1, 0));
			m_model = glm::rotate(m_model, glm::radians(m_secondaryRotation.x), glm::vec3(1, 0, 0));

			m_secondaryPosition = glm::vec3(0.0f);
			//m_position = m_model[3];

			m_altered = false;
		}

		return m_model;
	}



	TextBox::TextBox(weak(GameObject) _parentObject)
		: Component(_parentObject, "TEXT_BOX")
	{
		m_text = "NULL";
	}

	TextBox::TextBox(weak(GameObject) _parentObject, const char* _text, float _size, GUIAnchor _textAnchor, glm::vec3 _colour, glm::vec4 _backgroundColour)
		: Component(_parentObject, "TEXT_BOX")
	{
		m_text = _text;
		m_textAltered = true;
		m_visualiser = GetObject()->GetComponent<Visualiser>();
		m_colour = _colour;
		m_backgroundColour = _backgroundColour;

		m_tightSize = glm::vec2(0.0f, 0.0f);
		m_charSize = glm::vec2(_size, _size);
		m_textAnchor = _textAnchor;

		m_allowOverflow = false;
		m_useWordWrap = true;
		m_fillSpaces = false;
		m_provideTightSize = true;
	}

	void TextBox::UpdateGUI()
	{
		if (GetManager()->GetInputManager()->GetInputConceptState("DEBUG_ADD_CHAR").changedToNonZero)
		{
			SetText(GetText() + "E");
		}

		if (GetManager()->GetInputManager()->GetInputConceptState("DEBUG_ADD_SPACE").changedToNonZero)
		{
			SetText(GetText() + " ");
		}

		if (m_textAltered)
		{
			Regenerate();

			if (m_provideTightSize)
			{
				GetObject()->GetTransform()->Scale(1);
			}
		}
	}

	void TextBox::Regenerate()
	{
		shared(Visualiser) vis = m_visualiser.lock();

		shared(VertexArray) vertArray;
		if (GetManager()->GetResourceManager()->VertexArrayExists(GetObject()->GetName().c_str()))
		{
			vertArray = GetManager()->GetResourceManager()->GetVertexArray(GetObject()->GetName().c_str());
		}
		else
		{
			vertArray = GetManager()->GetResourceManager()->CreateVertexArray();
			vertArray->SetFilePath(GetObject()->GetName().c_str());
		}

		shared(VertexBuffer) vertBuffer = make_shared(VertexBuffer)();
		shared(VertexBuffer) texCoordsBuffer = make_shared(VertexBuffer)();
		shared(VertexBuffer) normsBuffer = make_shared(VertexBuffer)();

		glm::vec2 objectScale = glm::vec2(GetObject()->GetTransform()->GetAbsoluteScale().x, GetObject()->GetTransform()->GetAbsoluteScale().y);

		// These values represent a percentage of the object's scale

		// The padding value will be set based on the smallest value from both axes
		glm::vec2 parentRelative_padding = glm::vec2(0.1f, 0.1f);
		parentRelative_padding *= objectScale;
		parentRelative_padding = glm::vec2(glm::min(parentRelative_padding.x, parentRelative_padding.y)) / objectScale;

		glm::vec2 parentRelative_maxSize = glm::vec2(1.0f) - (parentRelative_padding * 2.0f);
		glm::vec2 parentRelative_charWidth = m_charSize / (objectScale * parentRelative_maxSize);
		glm::vec2 parentRelative_charWidthWithPadding = parentRelative_charWidth * (glm::vec2(1.0f) + m_charSpacing);
		glm::vec2 parentRelative_spaceWidth = parentRelative_charWidth * m_spaceCharWidth;

		std::vector<std::string> words = {};
		size_t segmentStartPos = 0;
		while (segmentStartPos < m_text.length())
		{
			size_t nextSpacePos = m_text.find(' ', segmentStartPos);

			if (nextSpacePos == std::string::npos)
			{
				words.push_back(m_text.substr(segmentStartPos));
				break;
			}
			else
			{
				words.push_back(m_text.substr(segmentStartPos, nextSpacePos - segmentStartPos));
				segmentStartPos = nextSpacePos + 1;
			}
		}

		std::vector<std::string> lines = {};
		std::vector<float> lineWidths = {};

		std::string currentLine = "";
		float currentLineWidth = parentRelative_maxSize.x + 1.0f;
		float currentTotalHeight = 0.0f;
		float totalWidth = 0.0f;
		for (size_t w = 0; w < words.size(); w++)
		{
			std::string word = words[w];
			float wordWidth = parentRelative_charWidth.x + ((word.length() - 1) * parentRelative_charWidthWithPadding.x);

			if (wordWidth > parentRelative_maxSize.x)
			{
				// if any single word is too large to fit inside the element there isn't really anything elegant that we can do, so stop 'writing'
				break;
			}
			else if (currentLineWidth + parentRelative_spaceWidth.x + wordWidth <= parentRelative_maxSize.x)
			{
				// if this word will fit on this line, add it and continue to next word

				currentLine.push_back(' ');
				currentLine.append(word);
				currentLineWidth += parentRelative_spaceWidth.x + wordWidth;
			}
			else if (m_useWordWrap)
			{
				// if this word will not fit on the current line, finalise the line and add this word to the next

				float nextLineHeight = parentRelative_charWidth.y;
				if (!currentLine.empty())
				{
					// this is only executed for lines other than the first
					lines.push_back(currentLine);
					lineWidths.push_back(currentLineWidth);

					nextLineHeight = parentRelative_charWidthWithPadding.y;
				}

				// new line needed, so check to see if a new line would fit inside the element
				if (currentTotalHeight + nextLineHeight <= parentRelative_maxSize.y || m_allowOverflow)
				{
					currentLine = word;
					currentLineWidth = wordWidth;
					currentTotalHeight += nextLineHeight;
				}
				else
				{
					// a new line wouldn't fit, so stop 'writing'
					break;
				}
			}
			else
			{
				// a new line is needed, but word wrap is disabled, so stop 'writing'
				break;
			}

			if (w == words.size() - 1)
			{
				lines.push_back(currentLine);
				lineWidths.push_back(currentLineWidth);
				currentTotalHeight += parentRelative_charWidthWithPadding.y;
			}

			if (currentLineWidth > totalWidth)
			{
				totalWidth = currentLineWidth;
			}
		}


		if (lines.empty())
		{
			m_textAltered = false;
			return; // TODO return blank/error vertex array
		}

		float totalHeight = parentRelative_charWidth.y + ((lines.size() - 1) * parentRelative_charWidthWithPadding.y);

		m_tightSize = glm::vec2(totalWidth, totalHeight) + (parentRelative_padding * 2.0f);


		// add backing quad

		glm::vec2 pad = parentRelative_padding;

		glm::vec2 totalMin = -pad;

		switch (m_textAnchor.x)
		{
		case GUIAnchorValue::LEFT:
			totalMin.x += -0.5f + pad.x;
			break;

		case GUIAnchorValue::CENTRE:
			totalMin.x += -(totalWidth * 0.5f);
			break;

		case GUIAnchorValue::RIGHT:
			totalMin.x += 0.5f - pad.x - totalWidth;
			break;
		}

		switch (m_textAnchor.y)
		{
		case GUIAnchorValue::BOTTOM:
			totalMin.y += -0.5f + pad.y;
			break;

		case GUIAnchorValue::MIDDLE:
			totalMin.y += totalHeight * 0.5f;
			break;

		case GUIAnchorValue::TOP:
			totalMin.y += 0.5f - totalHeight - pad.y;
			break;
		}

		glm::vec2 totalMax = totalMin + glm::vec2(totalWidth, totalHeight) + (pad * 2.0f);

		vertBuffer->Add(glm::vec3(totalMin.x, totalMin.y, -0.1f));
		vertBuffer->Add(glm::vec3(totalMax.x, totalMin.y, -0.1f));
		vertBuffer->Add(glm::vec3(totalMax.x, totalMax.y, -0.1f));

		vertBuffer->Add(glm::vec3(totalMax.x, totalMax.y, -0.1f));
		vertBuffer->Add(glm::vec3(totalMin.x, totalMax.y, -0.1f));
		vertBuffer->Add(glm::vec3(totalMin.x, totalMin.y, -0.1f));


		texCoordsBuffer->Add(glm::vec2(-0.1f, -0.1f));
		texCoordsBuffer->Add(glm::vec2(-0.1f, -0.1f));
		texCoordsBuffer->Add(glm::vec2(-0.1f, -0.1f));

		texCoordsBuffer->Add(glm::vec2(-0.1f, -0.1f));
		texCoordsBuffer->Add(glm::vec2(-0.1f, -0.1f));
		texCoordsBuffer->Add(glm::vec2(-0.1f, -0.1f));


		normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));

		normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));


		glm::vec2 charOffset = glm::vec2(0.0f, 0.0f);
		for (int l = 0; l < lines.size(); l++)
		{
			std::string thisLine = lines[l];
			float thisWidth = lineWidths[l];

			// This defines the location of the BOTTOM LEFT of the first character
			glm::vec2 parentRelative_zero = glm::vec2(-0.5f, -0.5f);
			switch (m_textAnchor.x)
			{
			case GUIAnchorValue::LEFT:
				parentRelative_zero.x = -0.5f + parentRelative_padding.x;
				break;

			case GUIAnchorValue::CENTRE:
				parentRelative_zero.x = -(thisWidth * 0.5f);
				break;

			case GUIAnchorValue::RIGHT:
				parentRelative_zero.x = 0.5f - parentRelative_padding.x - thisWidth;
				break;
			}

			switch (m_textAnchor.y)
			{
			case GUIAnchorValue::BOTTOM:
				parentRelative_zero.y = (-0.5f - parentRelative_charWidth.y) + parentRelative_padding.y + totalHeight;
				break;

			case GUIAnchorValue::MIDDLE:
				parentRelative_zero.y = (-parentRelative_charWidth.y) + (totalHeight * 0.5f);
				break;

			case GUIAnchorValue::TOP:
				parentRelative_zero.y = (0.5f - parentRelative_padding.y - parentRelative_charWidth.y);
				break;
			}

			charOffset.x = 0.0f;
			glm::vec4 charTexCoords = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			for (size_t c = 0; c < thisLine.length(); c++)
			{
				if (thisLine[c] != ' ')
				{
					glm::vec2 min = parentRelative_zero + charOffset;
					glm::vec2 max = min + parentRelative_charWidth;

					vertBuffer->Add(glm::vec3(min.x, min.y, 0.0f));
					vertBuffer->Add(glm::vec3(max.x, min.y, 0.0f));
					vertBuffer->Add(glm::vec3(max.x, max.y, 0.0f));

					vertBuffer->Add(glm::vec3(max.x, max.y, 0.0f));
					vertBuffer->Add(glm::vec3(min.x, max.y, 0.0f));
					vertBuffer->Add(glm::vec3(min.x, min.y, 0.0f));


					int thisCharID = thisLine[c] - '!';
					glm::vec2 mapPos = glm::vec2(thisCharID, 0.0f);
					while (mapPos.x > 7)
					{
						mapPos.y += 1;
						mapPos.x -= 8;
					}

					charTexCoords.x = (mapPos.x * 0.125f) + 0.02777778f;
					charTexCoords.y = (mapPos.y * 0.125f) + 0.02777778f;
					charTexCoords.z = charTexCoords.x + 0.125f - (2.0f * 0.02777778f);
					charTexCoords.w = charTexCoords.y + 0.125f - (2.0f * 0.02777778f);

					texCoordsBuffer->Add(glm::vec2(charTexCoords.x, charTexCoords.w));
					texCoordsBuffer->Add(glm::vec2(charTexCoords.z, charTexCoords.w));
					texCoordsBuffer->Add(glm::vec2(charTexCoords.z, charTexCoords.y));

					texCoordsBuffer->Add(glm::vec2(charTexCoords.z, charTexCoords.y));
					texCoordsBuffer->Add(glm::vec2(charTexCoords.x, charTexCoords.y));
					texCoordsBuffer->Add(glm::vec2(charTexCoords.x, charTexCoords.w));


					normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));
					normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));
					normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));

					normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));
					normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));
					normsBuffer->Add(glm::vec3(0.0f, 0.0f, 1.0f));

					if (c + 1 < thisLine.length() && thisLine[c + 1] == ' ')
					{
						charOffset.x += parentRelative_charWidth.x;
					}
					else
					{
						charOffset.x += parentRelative_charWidthWithPadding.x;
					}
				}
				else
				{
					charOffset.x += parentRelative_spaceWidth.x;
				}
			}
			charOffset.y -= parentRelative_charWidthWithPadding.y;
		}

		vertArray->SetBuffer(0, vertBuffer);
		vertArray->SetBuffer(1, texCoordsBuffer);
		vertArray->SetBuffer(2, normsBuffer);

		vis->SetVertexArray(GetObject()->GetName().c_str());

		m_textAltered = false;
	}

	glm::vec2 TextBox::GetTightSize()
	{
		if (!m_provideTightSize)
		{
			return GetObject()->GetTransform()->GetAbsoluteScale();
		}

		return m_tightSize;
	}

	GUIAnchor TextBox::GetTextAnchor()
	{
		return m_textAnchor;
	}


	ProgressBar::ProgressBar(weak(GameObject) _parentObject)
		: Component(_parentObject, "PROGRESS_BAR")
	{
		m_transform = GetObject()->GetTransform();

		m_maxWidth = m_transform.lock()->GetLocalScale().x;
	}

	void ProgressBar::ChangeValue(float _newValue)
	{
		m_valueChanged = true;
		
		if (_newValue > 1.0f)
		{
			m_value = 1.0f;
			//WARN("ProgressBar has been provided a non-normalised value!");
		}
		else if (_newValue < 0.0f)
		{
			m_value = 0.0f;
			//WARN("ProgressBar has been provided a non-normalised value!");
		}
		else
		{
			m_value = _newValue;
		}
	}

	void ProgressBar::UpdateGUI()
	{
		if (m_valueChanged && GetObject()->GetComponent<Visualiser>()->IsVisible())
		{
			shared(Transform) t = m_transform.lock();
			t->SetScale(glm::vec3(m_value * m_maxWidth, t->GetLocalScale().y, t->GetLocalScale().z));
		}
	}


	InputButton::InputButton(weak(GameObject) _parentObject, bool _treatHoverAsActivation, float _hoverLingerTime)
		: Component(_parentObject, "INPUT_BUTTON")
	{
		m_treatHoverAsActivation = _treatHoverAsActivation;
		m_lingerTimeBeforeActivation = _hoverLingerTime;

		ResetState();
	}

	void InputButton::ResetState()
	{
		m_cursorInside = false;
		m_conceptHeld = false;

		m_cursorInsideNotYetTriggered = false;
		 m_timeSinceCursorEntered = 0.0f;

		m_shouldTriggerInputPressed = false;
		m_shouldTriggerInputReleased = false;
	}

	void InputButton::InputUpdate(glm::ivec2 _mousePos)
	{
		m_shouldTriggerInputPressed = false;
		m_shouldTriggerInputReleased = false;

		ConceptState pressConceptState = GetObject()->GetManager()->GetInputManager()->GetInputConceptState("ONSCREEN_BUTTON_PRESS");
		glm::ivec2 objPos = GetObject()->GetTransform()->GetAbsolutePosition();
		glm::ivec2 objSize = GetObject()->GetTransform()->GetAbsoluteScale();

		bool timeSinceCursorEnteredChanged = false;

		// if the cursor is 'inside' the button this frame
		if ((_mousePos.x > objPos.x - (objSize.x / 2)) && (_mousePos.x < objPos.x + (objSize.x / 2)) && (_mousePos.y > objPos.y - (objSize.y / 2)) && (_mousePos.y < objPos.y + (objSize.y / 2)))
		{
			if (m_treatHoverAsActivation)
			{
				if (m_cursorInside)
				{
					if (m_timeSinceCursorEntered < m_lingerTimeBeforeActivation)
					{
						m_timeSinceCursorEntered += GetObject()->GetManager()->RawDeltaTime();
						timeSinceCursorEnteredChanged = true;
					}
				}
				else
				{
					m_cursorInside = true;
					m_cursorInsideNotYetTriggered = true;
					m_timeSinceCursorEntered = 0.0f;
					timeSinceCursorEnteredChanged = true;
				}

				if (m_cursorInsideNotYetTriggered && m_timeSinceCursorEntered >= m_lingerTimeBeforeActivation)
				{
					m_shouldTriggerInputPressed = true;
					m_cursorInsideNotYetTriggered = false;
				}
			}
			else
			{
				if (!m_cursorInside)
				{
					m_cursorInside = true;
				}

				if (pressConceptState.changedToNonZero)
				{
					m_shouldTriggerInputPressed = true;
					m_conceptHeld = true;
				}
				else if (pressConceptState.changedToZero)
				{
					m_shouldTriggerInputReleased = true;
					m_conceptHeld = false;
				}
			}
		}
		// if the cursor is 'outside' the button this frame
		else
		{
			// if the cursor was 'inside' the button in the previous frame
			if (m_cursorInside)
			{
				m_cursorInside = false;

				if (m_treatHoverAsActivation)
				{
					m_timeSinceCursorEntered = 0.0f;
					timeSinceCursorEnteredChanged = true;

					if (!m_cursorInsideNotYetTriggered)
					{
						m_shouldTriggerInputReleased = true;
					}
				}
				else
				{
					if (m_conceptHeld)
					{
						m_shouldTriggerInputReleased = true;
					}
				}
			}
		}

		if (m_treatHoverAsActivation && timeSinceCursorEnteredChanged)
		{
			for (weak(GameObject) c : GetObject()->GetChildGameObjects())
			{
				shared(GameObject) child = c.lock();
				if (child && child->GetComponent<ProgressBar>())
				{
					child->GetComponent<ProgressBar>()->ChangeValue(m_timeSinceCursorEntered / m_lingerTimeBeforeActivation);
				}
			}
		}
	}



	/*ContainerGUI::ContainerGUI(weak(GameObject) _parentObject, std::string _parentTransformObjectName, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, GUIAnchor _posAnchor, GUIAnchor _screenAnchor)
		: TransformGUI(_parentObject, _parentTransformObjectName, _position, _rotation, _scale, _posAnchor, _screenAnchor)
	{

	}

	ContainerGUI::~ContainerGUI()
	{

	}

	void ContainerGUI::UpdateGUI()
	{
		TransformGUI::UpdateGUI();

		if (m_altered)
		{
			OrganiseItems();
		}
	}

	void ContainerGUI::OrganiseItems()
	{
		for (size_t t = 0; t < m_childTransforms.size(); t++)
		{
			shared(TransformGUI) current = std::dynamic_pointer_cast<TransformGUI>(m_childTransforms[t].lock());
			
		}
	}*/

} // namespace MYENGINE
