#ifndef _MYENGINE_RESOURCEMANAGER_H_
#define _MYENGINE_RESOURCEMANAGER_H_

#include "engine/Defines.h"
#include "engine/managers/SubManager.h"

#include <vector>

namespace MYENGINE
{

	struct Manager;
	struct Camera;
	struct VertexArray;
	struct ShaderProgram;
	struct Texture;
	struct RenderTexture;
	struct Audio;
	struct Primitives;
	struct GameObject;
	struct TextBox;

	struct ResourceManager : public SubManager
	{
	public:
		ResourceManager(weak(Manager) manager);
		~ResourceManager();

		/**
		* Retrieve the named VertexArray from cache, or load it in.
		*
		* @param resourceName : The name of the resource.
		* @returns the VertexArray.
		**/
		shared(VertexArray) GetVertexArray(const char* resourceName);

		/**
		* Retrieve the named ShaderProgram from cache, or load it in.
		*
		* @param resourceName : The name of the resource.
		* @returns the ShaderProgram.
		**/
		shared(ShaderProgram) GetShaderProgram(const char* resourceName);

		/**
		* Retrieve the named Texture from cache. If not able to, and the resourceName matches the name of a GameObject with a Camera component, create a RenderTexture. Otherwise, load the Texture in.
		*
		* @param resourceName : The name of the resource.
		* @returns the Texture.
		**/
		shared(Texture) GetTexture(const char* resourceName);

		/**
		* Retrieve the named Audio from cache, or load it in.
		*
		* @param resourceName : The name of the resource.
		* @returns the Audio.
		**/
		shared(Audio) GetAudio(const char* resourceName);

		/**
		* Check if the named VertexArray exists in cache.
		*
		* @param resourceName : The name of the resource.
		* @returns whether the named VertexArray exists in cache.
		**/
		bool VertexArrayExists(const char* resourceName);

		/**
		* @returns the default texture to be used as a fallback.
		**/
		shared(Texture) GetDefaultTexture() { return m_defaultTexture.lock(); }

	private:
		friend struct Primitives;
		friend struct GameObject;
		friend struct Manager;
		friend struct SceneLoader;
		friend struct TextBox;

		const char* GetDataDir() { return "../../data/"; }

		shared(VertexArray) CreateVertexArray(const char* _filePath);
		shared(VertexArray) CreateVertexArray();

		shared(ShaderProgram) CreateShaderProgram(const char* _filePath);

		shared(Texture) CreateTexture(const char* _filePath);
		shared(Texture) CreateRenderTexture(const char* _name, weak(Camera) _camera, weak(ShaderProgram) _shaderProgram);
		shared(Texture) CreateCubeTexture(const char* _filePath, const char* _fileType);

		shared(Audio) CreateAudio(const char* _filePath);

		// Cached models
		std::vector<shared(VertexArray)> m_cachedVertexArrays;

		// Cached shader programs
		std::vector<shared(ShaderProgram)> m_cachedShaderPrograms;

		// Cached textures
		std::vector<shared(Texture)> m_cachedTextures;
		std::vector<weak(RenderTexture)> m_renderTextures;

		// Cached audio
		std::vector<shared(Audio)> m_cachedAudio;

		// Special instances
		weak(ShaderProgram) m_depthShader;
		weak(Texture) m_depthMap;

		// default instances
		weak(Texture) m_defaultTexture;
	};

} // namespace MYENGINE

#endif
