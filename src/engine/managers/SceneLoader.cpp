#include "SceneLoader.h"

#include "engine/Exception.h"
#include "engine/managers/Manager.h"
#include "engine/managers/ResourceManager.h"
#include "engine/GameObject.h"
#include "engine/components/Component_Types.h"
#include "engine/components/audio/AudioComponents.h"
#include "engine/gui/GUIDefs.h"
#include "engine/components/gui/GUIComponents.h"
#include "engine/components/network/NetworkComponents.h"
#include "engine/components/physics/ColliderComponents.h"
#include "engine/components/physics/PhysicsComponents.h"

#include <fstream>

namespace MYENGINE
{

	namespace UTIL
	{

		std::vector<std::string> GetCellsFromString(std::string _string, char _seperator, bool _addLeftovers, bool _removeQuotes)
		{
			bool inQuotes = false;

			std::vector<std::string> cells{};
			std::string cell = "";
			for (int i = 0; i < (int)_string.length(); i++)
			{
				char ch = _string.at(i);
				if (ch == '"')
				{
					inQuotes = !inQuotes;
				}

				if (ch != '"' || (ch == '"' && !_removeQuotes))
				{
					if (ch == _seperator && !inQuotes)
					{
						cells.push_back(cell);
						cell = "";
					}
					else if (ch != ' ' || inQuotes)
					{
						cell.push_back(ch);
					}
				}
			}
			if (_addLeftovers) { cells.push_back(cell); }
			return cells;
		}

		int IntFromString(std::string _string)
		{
			return std::stoi(_string);
		}

		float FloatFromString(std::string _string)
		{
			return std::strtof(_string.c_str(), nullptr);
		}

		glm::vec2 Vec2FromString(std::string _string)
		{
			if (_string == "NULL") { return glm::vec2(1.0f, 1.0f); }

			std::vector<std::string> cells = GetCellsFromString(_string, ',', true, false);
			return glm::vec2(FloatFromString(cells.at(0)), FloatFromString(cells.at(1)));
		}

		glm::vec3 Vec3FromString(std::string _string)
		{
			if (_string == "NULL") { return glm::vec3(1.0f, 1.0f, 1.0f); }

			std::vector<std::string> cells = GetCellsFromString(_string, ',', true, false);
			return glm::vec3(FloatFromString(cells.at(0)), FloatFromString(cells.at(1)), FloatFromString(cells.at(2)));
		}

		glm::vec4 Vec4FromString(std::string _string)
		{
			if (_string == "NULL") { return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); }

			std::vector<std::string> cells = GetCellsFromString(_string, ',', true, false);
			return glm::vec4(FloatFromString(cells.at(0)), FloatFromString(cells.at(1)), FloatFromString(cells.at(2)), FloatFromString(cells.at(3)));
		}

