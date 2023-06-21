#ifndef _MYENGINE_VERTEXBUFFER_H_
#define _MYENGINE_VERTEXBUFFER_H_

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

typedef unsigned int GLuint;

namespace MYENGINE
{

	struct VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		/**
		* Add a 2D vector to the buffer.
		* 
		* @param _value : The value to add.
		**/
		void Add(glm::vec2 _value);

		/**
		* Add a 3D vector to the buffer.
		*
		* @param _value : The value to add.
		**/
		void Add(glm::vec3 _value);

		/**
		* Add a 4D vector to the buffer.
		*
		* @param _value : The value to add.
		**/
		void Add(glm::vec4 _value);

		/**
		* @returns the buffer's OpenGL ID.
		**/
		GLuint GetID();

		/**
		* This is dependent on the most recent Add() function called.
		* 
		* @returns the buffer's component count.
		**/
		int GetComponentCount();

		/**
		* @returns the number of vertices stored in this buffer.
		**/
		int GetVertCount();

	private:
		void Added(int _componentCount);

		GLuint m_id;
		std::vector<float> m_data;

		bool m_dirty;
		int m_componentCount = 0;
		int m_vertCount = 0;
	};

} // namespace MYENGINE

#endif
