#ifndef _MYENGINE_TEXTURE_H_
#define _MYENGINE_TEXTURE_H_

#include "engine/Defines.h"
#include "engine/Resource.h"

#include <glm/vec2.hpp>

#include <string>

typedef unsigned int GLuint;

namespace MYENGINE
{

	struct Camera;
	struct ShaderProgram;

	struct Texture : public Resource
	{
	public:
		Texture(weak(ResourceManager) resourceManager, const char* _filePath);
		~Texture();

		virtual void Initialise() override;

		/**
		* @returns the Texture's file path.
		**/
		const char* GetFilePath();

		/**
		* If this instance is not of the subtype RenderTexture, this function will always return -1.
		* 
		* @returns the Texture's frame buffer ID.
		**/
		virtual GLuint GetFrameBufferID() { return -1; }

		/**
		* @returns whether the Texture contains transparency
		**/
		bool HasTransparency() { return m_hasTransparency; }

	protected:
		std::string m_filePath;

		bool m_hasTransparency;
	};

	struct RenderTexture : Texture
	{
	public:
		RenderTexture(weak(ResourceManager) resourceManager, const char* _filePath, weak(Camera) _camera, weak(ShaderProgram) _shaderProgram);

		void Initialise() override;

		/**
		* @returns the shader program resource used with this RenderTexture
		**/
		shared(ShaderProgram) GetShaderProgram() { return m_linkedShaderProgram.lock(); }

		/**
		* @returns the Camera component used with this RenderTexture
		**/
		weak(Camera) GetLinkedCamera() { return m_linkedCamera; }

		/**
		* @returns the RenderTexture's frame buffer ID
		**/
		GLuint GetFrameBufferID() { return m_fbo; }

	private:
		glm::vec2 m_actualDimensions;

		weak(ShaderProgram) m_linkedShaderProgram;
		weak(Camera) m_linkedCamera;

		GLuint m_fbo;
	};

	struct CubeTexture : Texture
	{
	public:
		CubeTexture(weak(ResourceManager) resourceManager, const char* _filePath, const char* _fileType);

		void Initialise() override;

	private:
		std::string m_fileType;
	};

} // namespace MYENGINE

#endif