		bool BoolFromString(std::string _string)
		{
			if (_string == "true")
			{
				return true;
			}
			else if (_string == "false")
			{
				return false;
			}
			else
			{
				throw Exception(("Invalid boolean value. Given string was: " + _string).c_str());
			}
		}
	} // namespace UTIL

	weak(Manager) SceneLoader::m_manager;

	bool SceneLoader::LoadFromFile(const char* _filePath)
	{
		std::ifstream file;
		file.open(_filePath);
		if (!file.is_open())
		{
			return false;
		}

		try
		{
			std::string line;
			while (std::getline(file, line))
			{
				std::vector<std::string> cells = UTIL::GetCellsFromString(line, '|', false, false);
				if (cells.size() > 0 && cells.at(0).at(0) != '#')
				{
					std::vector<std::string> nameCells = UTIL::GetCellsFromString(cells.at(0), '*', true, false);
					size_t firstTagIndex = 1;
					std::string parentName = "NULL";
					if (nameCells.size() > 1 && nameCells.at(1).at(0) != '~')
					{
						parentName = nameCells.at(1);
						firstTagIndex++;
					}

					shared(GameObject) newObject = make_shared(GameObject)(nameCells.at(0).c_str(), m_manager, parentName);
					m_manager.lock()->CreateGameObject(newObject);

					for (size_t n = firstTagIndex; n < nameCells.size(); n++)
					{
						newObject->AddTag(nameCells[n].substr(1));
					}

					for (size_t c = 0; c < cells.size() - 1; c++)
					{
						std::vector<std::string> componentCells = UTIL::GetCellsFromString(cells.at(c + 1), '*', true, true);

						if (componentCells[0] == "TRA")
						{
							newObject->AddComponent(make_shared(Transform)(newObject, UTIL::Vec3FromString(componentCells.at(1)), UTIL::Vec3FromString(componentCells.at(2)), UTIL::Vec3FromString(componentCells.at(3)), UTIL::BoolFromString(componentCells.at(4))));
						}
						else if (componentCells[0] == "GUI")
						{
							newObject->AddComponent(make_shared(TransformGUI)(newObject, UTIL::Vec3FromString(componentCells.at(1)), UTIL::Vec3FromString(componentCells.at(2)), UTIL::Vec3FromString(componentCells.at(3)), GUIAnchor(UTIL::Vec2FromString(componentCells.at(4))), GUIAnchor(UTIL::Vec2FromString(componentCells.at(5)))));
						}
						else if (componentCells[0] == "VIS")
						{
							newObject->AddComponent(make_shared(Visualiser)(newObject, componentCells.at(1).c_str(), componentCells.at(2).c_str(), componentCells.at(3).c_str()));
						}
						else if (componentCells[0] == "CAM_PERSP")
						{
							newObject->AddComponent(make_shared(PerspCamera)(newObject, UTIL::IntFromString(componentCells.at(1)), UTIL::IntFromString(componentCells.at(2)), UTIL::FloatFromString(componentCells.at(3)), UTIL::FloatFromString(componentCells.at(4))));
						}
						else if (componentCells[0] == "CAM_ORTHO")
						{
							newObject->AddComponent(make_shared(OrthoCamera)(newObject, UTIL::IntFromString(componentCells.at(1)), UTIL::IntFromString(componentCells.at(2)), UTIL::FloatFromString(componentCells.at(3)), UTIL::FloatFromString(componentCells.at(4))));
						}
						else if (componentCells[0] == "LIT_SUN")
						{
							newObject->AddComponent(make_shared(SunLight)(newObject, UTIL::Vec3FromString(componentCells.at(1))));
						}
						else if (componentCells[0] == "LIT_ORB")
						{
							newObject->AddComponent(make_shared(OrbLight)(newObject, UTIL::Vec3FromString(componentCells.at(1))));
						}
						else if (componentCells[0] == "LIT_SPT")
						{
							newObject->AddComponent(make_shared(SpotLight)(newObject, UTIL::Vec3FromString(componentCells.at(1))));
						}
						/*else if (componentCells[0] == "BUT")
						{
							newObject->AddComponent(make_shared(Button)(newObject, glm::vec2(0, 0)));
						}*/
						else if (componentCells[0] == "TXT")
						{
							newObject->AddComponent(make_shared(TextBox)(newObject, componentCells.at(1).c_str(), UTIL::FloatFromString(componentCells.at(2)), GUIAnchor(UTIL::Vec2FromString(componentCells.at(3))), UTIL::Vec3FromString(componentCells.at(4)), UTIL::Vec4FromString(componentCells.at(5))));
						}
						else if (componentCells[0] == "FCA")
						{
							newObject->AddComponent(make_shared(FreeCamera)(newObject, 30.0f, 0.4f));
						}
						else if (componentCells[0] == "DYN")
						{
							newObject->AddComponent(make_shared(DynamicPhysics)(newObject, UTIL::IntFromString(componentCells.at(1)), UTIL::FloatFromString(componentCells.at(2)), UTIL::FloatFromString(componentCells.at(3)), UTIL::Vec3FromString(componentCells.at(4))));
						}
						else if (componentCells[0] == "COL_PLA")
						{
							newObject->AddComponent(make_shared(PlaneCollider)(newObject, UTIL::Vec3FromString(componentCells.at(1))));
						}
						else if (componentCells[0] == "COL_SPH")
						{
							newObject->AddComponent(make_shared(SphereCollider)(newObject, UTIL::FloatFromString(componentCells.at(1)), UTIL::FloatFromString(componentCells.at(2))));
						}
						else if (componentCells[0] == "COL_BOX")
						{
							newObject->AddComponent(make_shared(BoxCollider)(newObject, UTIL::Vec3FromString(componentCells.at(1)), UTIL::FloatFromString(componentCells.at(2))));
						}
						else if (componentCells[0] == "NET")
						{
							newObject->AddComponent(make_shared(NetworkComponent)(newObject));
						}
						else if (componentCells[0] == "AUD")
						{
							newObject->AddComponent(make_shared(AudioSource)(newObject, m_manager.lock()->GetResourceManager()->GetAudio(componentCells.at(1).c_str()), UTIL::FloatFromString(componentCells.at(2)), UTIL::FloatFromString(componentCells.at(3))));
						}
						else
						{
							// If the current component isn't an internal engine one, check game-defined components. This function must be implemented by the user.
							if (!GAME::CheckGameComponents(newObject, componentCells))
							{
								throw Exception(("Invalid component ID found during object creation. Given ID was: " + std::string(componentCells[0])).c_str());
							}
						}
					}


					/*std::cout << "> Loaded " << newObject->GetName() << ": ";
					for (int j = 0; j < newObject->GetComponentCount(); j++)
					{
						shared(Component) comp = newObject->GetComponent(j);
						//comp->ObjectLoaded();
						std::cout << " [" << comp->GetName() << "] ";
					}
					std::cout << std::endl;*/

					// hacky way to set up the depth map as a special case
					if (newObject->GetName() == "sun")
					{
						m_manager.lock()->GetResourceManager()->m_depthMap = newObject->GetComponent<Visualiser>()->GetTexture();
					}
				}
			}
		}
		catch (std::exception& e)
		{
			std::cout << "[Scene Loader] Standard namespace has thrown: " << e.what() << std::endl;
			std::cin.get();
			return false;
		}
		catch (...)
		{
			std::cout << "[Scene Loader] Unknown object was thrown, no additional information available." << std::endl;
			std::cin.get();
			return false;
		}

		file.close();

		NOTE("Scene '" << _filePath << "' finished loading");
		return true;
	}

} // namespace MYENGINE
