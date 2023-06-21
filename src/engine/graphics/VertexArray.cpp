#include "VertexArray.h"

#include "VertexBuffer.h"
#include "engine/Exception.h"

#include "bugl.h"

namespace MYENGINE
{

	VertexArray::VertexArray(weak(ResourceManager) resourceManager)
		: Resource(resourceManager, "UNNAMED_RESOURCE")
	{
		glGenVertexArrays(1, &m_id);

		if (!m_id) { throw Exception("Vertex array failed to generate."); }

		m_dirty = true;
		buffers.resize(20);
		m_vertCount = 0;
	}

	VertexArray::VertexArray(weak(ResourceManager) resourceManager, const char* _filePath)
		: Resource(resourceManager, _filePath)
	{
		m_filePath = _filePath;
		m_dirty = false;
		m_vertCount = 0;

		// this will need attention in the future re: memory cleanup!
		m_id = buLoadModel(_filePath, &m_vertCount);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::SetBuffer(int _location, shared(VertexBuffer) _buffer)
	{
		buffers.at(_location) = _buffer;
		m_dirty = true;
	}

	unsigned int VertexArray::GetId()
	{
		if (m_dirty)
		{
			glBindVertexArray(m_id);

			for (int i = 0; i < (int)buffers.size(); i++)
			{
				if (!buffers.at(i)) { continue; }

				glBindBuffer(GL_ARRAY_BUFFER, buffers.at(i)->GetID());

				glVertexAttribPointer(i, buffers.at(i)->GetComponentCount(), GL_FLOAT, GL_FALSE, 0, (void*)0);

				glEnableVertexAttribArray(i);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			glBindVertexArray(0);
			m_dirty = false;
			m_vertCount = buffers.at(0)->GetVertCount();
		}

		return m_id;
	}

	const char* VertexArray::GetFilePath() { return m_filePath.c_str(); }

	int VertexArray::GetVertCount() { return (int)m_vertCount; }

} // namespace MYENGINE
