#include "SettingsManager.h"

#include "engine/Exception.h"
#include "engine/managers/Manager.h"
#include "engine/managers/InputManager.h"
#include "engine/input/InputSystemDefs.h"
#include "engine/input/InputDefs.h"
#include "engine/managers/SceneLoader.h"

#include <fstream>

#define CHECK_SETTING_TYPE(t, alias) (type == #alias) { newSetting = make_shared(_Setting<t>)(UTIL::##alias##FromString(value)); }

namespace MYENGINE
{

	namespace UTIL
	{

		std::vector<Input> InputsFromString(std::string _string)
		{
			std::vector<std::string> inputsAsStrings = GetCellsFromString(_string, '*', true, false);
			std::vector<Input> result = {};
			for (size_t i = 0; i < inputsAsStrings.size(); i++)
			{
				std::vector<std::string> cells = GetCellsFromString(inputsAsStrings[i], ',', true, true);

				InputID inputID = -1;
				if (InputTypeFromString(cells.at(1)) == InputType::OnScreenButton)
				{
					inputID = OnScreenButton::Create(cells.at(9).c_str(), cells.at(10).c_str(), IntFromString(cells.at(11)), IntFromString(cells.at(12)), IntFromString(cells.at(13)), IntFromString(cells.at(14)), IntFromString(cells.at(15)), IntFromString(cells.at(16)), BoolFromString(cells.at(17)), FloatFromString(cells.at(18)));
				}
				else
				{
					inputID = InputIdFromString(cells.at(2));
				}

				result.push_back(Input(InputDeviceTypes::FromString(cells.at(0)), InputTypeFromString(cells.at(1)), inputID, glm::vec2(FloatFromString(cells.at(3)) * INPUT_VALUE_MAX, FloatFromString(cells.at(4)) * INPUT_VALUE_MAX), BoolFromString(cells.at(5)), FloatFromString(cells.at(6)), BoolFromString(cells.at(7)), FloatFromString(cells.at(8)) * INPUT_VALUE_MAX));
			}
			return result;
		}

	} // namespace UTIL


	SettingsManager::SettingsManager(weak(Manager) _manager)
		: SubManager(_manager)
	{

	}

	SettingsManager::~SettingsManager()
	{
		
	}

	bool SettingsManager::LoadFromFile(const char* _filePath, bool _contextBased, shared(InputManager) _inputManager)
	{
		std::ifstream file;
		file.open(_filePath);
		if (!file.is_open())
		{
			NOTE("Settings file '" << _filePath << "' failed to load.");
			return false;
		}

		try
		{
			std::string line;
			unsigned int lineNumber = 0;
			while (std::getline(file, line))
			{
				lineNumber++;
				if (!line.empty() && line[0] != '#') // ignore the line if it's empty or marked as a comment
				{
					bool error = false;
					std::vector<std::string> sections = UTIL::GetCellsFromString(line, '|', true, false);

					int expectedSections = _contextBased ? 4 : 3;

					if (sections.size() >= expectedSections)
					{
						std::string name = sections[0];
						std::string context = _contextBased ? sections[1] : "NA";
						std::string type = sections[_contextBased ? 2 : 1];
						std::string value = sections[_contextBased ? 3 : 2];

						if (!name.empty() && !context.empty() && !type.empty() && !value.empty())
						{
							//NOTE(type << " " << name << " = " << value << " (for context " << context << ")");

							shared(Setting) newSetting;

							if CHECK_SETTING_TYPE(int, Int)
							else if CHECK_SETTING_TYPE(float, Float)
							else if CHECK_SETTING_TYPE(bool, Bool)
							else if CHECK_SETTING_TYPE(glm::vec2, Vec2)
							else if CHECK_SETTING_TYPE(glm::vec3, Vec3)
							else if CHECK_SETTING_TYPE(glm::vec4, Vec4)
							else if CHECK_SETTING_TYPE(std::vector<Input>, Inputs)

							else
							{
								error = true;
							}

							if (!error)
							{
								std::string fullName = name;
								if (_contextBased)
								{
									fullName.append("__" + context);
								}

								if (!_inputManager)
								{
									m_settings[fullName] = newSetting;
								}
								else
								{
									// provide directly to InputManager
									std::vector<Input> inputs = {};
									newSetting->GetData(inputs);

									if (!_inputManager->AddInputsToConceptBinding(name, inputs))
									{
										error = true;
									}
								}
							}
						}
						else
						{
							error = true;
						}
					}
					else
					{
						error = true;
					}

					if (error)
					{
						WARN("Settings file '" << _filePath << "' contains an invalid setting on line " << lineNumber);
					}
				}
			}
		}
		catch (std::exception& e)
		{
			std::cout << "[Settings Manager] Standard namespace has thrown: " << e.what() << std::endl;
			std::cin.get();
			return false;
		}
		catch (...)
		{
			std::cout << "[Settings Manager] Unknown object was thrown, no additional information available." << std::endl;
			std::cin.get();
			return false;
		}

		file.close();

		NOTE("Settings file '" << _filePath << "' finished loading");
		return true;
	}


#define INPUT_ID_MAP_ENTRY(group, name) { #name, group::name }

