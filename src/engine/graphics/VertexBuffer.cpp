#include "VertexBuffer.h"

#include "engine/Exception.h"

#include <GL/glew.h>

namespace MYENGINE
{

	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &m_id);
		if (!m_id) { throw Exception("Vertex buffer failed to generate."); }

		m_dirty = true;
		m_componentCount = 0;
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::Added(int _componentCount)
	{
		m_vertCount++;
		m_componentCount = _componentCount;
		m_dirty = true;
	}

	void VertexBuffer::Add(glm::vec2 _value)
	{
		m_data.push_back(_value.x);
		m_data.push_back(_value.y);
		Added(2);
	}

	void VertexBuffer::Add(glm::vec3 _value)
	{
		m_data.push_back(_value.x);
		m_data.push_back(_value.y);
		m_data.push_back(_value.z);
		Added(3);
	}

	void VertexBuffer::Add(glm::vec4 _value)
	{
		m_data.push_back(_value.x);
		m_data.push_back(_value.y);
		m_data.push_back(_value.z);
		m_data.push_back(_value.w);
		Added(4);
	}

	GLuint VertexBuffer::GetID()
	{
		if (m_dirty)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id);

			// upload modified data into the new VBO
			glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(m_data.at(0)), &m_data.at(0), GL_STATIC_DRAW);

			// reset state
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_dirty = false;
		}

		return m_id;
	}

	int VertexBuffer::GetComponentCount() { return m_componentCount; }
	int VertexBuffer::GetVertCount() { return m_vertCount; }

} // namespace MYENGINE
