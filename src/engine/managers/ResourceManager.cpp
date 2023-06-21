#include "ResourceManager.h"

#include "engine/managers/Manager.h"
#include "engine/managers/SceneLoader.h"
#include "engine/GameObject.h"
#include "engine/components/Component_Types.h"
#include "engine/graphics/VertexArray.h"
#include "engine/graphics/ShaderProgram.h"
#include "engine/graphics/Texture.h"
#include "engine/audio/Audio.h"

namespace MYENGINE
{

	ResourceManager::ResourceManager(weak(Manager) manager)
		: SubManager(manager)
	{
		m_cachedVertexArrays = std::vector<shared(VertexArray)>{};
		m_cachedShaderPrograms = std::vector<shared(ShaderProgram)>{};
		m_cachedTextures = std::vector<shared(Texture)>{};
		m_renderTextures = std::vector<weak(RenderTexture)>{};
		m_cachedAudio = std::vector<shared(Audio)>{};
		
		m_depthShader = CreateShaderProgram("depth-map");
		m_defaultTexture = CreateTexture("../../data/textures/default.bmp");
		CreateAudio("../../data/audio/dixie_horn.ogg");
	}

	ResourceManager::~ResourceManager()
	{
		//std::cout << "Resource manager destructing..." << std::endl;

		m_cachedVertexArrays.clear();
		m_cachedShaderPrograms.clear();
		m_cachedTextures.clear();

		//std::cout << "Resource manager destructed!" << std::endl;
	}

	shared(VertexArray) ResourceManager::GetVertexArray(const char* resourceName)
	{
		for (int i = 0; i < m_cachedVertexArrays.size(); i++)
		{
			if (strcmp(m_cachedVertexArrays.at(i)->GetFilePath(), resourceName) == 0)
			{
				return m_cachedVertexArrays[i];
			}
		}
		//std::cout << "- Loading vertex array " << resourceName << std::endl;
		return CreateVertexArray(resourceName);
	}

	shared(ShaderProgram) ResourceManager::GetShaderProgram(const char* resourceName)
	{
		for (int i = 0; i < m_cachedShaderPrograms.size(); i++)
		{
			if (m_cachedShaderPrograms.at(i)->GetName() == resourceName)
			{
				return m_cachedShaderPrograms[i];
			}
		}
		//std::cout << "- Loading shader program " << resourceName << std::endl;
		return CreateShaderProgram(resourceName);
	}

	shared(Texture) ResourceManager::GetTexture(const char* resourceName)
	{
		for (int i = 0; i < m_cachedTextures.size(); i++)
		{
			if (strcmp(m_cachedTextures.at(i)->GetFilePath(), resourceName) == 0)
			{
				return m_cachedTextures[i];
			}
		}

		for (int c = 0; c < GetManager()->GetCameraCount(); c++) // see if the desired texture is a camera, in which case a render texture is desired
		{
			shared(Camera) cam = GetManager()->GetCamera(c);
			if (strcmp(cam->GetObject()->GetName().c_str(), resourceName) == 0)
			{
				//std::cout << "- Creating render texture for camera '" << resourceName << "'" << std::endl;
				if (strcmp("sun", resourceName) == 0 || strcmp("spot_light", resourceName) == 0 || strcmp("spot_light2", resourceName) == 0)
				{
					return CreateRenderTexture(resourceName, cam, m_depthShader);
				}
				else
				{
					return CreateRenderTexture(resourceName, cam, weak(ShaderProgram)());
				}
			}
		}

		std::vector<std::string> nameSegments = UTIL::GetCellsFromString(resourceName, '?', true, false);
		if (!nameSegments[0].empty() && nameSegments[0].back() == '/' && nameSegments.size() == 2)
		{
			//std::cout << "- Loading cube texture " << resourceName << std::endl;
			return CreateCubeTexture(nameSegments[0].c_str(), nameSegments[1].c_str());
		}

		//std::cout << "- Loading texture " << resourceName << std::endl;
		return CreateTexture(resourceName);
	}

	shared(Audio) ResourceManager::GetAudio(const char* resourceName)
	{
		for (int i = 0; i < m_cachedAudio.size(); i++)
		{
			if (strcmp(m_cachedAudio.at(i)->GetFilePath(), resourceName) == 0)
			{
				return m_cachedAudio[i];
			}
		}
		//std::cout << "- Loading audio " << resourceName << std::endl;
		return CreateAudio(resourceName);
	}

	shared(VertexArray) ResourceManager::CreateVertexArray(const char* _filePath)
	{
		shared(VertexArray) newVertArray = make_shared(VertexArray)(GetManager()->GetResourceManager(), _filePath);
		m_cachedVertexArrays.push_back(newVertArray);
		return newVertArray;
	}
	shared(VertexArray) ResourceManager::CreateVertexArray()
	{
		shared(VertexArray) newVertArray = make_shared(VertexArray)(GetManager()->GetResourceManager());
		m_cachedVertexArrays.push_back(newVertArray);
		return newVertArray;
	}

	shared(ShaderProgram) ResourceManager::CreateShaderProgram(const char* _filePath)
	{
		shared(ShaderProgram) newShaderProgram = make_shared(ShaderProgram)(GetManager()->GetResourceManager(), _filePath);
		m_cachedShaderPrograms.push_back(newShaderProgram);
		return newShaderProgram;
	}

	shared(Texture) ResourceManager::CreateTexture(const char* _filePath)
	{
		shared(Texture) newTexture = make_shared(Texture)(GetManager()->GetResourceManager(), _filePath);
		m_cachedTextures.push_back(newTexture);
		newTexture->Initialise();
		return newTexture;
	}

	shared(Texture) ResourceManager::CreateRenderTexture(const char* _name, weak(Camera) _camera, weak(ShaderProgram) _shaderProgram)
	{
		shared(RenderTexture) newRenderTexture = make_shared(RenderTexture)(GetManager()->GetResourceManager(), _name, _camera, _shaderProgram);
		m_cachedTextures.push_back(newRenderTexture);

		newRenderTexture->Initialise();
		m_renderTextures.push_back(newRenderTexture);
		return newRenderTexture;
	}

	shared(Texture) ResourceManager::CreateCubeTexture(const char* _filePath, const char* _fileType)
	{
		shared(CubeTexture) newCubeTexture = make_shared(CubeTexture)(GetManager()->GetResourceManager(), _filePath, _fileType);
		m_cachedTextures.push_back(newCubeTexture);
		newCubeTexture->Initialise();
		return newCubeTexture;
	}

	shared(Audio) ResourceManager::CreateAudio(const char* _filePath)
	{
		shared(Audio) newAudio = make_shared(Audio)(GetManager()->GetResourceManager(), _filePath);
		m_cachedAudio.push_back(newAudio);
		newAudio->Initialise();
		return newAudio;
	}

	bool ResourceManager::VertexArrayExists(const char* resourceName)
	{
		for (int i = 0; i < m_cachedVertexArrays.size(); i++)
		{
			if (strcmp(m_cachedVertexArrays.at(i)->GetFilePath(), resourceName) == 0)
			{
				return true;
			}
		}
		return false;
	}

} // namespace MYENGINE
