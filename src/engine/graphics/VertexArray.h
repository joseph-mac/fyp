#ifndef _MYENGINE_VERTEXARRAY_H_
#define _MYENGINE_VERTEXARRAY_H_

#include "engine/Defines.h"
#include "engine/Resource.h"

#include <vector>
#include <string>

namespace MYENGINE
{

	struct VertexBuffer;

	struct VertexArray : public Resource
	{
	public:
		VertexArray(weak(ResourceManager) resourceManager);
		VertexArray(weak(ResourceManager) resourceManager, const char* _filePath);
		~VertexArray();

		/**
		* Replace the buffer at _location with _buffer.
		*
		* @param _location : The location to overwrite.
		* @param _buffer : The buffer to set at _location.
		**/
		void SetBuffer(int _location, shared(VertexBuffer) _buffer);

		/**
		* @returns the buffer containing vertex data.
		**/
		shared(VertexBuffer) GetVerticesBuffer() { return GetBuffer(0); }

		/**
		* @returns the buffer containing texture coordinate data.
		**/
		shared(VertexBuffer) GetTexCoordsBuffer() { return GetBuffer(1); }

		/**
		* @returns the buffer containing normal data.
		**/
		shared(VertexBuffer) GetNormalsBuffer() { return GetBuffer(2); }

		/**
		* @returns the array's file path.
		**/
		const char* GetFilePath();
		
		/**
		* @returns the number of vertices this array contains.
		**/
		int GetVertCount();

		/**
		* Manually set the array's file path.
		*
		* @param _filePath : The new file path to use.
		**/
		void SetFilePath(const char* _filePath) { m_filePath = _filePath; m_name = _filePath; }

		/**
		* @returns the array's OpenGL ID.
		**/
		unsigned int GetId() override;

	private:
		std::string m_filePath;

		std::vector<shared(VertexBuffer)> buffers;
		shared(VertexBuffer) GetBuffer(int _location) { return buffers.at(_location); }

		bool m_dirty;
		size_t m_vertCount;
	};

} // namespace MYENGINE

#endif
