#ifndef _MYENGINE_SCENELOADER_H_
#define _MYENGINE_SCENELOADER_H_

#include "engine/Defines.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <string>

namespace MYENGINE
{
	
	struct Manager;
	struct GameObject;

	struct SceneLoader
	{
	public:
		/**
		* Load GameObjects into the engine from a scene text file.
		*
		* @param _filePath : The file path of the scene text file.
		* @returns Whether the loading was successful.
		**/
		static bool LoadFromFile(const char* _filePath);

	private:
		friend class Core;

		static weak(Manager) m_manager;
	};

	namespace UTIL
	{

		std::vector<std::string> GetCellsFromString(std::string _string, char _seperator, bool _addLeftovers, bool _removeQuotes);
		int IntFromString(std::string _string);
		float FloatFromString(std::string _string);
		glm::vec2 Vec2FromString(std::string _string);
		glm::vec3 Vec3FromString(std::string _string);
		glm::vec4 Vec4FromString(std::string _string);
		bool BoolFromString(std::string _string);

	} // namespace UTIL

} // namespace MYENGINE


namespace GAME
{

	extern bool CheckGameComponents(shared(MYENGINE::GameObject) _newObject, std::vector<std::string> _componentCells);

} // namespace GAME

#endif