	namespace UTIL
	{

		std::map<std::string, InputID> Maps::inputIdMap =
		{
			INPUT_ID_MAP_ENTRY(InputAxes, LEFTX),
			INPUT_ID_MAP_ENTRY(InputAxes, LEFTY),
			INPUT_ID_MAP_ENTRY(InputAxes, RIGHTX),
			INPUT_ID_MAP_ENTRY(InputAxes, RIGHTY),
			INPUT_ID_MAP_ENTRY(InputAxes, TRIGGERLEFT),
			INPUT_ID_MAP_ENTRY(InputAxes, TRIGGERRIGHT),
			INPUT_ID_MAP_ENTRY(InputAxes, MOUSEX),
			INPUT_ID_MAP_ENTRY(InputAxes, MOUSEY),
			INPUT_ID_MAP_ENTRY(InputAxes, LEFTX_NEG),
			INPUT_ID_MAP_ENTRY(InputAxes, LEFTY_NEG),
			INPUT_ID_MAP_ENTRY(InputAxes, RIGHTX_NEG),
			INPUT_ID_MAP_ENTRY(InputAxes, RIGHTY_NEG),

			INPUT_ID_MAP_ENTRY(ControllerButtons, UP),
			INPUT_ID_MAP_ENTRY(ControllerButtons, DOWN),
			INPUT_ID_MAP_ENTRY(ControllerButtons, LEFT),
			INPUT_ID_MAP_ENTRY(ControllerButtons, RIGHT),
			INPUT_ID_MAP_ENTRY(ControllerButtons, NORTH),
			INPUT_ID_MAP_ENTRY(ControllerButtons, EAST),
			INPUT_ID_MAP_ENTRY(ControllerButtons, SOUTH),
			INPUT_ID_MAP_ENTRY(ControllerButtons, WEST),
			INPUT_ID_MAP_ENTRY(ControllerButtons, LEFTSTICK),
			INPUT_ID_MAP_ENTRY(ControllerButtons, RIGHTSTICK),
			INPUT_ID_MAP_ENTRY(ControllerButtons, LEFTBUMPER),
			INPUT_ID_MAP_ENTRY(ControllerButtons, RIGHTBUMPER),
			INPUT_ID_MAP_ENTRY(ControllerButtons, MISCRIGHT),
			INPUT_ID_MAP_ENTRY(ControllerButtons, MISCMIDDLE),
			INPUT_ID_MAP_ENTRY(ControllerButtons, MISCLEFT),
			INPUT_ID_MAP_ENTRY(ControllerButtons, MISCOTHER ),
			INPUT_ID_MAP_ENTRY(ControllerButtons, TOUCHPAD),

			INPUT_ID_MAP_ENTRY(MouseButtons, LEFTCLICK),
			INPUT_ID_MAP_ENTRY(MouseButtons, MIDDLECLICK),
			INPUT_ID_MAP_ENTRY(MouseButtons, RIGHTCLICK),
			INPUT_ID_MAP_ENTRY(MouseButtons, MOUSE4),
			INPUT_ID_MAP_ENTRY(MouseButtons, MOUSE5),

			INPUT_ID_MAP_ENTRY(KeyboardKeys, UNKNOWN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, RETURN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, ESCAPE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, BACKSPACE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, TAB),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SPACE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, EXCLAIM),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, QUOTEDBL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, HASH),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, PERCENT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, DOLLAR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AMPERSAND),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, QUOTE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, LEFTPAREN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, RIGHTPAREN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, ASTERISK),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, PLUS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, COMMA),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, MINUS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, FULLSTOP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, PERIOD),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SLASH),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, ZERO),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, ONE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, TWO),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, THREE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, FOUR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, FIVE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SIX),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SEVEN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, EIGHT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, NINE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, COLON),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SEMICOLON),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, LESS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, EQUALS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, GREATER),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, QUESTION),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, LEFTBRACKET),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, BACKSLASH),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, RIGHTBRACKET),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CARET),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, UNDERSCORE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, BACKQUOTE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, a),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, b),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, c),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, d),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, e),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, f),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, g),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, h),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, i),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, j),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, k),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, l),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, m),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, n),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, o),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, p),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, q),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, r),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, s),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, t),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, u),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, v),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, w),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, x),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, y),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, z),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CAPSLOCK),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F1),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F2),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F3),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F4),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F5),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F6),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F7),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F8),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F9),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F10),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F11),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F12),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, PRINTSCREEN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SCROLLLOCK),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, PAUSE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, INSERT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, HOME),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, PAGEUP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, DELETE_),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, END),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, PAGEDOWN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, RIGHTARROW),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, LEFTARROW),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, DOWNARROW),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, UPARROW),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, NUMLOCKCLEAR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_DIVIDE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_MULTIPLY),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_MINUS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_PLUS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_ENTER),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_1),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_2),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_3),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_4),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_5),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_6),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_7),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_8),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_9),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_0),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_FULLSTOP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_PERIOD),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, APPLICATION),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, POWER),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_EQUALS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F13),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F14),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F15),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F16),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F17),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F18),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F19),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F20),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F21),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F22),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F23),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, F24),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, EXECUTE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, HELP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, MENU),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SELECT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, STOP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AGAIN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, UNDO),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CUT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, COPY),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, PASTE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, FIND),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, MUTE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, VOLUMEUP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, VOLUMEDOWN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_COMMA),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_EQUALSAS400),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, ALTERASE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SYSREQ),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CANCEL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CLEAR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, PRIOR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, RETURN2),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SEPARATOR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, OUT_),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, OPER),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CLEARAGAIN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CRSEL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, EXSEL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_00),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_000),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, THOUSANDSSEPARATOR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, DECIMALSEPARATOR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CURRENCYUNIT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CURRENCYSUBUNIT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_LEFTPAREN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_RIGHTPAREN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_LEFTBRACE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_RIGHTBRACE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_TAB),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_BACKSPACE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_A),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_B),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_C),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_D),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_E),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_F),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_XOR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_POWER),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_PERCENT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_LESS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_GREATER),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_AMPERSAND),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_DBLAMPERSAND),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_VERTICALBAR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_DBLVERTICALBAR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_COLON),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_HASH),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_SPACE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_AT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_EXCLAM),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_MEMSTORE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_MEMRECALL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_MEMCLEAR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_MEMADD),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_MEMSUBTRACT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_MEMMULTIPLY),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_MEMDIVIDE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_PLUSMINUS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_CLEAR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_CLEARENTRY),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_BINARY),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_OCTAL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_DECIMAL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KP_HEXADECIMAL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, LCTRL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, LSHIFT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, LALT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, LGUI),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, RCTRL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, RSHIFT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, RALT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, RGUI),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, MODE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AUDIONEXT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AUDIOPREV),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AUDIOSTOP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AUDIOPLAY),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AUDIOMUTE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, MEDIASELECT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, WWW),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, MAIL),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, CALCULATOR),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, COMPUTER),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AC_SEARCH),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AC_HOME),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AC_BACK),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AC_FORWARD),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AC_STOP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AC_REFRESH),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AC_BOOKMARKS),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, BRIGHTNESSDOWN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, BRIGHTNESSUP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, DISPLAYSWITCH),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KBDILLUMTOGGLE),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KBDILLUMDOWN),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, KBDILLUMUP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, EJECT),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, SLEEP),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, APP1),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, APP2),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AUDIOREWIND),
			INPUT_ID_MAP_ENTRY(KeyboardKeys, AUDIOFASTFORWARD),

		};

	} // namespace UTIL

} // namespace MYENGINE
