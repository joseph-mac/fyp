#include "InputDefs.h"

#include "engine/Core.h"
#include "engine/managers/Manager.h"
#include "engine/managers/SettingsManager.h"
#include "engine/managers/InputManager.h"

#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_keycode.h>

#include <glm/vec2.hpp>

namespace MYENGINE
{

	// InputDeviceType defs
	const InputDeviceType InputDeviceTypes::Unknown = 1 << SDL_CONTROLLER_TYPE_UNKNOWN;

	const InputDeviceType InputDeviceTypes::XB360 = 1 << SDL_CONTROLLER_TYPE_XBOX360;
	const InputDeviceType InputDeviceTypes::XB1 = 1 << SDL_CONTROLLER_TYPE_XBOXONE;
	const InputDeviceType InputDeviceTypes::XBSX = 1 << SDL_CONTROLLER_TYPE_XBOXONE;

	const InputDeviceType InputDeviceTypes::PS3 = 1 << SDL_CONTROLLER_TYPE_PS3;
	const InputDeviceType InputDeviceTypes::PS4 = 1 << SDL_CONTROLLER_TYPE_PS4;
	const InputDeviceType InputDeviceTypes::PS5 = 1 << SDL_CONTROLLER_TYPE_PS5;

	const InputDeviceType InputDeviceTypes::NINSWpro = 1 << SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO;

	const InputDeviceType InputDeviceTypes::Virtual = 1 << SDL_CONTROLLER_TYPE_VIRTUAL;

	const InputDeviceType InputDeviceTypes::Keyboard = 1 << 8;
	const InputDeviceType InputDeviceTypes::Mouse = 1 << 9;

	const InputDeviceType InputDeviceTypes::LAST = 1 << 10;

	const InputDeviceType InputDeviceTypes::XB_all = XB360 | XB1 | XBSX;
	const InputDeviceType InputDeviceTypes::PS_all = PS3 | PS4 | PS5;
	const InputDeviceType InputDeviceTypes::NIN_all = NINSWpro;
	const InputDeviceType InputDeviceTypes::AllControllers = XB360 | XB1 | XBSX | PS3 | PS4 | PS5 | NINSWpro | Virtual;

	std::string InputDeviceTypes::ToString(InputDeviceType _type)
	{
		std::string result = "|";
		for (unsigned short i = 0; 1 << i < InputDeviceTypes::LAST; i++)
		{
			InputDeviceType tempType = (InputDeviceType)(1 << i);

			if ((int)(tempType & _type) > 0)
			{
				std::string append = "";
				switch (tempType)
				{
				case InputDeviceTypes::Unknown:
					append = " Unknown |";
					break;
				case InputDeviceTypes::XB360:
					append = " XBOX 360 |";
					break;
				case InputDeviceTypes::XB1:
					append = " XBOX ONE/SX |";
					break;
				case InputDeviceTypes::PS3:
					append = " PS3 |";
					break;
				case InputDeviceTypes::PS4:
					append = " PS4 |";
					break;
				case InputDeviceTypes::PS5:
					append = " PS5 |";
					break;

				case InputDeviceTypes::NINSWpro:
					append = " Nintendo Switch Pro |";
					break;

				case InputDeviceTypes::Virtual:
					append = " Virtual |";
					break;

				case InputDeviceTypes::Keyboard:
					append = " Keyboard |";
					break;
				case InputDeviceTypes::Mouse:
					append = " Mouse |";
					break;
				}
				result.append(append);
			}
		}
		return result;
	}

	std::map<std::string, InputDeviceType> inputDeviceTypesMap =
	{
		{ "Unknown", InputDeviceTypes::Unknown },

		{ "XB360", InputDeviceTypes::XB360 },
		{ "XB1", InputDeviceTypes::XB1 },
		{ "XBSX", InputDeviceTypes::XBSX },

		{ "PS3", InputDeviceTypes::PS3 },
		{ "PS4", InputDeviceTypes::PS4 },
		{ "PS5", InputDeviceTypes::PS5 },

		{ "NINSWpro", InputDeviceTypes::NINSWpro },

		{ "Virtual", InputDeviceTypes::Virtual },

		{ "Keyboard", InputDeviceTypes::Keyboard },
		{ "Mouse", InputDeviceTypes::Mouse },

		{ "XB_all", InputDeviceTypes::XB_all },
		{ "PS_all", InputDeviceTypes::PS_all },
		{ "NIN_all", InputDeviceTypes::NIN_all },
		{ "AllControllers", InputDeviceTypes::AllControllers }
	};

	InputDeviceType InputDeviceTypes::FromString(std::string _str)
	{
		return inputDeviceTypesMap.at(_str);
	}


	// KeyboardKey defs
	const InputID KeyboardKeys::UNKNOWN = SDLK_UNKNOWN;
	const InputID KeyboardKeys::RETURN = SDLK_RETURN;
	const InputID KeyboardKeys::ESCAPE = SDLK_ESCAPE;
	const InputID KeyboardKeys::BACKSPACE = SDLK_BACKSPACE;
	const InputID KeyboardKeys::TAB = SDLK_TAB;
	const InputID KeyboardKeys::SPACE = SDLK_SPACE;
	const InputID KeyboardKeys::EXCLAIM = SDLK_EXCLAIM;
	const InputID KeyboardKeys::QUOTEDBL = SDLK_QUOTEDBL;
	const InputID KeyboardKeys::HASH = SDLK_HASH;
	const InputID KeyboardKeys::PERCENT = SDLK_PERCENT;
	const InputID KeyboardKeys::DOLLAR = SDLK_DOLLAR;
	const InputID KeyboardKeys::AMPERSAND = SDLK_AMPERSAND;
	const InputID KeyboardKeys::QUOTE = SDLK_QUOTE;
	const InputID KeyboardKeys::LEFTPAREN = SDLK_LEFTPAREN;
	const InputID KeyboardKeys::RIGHTPAREN = SDLK_RIGHTPAREN;
	const InputID KeyboardKeys::ASTERISK = SDLK_ASTERISK;
	const InputID KeyboardKeys::PLUS = SDLK_PLUS;
	const InputID KeyboardKeys::COMMA = SDLK_COMMA;
	const InputID KeyboardKeys::MINUS = SDLK_MINUS;
	const InputID KeyboardKeys::FULLSTOP = SDLK_PERIOD;
	const InputID KeyboardKeys::PERIOD = KeyboardKeys::FULLSTOP;
	const InputID KeyboardKeys::SLASH = SDLK_SLASH;
	const InputID KeyboardKeys::ZERO = SDLK_0;
	const InputID KeyboardKeys::ONE = SDLK_1;
	const InputID KeyboardKeys::TWO = SDLK_2;
	const InputID KeyboardKeys::THREE = SDLK_3;
	const InputID KeyboardKeys::FOUR = SDLK_4;
	const InputID KeyboardKeys::FIVE = SDLK_5;
	const InputID KeyboardKeys::SIX = SDLK_6;
	const InputID KeyboardKeys::SEVEN = SDLK_7;
	const InputID KeyboardKeys::EIGHT = SDLK_8;
	const InputID KeyboardKeys::NINE = SDLK_9;
	const InputID KeyboardKeys::COLON = SDLK_COLON;
	const InputID KeyboardKeys::SEMICOLON = SDLK_SEMICOLON;
	const InputID KeyboardKeys::LESS = SDLK_LESS;
	const InputID KeyboardKeys::EQUALS = SDLK_EQUALS;
	const InputID KeyboardKeys::GREATER = SDLK_GREATER;
	const InputID KeyboardKeys::QUESTION = SDLK_QUESTION;
	const InputID KeyboardKeys::AT = SDLK_AT;
	const InputID KeyboardKeys::LEFTBRACKET = SDLK_LEFTBRACKET;
	const InputID KeyboardKeys::BACKSLASH = SDLK_BACKSLASH;
	const InputID KeyboardKeys::RIGHTBRACKET = SDLK_RIGHTBRACKET;
	const InputID KeyboardKeys::CARET = SDLK_CARET;
	const InputID KeyboardKeys::UNDERSCORE = SDLK_UNDERSCORE;
	const InputID KeyboardKeys::BACKQUOTE = SDLK_BACKQUOTE;
	const InputID KeyboardKeys::a = SDLK_a;
	const InputID KeyboardKeys::b = SDLK_b;
	const InputID KeyboardKeys::c = SDLK_c;
	const InputID KeyboardKeys::d = SDLK_d;
	const InputID KeyboardKeys::e = SDLK_e;
	const InputID KeyboardKeys::f = SDLK_f;
	const InputID KeyboardKeys::g = SDLK_g;
	const InputID KeyboardKeys::h = SDLK_h;
	const InputID KeyboardKeys::i = SDLK_i;
	const InputID KeyboardKeys::j = SDLK_j;
	const InputID KeyboardKeys::k = SDLK_k;
	const InputID KeyboardKeys::l = SDLK_l;
	const InputID KeyboardKeys::m = SDLK_m;
	const InputID KeyboardKeys::n = SDLK_n;
	const InputID KeyboardKeys::o = SDLK_o;
	const InputID KeyboardKeys::p = SDLK_p;
	const InputID KeyboardKeys::q = SDLK_q;
	const InputID KeyboardKeys::r = SDLK_r;
	const InputID KeyboardKeys::s = SDLK_s;
	const InputID KeyboardKeys::t = SDLK_t;
	const InputID KeyboardKeys::u = SDLK_u;
	const InputID KeyboardKeys::v = SDLK_v;
	const InputID KeyboardKeys::w = SDLK_w;
	const InputID KeyboardKeys::x = SDLK_x;
	const InputID KeyboardKeys::y = SDLK_y;
	const InputID KeyboardKeys::z = SDLK_z;
	const InputID KeyboardKeys::CAPSLOCK = SDLK_CAPSLOCK;
	const InputID KeyboardKeys::F1 = SDLK_F1;
	const InputID KeyboardKeys::F2 = SDLK_F2;
	const InputID KeyboardKeys::F3 = SDLK_F3;
	const InputID KeyboardKeys::F4 = SDLK_F4;
	const InputID KeyboardKeys::F5 = SDLK_F5;
	const InputID KeyboardKeys::F6 = SDLK_F6;
	const InputID KeyboardKeys::F7 = SDLK_F7;
	const InputID KeyboardKeys::F8 = SDLK_F8;
	const InputID KeyboardKeys::F9 = SDLK_F9;
	const InputID KeyboardKeys::F10 = SDLK_F10;
	const InputID KeyboardKeys::F11 = SDLK_F11;
	const InputID KeyboardKeys::F12 = SDLK_F12;
	const InputID KeyboardKeys::PRINTSCREEN = SDLK_PRINTSCREEN;
	const InputID KeyboardKeys::SCROLLLOCK = SDLK_SCROLLLOCK;
	const InputID KeyboardKeys::PAUSE = SDLK_PAUSE;
	const InputID KeyboardKeys::INSERT = SDLK_INSERT;
	const InputID KeyboardKeys::HOME = SDLK_HOME;
	const InputID KeyboardKeys::PAGEUP = SDLK_PAGEUP;
	const InputID KeyboardKeys::DELETE_ = SDLK_DELETE;
	const InputID KeyboardKeys::END = SDLK_END;
	const InputID KeyboardKeys::PAGEDOWN = SDLK_PAGEDOWN;
	const InputID KeyboardKeys::RIGHTARROW = SDLK_RIGHT;
	const InputID KeyboardKeys::LEFTARROW = SDLK_LEFT;
	const InputID KeyboardKeys::DOWNARROW = SDLK_DOWN;
	const InputID KeyboardKeys::UPARROW = SDLK_UP;
	const InputID KeyboardKeys::NUMLOCKCLEAR = SDLK_NUMLOCKCLEAR;
	const InputID KeyboardKeys::KP_DIVIDE = SDLK_KP_DIVIDE;
	const InputID KeyboardKeys::KP_MULTIPLY = SDLK_KP_MULTIPLY;
	const InputID KeyboardKeys::KP_MINUS = SDLK_KP_MINUS;
	const InputID KeyboardKeys::KP_PLUS = SDLK_KP_PLUS;
	const InputID KeyboardKeys::KP_ENTER = SDLK_KP_ENTER;
	const InputID KeyboardKeys::KP_1 = SDLK_KP_1;
	const InputID KeyboardKeys::KP_2 = SDLK_KP_2;
	const InputID KeyboardKeys::KP_3 = SDLK_KP_3;
	const InputID KeyboardKeys::KP_4 = SDLK_KP_4;
	const InputID KeyboardKeys::KP_5 = SDLK_KP_5;
	const InputID KeyboardKeys::KP_6 = SDLK_KP_6;
	const InputID KeyboardKeys::KP_7 = SDLK_KP_7;
	const InputID KeyboardKeys::KP_8 = SDLK_KP_8;
	const InputID KeyboardKeys::KP_9 = SDLK_KP_9;
	const InputID KeyboardKeys::KP_0 = SDLK_KP_0;
	const InputID KeyboardKeys::KP_FULLSTOP = SDLK_KP_PERIOD;
	const InputID KeyboardKeys::KP_PERIOD = KeyboardKeys::KP_FULLSTOP;
	const InputID KeyboardKeys::APPLICATION = SDLK_APPLICATION;
	const InputID KeyboardKeys::POWER = SDLK_POWER;
	const InputID KeyboardKeys::KP_EQUALS = SDLK_KP_EQUALS;
	const InputID KeyboardKeys::F13 = SDLK_F13;
	const InputID KeyboardKeys::F14 = SDLK_F14;
	const InputID KeyboardKeys::F15 = SDLK_F15;
	const InputID KeyboardKeys::F16 = SDLK_F16;
	const InputID KeyboardKeys::F17 = SDLK_F17;
	const InputID KeyboardKeys::F18 = SDLK_F18;
	const InputID KeyboardKeys::F19 = SDLK_F19;
	const InputID KeyboardKeys::F20 = SDLK_F20;
	const InputID KeyboardKeys::F21 = SDLK_F21;
	const InputID KeyboardKeys::F22 = SDLK_F22;
	const InputID KeyboardKeys::F23 = SDLK_F23;
	const InputID KeyboardKeys::F24 = SDLK_F24;
	const InputID KeyboardKeys::EXECUTE = SDLK_EXECUTE;
	const InputID KeyboardKeys::HELP = SDLK_HELP;
	const InputID KeyboardKeys::MENU = SDLK_MENU;
	const InputID KeyboardKeys::SELECT = SDLK_SELECT;
	const InputID KeyboardKeys::STOP = SDLK_STOP;
	const InputID KeyboardKeys::AGAIN = SDLK_AGAIN;
	const InputID KeyboardKeys::UNDO = SDLK_UNDO;
	const InputID KeyboardKeys::CUT = SDLK_CUT;
	const InputID KeyboardKeys::COPY = SDLK_COPY;
	const InputID KeyboardKeys::PASTE = SDLK_PASTE;
	const InputID KeyboardKeys::FIND = SDLK_FIND;
	const InputID KeyboardKeys::MUTE = SDLK_MUTE;
	const InputID KeyboardKeys::VOLUMEUP = SDLK_VOLUMEUP;
	const InputID KeyboardKeys::VOLUMEDOWN = SDLK_VOLUMEDOWN;
	const InputID KeyboardKeys::KP_COMMA = SDLK_KP_COMMA;
	const InputID KeyboardKeys::KP_EQUALSAS400 = SDLK_KP_EQUALSAS400;
	const InputID KeyboardKeys::ALTERASE = SDLK_ALTERASE;
	const InputID KeyboardKeys::SYSREQ = SDLK_SYSREQ;
	const InputID KeyboardKeys::CANCEL = SDLK_CANCEL;
	const InputID KeyboardKeys::CLEAR = SDLK_CLEAR;
	const InputID KeyboardKeys::PRIOR = SDLK_PRIOR;
	const InputID KeyboardKeys::RETURN2 = SDLK_RETURN2;
	const InputID KeyboardKeys::SEPARATOR = SDLK_SEPARATOR;
	const InputID KeyboardKeys::OUT_ = SDLK_OUT;
	const InputID KeyboardKeys::OPER = SDLK_OPER;
	const InputID KeyboardKeys::CLEARAGAIN = SDLK_CLEARAGAIN;
	const InputID KeyboardKeys::CRSEL = SDLK_CRSEL;
	const InputID KeyboardKeys::EXSEL = SDLK_EXSEL;
	const InputID KeyboardKeys::KP_00 = SDLK_KP_00;
	const InputID KeyboardKeys::KP_000 = SDLK_KP_000;
	const InputID KeyboardKeys::THOUSANDSSEPARATOR = SDLK_THOUSANDSSEPARATOR;
	const InputID KeyboardKeys::DECIMALSEPARATOR = SDLK_DECIMALSEPARATOR;
	const InputID KeyboardKeys::CURRENCYUNIT = SDLK_CURRENCYUNIT;
	const InputID KeyboardKeys::CURRENCYSUBUNIT = SDLK_CURRENCYSUBUNIT;
	const InputID KeyboardKeys::KP_LEFTPAREN = SDLK_KP_LEFTPAREN;
	const InputID KeyboardKeys::KP_RIGHTPAREN = SDLK_KP_RIGHTPAREN;
	const InputID KeyboardKeys::KP_LEFTBRACE = SDLK_KP_LEFTBRACE;
	const InputID KeyboardKeys::KP_RIGHTBRACE = SDLK_KP_RIGHTBRACE;
	const InputID KeyboardKeys::KP_TAB = SDLK_KP_TAB;
	const InputID KeyboardKeys::KP_BACKSPACE = SDLK_KP_BACKSPACE;
	const InputID KeyboardKeys::KP_A = SDLK_KP_A;
	const InputID KeyboardKeys::KP_B = SDLK_KP_B;
	const InputID KeyboardKeys::KP_C = SDLK_KP_C;
	const InputID KeyboardKeys::KP_D = SDLK_KP_D;
	const InputID KeyboardKeys::KP_E = SDLK_KP_E;
	const InputID KeyboardKeys::KP_F = SDLK_KP_F;
	const InputID KeyboardKeys::KP_XOR = SDLK_KP_XOR;
	const InputID KeyboardKeys::KP_POWER = SDLK_KP_POWER;
	const InputID KeyboardKeys::KP_PERCENT = SDLK_KP_PERCENT;
	const InputID KeyboardKeys::KP_LESS = SDLK_KP_LESS;
	const InputID KeyboardKeys::KP_GREATER = SDLK_KP_GREATER;
	const InputID KeyboardKeys::KP_AMPERSAND = SDLK_KP_AMPERSAND;
	const InputID KeyboardKeys::KP_DBLAMPERSAND = SDLK_KP_DBLAMPERSAND;
	const InputID KeyboardKeys::KP_VERTICALBAR = SDLK_KP_VERTICALBAR;
	const InputID KeyboardKeys::KP_DBLVERTICALBAR = SDLK_KP_DBLVERTICALBAR;
	const InputID KeyboardKeys::KP_COLON = SDLK_KP_COLON;
	const InputID KeyboardKeys::KP_HASH = SDLK_KP_HASH;
	const InputID KeyboardKeys::KP_SPACE = SDLK_KP_SPACE;
	const InputID KeyboardKeys::KP_AT = SDLK_KP_AT;
	const InputID KeyboardKeys::KP_EXCLAM = SDLK_KP_EXCLAM;
	const InputID KeyboardKeys::KP_MEMSTORE = SDLK_KP_MEMSTORE;
	const InputID KeyboardKeys::KP_MEMRECALL = SDLK_KP_MEMRECALL;
	const InputID KeyboardKeys::KP_MEMCLEAR = SDLK_KP_MEMCLEAR;
	const InputID KeyboardKeys::KP_MEMADD = SDLK_KP_MEMADD;
	const InputID KeyboardKeys::KP_MEMSUBTRACT = SDLK_KP_MEMSUBTRACT;
	const InputID KeyboardKeys::KP_MEMMULTIPLY = SDLK_KP_MEMMULTIPLY;
	const InputID KeyboardKeys::KP_MEMDIVIDE = SDLK_KP_MEMDIVIDE;
	const InputID KeyboardKeys::KP_PLUSMINUS = SDLK_KP_PLUSMINUS;
	const InputID KeyboardKeys::KP_CLEAR = SDLK_KP_CLEAR;
	const InputID KeyboardKeys::KP_CLEARENTRY = SDLK_KP_CLEARENTRY;
	const InputID KeyboardKeys::KP_BINARY = SDLK_KP_BINARY;
	const InputID KeyboardKeys::KP_OCTAL = SDLK_KP_OCTAL;
	const InputID KeyboardKeys::KP_DECIMAL = SDLK_KP_DECIMAL;
	const InputID KeyboardKeys::KP_HEXADECIMAL = SDLK_KP_HEXADECIMAL;
	const InputID KeyboardKeys::LCTRL = SDLK_LCTRL;
	const InputID KeyboardKeys::LSHIFT = SDLK_LSHIFT;
	const InputID KeyboardKeys::LALT = SDLK_LALT;
	const InputID KeyboardKeys::LGUI = SDLK_LGUI;
	const InputID KeyboardKeys::RCTRL = SDLK_RCTRL;
	const InputID KeyboardKeys::RSHIFT = SDLK_RSHIFT;
	const InputID KeyboardKeys::RALT = SDLK_RALT;
	const InputID KeyboardKeys::RGUI = SDLK_RGUI;
	const InputID KeyboardKeys::MODE = SDLK_MODE;
	const InputID KeyboardKeys::AUDIONEXT = SDLK_AUDIONEXT;
	const InputID KeyboardKeys::AUDIOPREV = SDLK_AUDIOPREV;
	const InputID KeyboardKeys::AUDIOSTOP = SDLK_AUDIOSTOP;
	const InputID KeyboardKeys::AUDIOPLAY = SDLK_AUDIOPLAY;
	const InputID KeyboardKeys::AUDIOMUTE = SDLK_AUDIOMUTE;
	const InputID KeyboardKeys::MEDIASELECT = SDLK_MEDIASELECT;
	const InputID KeyboardKeys::WWW = SDLK_WWW;
	const InputID KeyboardKeys::MAIL = SDLK_MAIL;
	const InputID KeyboardKeys::CALCULATOR = SDLK_CALCULATOR;
	const InputID KeyboardKeys::COMPUTER = SDLK_COMPUTER;
	const InputID KeyboardKeys::AC_SEARCH = SDLK_AC_SEARCH;
	const InputID KeyboardKeys::AC_HOME = SDLK_AC_HOME;
	const InputID KeyboardKeys::AC_BACK = SDLK_AC_BACK;
	const InputID KeyboardKeys::AC_FORWARD = SDLK_AC_FORWARD;
	const InputID KeyboardKeys::AC_STOP = SDLK_AC_STOP;
	const InputID KeyboardKeys::AC_REFRESH = SDLK_AC_REFRESH;
	const InputID KeyboardKeys::AC_BOOKMARKS = SDLK_AC_BOOKMARKS;
	const InputID KeyboardKeys::BRIGHTNESSDOWN = SDLK_BRIGHTNESSDOWN;
	const InputID KeyboardKeys::BRIGHTNESSUP = SDLK_BRIGHTNESSUP;
	const InputID KeyboardKeys::DISPLAYSWITCH = SDLK_DISPLAYSWITCH;
	const InputID KeyboardKeys::KBDILLUMTOGGLE = SDLK_KBDILLUMTOGGLE;
	const InputID KeyboardKeys::KBDILLUMDOWN = SDLK_KBDILLUMDOWN;
	const InputID KeyboardKeys::KBDILLUMUP = SDLK_KBDILLUMUP;
	const InputID KeyboardKeys::EJECT = SDLK_EJECT;
	const InputID KeyboardKeys::SLEEP = SDLK_SLEEP;
	const InputID KeyboardKeys::APP1 = SDLK_APP1;
	const InputID KeyboardKeys::APP2 = SDLK_APP2;
	const InputID KeyboardKeys::AUDIOREWIND = SDLK_AUDIOREWIND;
	const InputID KeyboardKeys::AUDIOFASTFORWARD = SDLK_AUDIOFASTFORWARD;


	// MouseButton defs
	const InputID MouseButtons::LEFTCLICK = SDL_BUTTON_LEFT;
	const InputID MouseButtons::MIDDLECLICK = SDL_BUTTON_MIDDLE;
	const InputID MouseButtons::RIGHTCLICK = SDL_BUTTON_RIGHT;
	const InputID MouseButtons::MOUSE4 = SDL_BUTTON_X1;
	const InputID MouseButtons::MOUSE5 = SDL_BUTTON_X2;


	// ControllerButton defs
	const InputID ControllerButtons::UP = SDL_CONTROLLER_BUTTON_DPAD_UP;
	const InputID ControllerButtons::DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	const InputID ControllerButtons::LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	const InputID ControllerButtons::RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	const InputID ControllerButtons::NORTH = SDL_CONTROLLER_BUTTON_Y;
	const InputID ControllerButtons::EAST = SDL_CONTROLLER_BUTTON_B;
	const InputID ControllerButtons::SOUTH = SDL_CONTROLLER_BUTTON_A;
	const InputID ControllerButtons::WEST = SDL_CONTROLLER_BUTTON_X;

	const InputID ControllerButtons::LEFTSTICK = SDL_CONTROLLER_BUTTON_LEFTSTICK;
	const InputID ControllerButtons::RIGHTSTICK = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
	const InputID ControllerButtons::LEFTBUMPER = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	const InputID ControllerButtons::RIGHTBUMPER = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	const InputID ControllerButtons::MISCRIGHT = SDL_CONTROLLER_BUTTON_START;
	const InputID ControllerButtons::MISCMIDDLE = SDL_CONTROLLER_BUTTON_GUIDE;
	const InputID ControllerButtons::MISCLEFT = SDL_CONTROLLER_BUTTON_BACK;
	const InputID ControllerButtons::MISCOTHER = SDL_CONTROLLER_BUTTON_MISC1;
	const InputID ControllerButtons::TOUCHPAD = SDL_CONTROLLER_BUTTON_TOUCHPAD;


	// Axis defs
	const InputID InputAxes::LEFTX = SDL_CONTROLLER_AXIS_LEFTX;
	const InputID InputAxes::LEFTY = SDL_CONTROLLER_AXIS_LEFTY;
	const InputID InputAxes::RIGHTX = SDL_CONTROLLER_AXIS_RIGHTX;
	const InputID InputAxes::RIGHTY = SDL_CONTROLLER_AXIS_RIGHTY;
	const InputID InputAxes::TRIGGERLEFT = SDL_CONTROLLER_AXIS_TRIGGERLEFT;
	const InputID InputAxes::TRIGGERRIGHT = SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
	const InputID InputAxes::MOUSEX = InputAxes::TRIGGERRIGHT + 1;
	const InputID InputAxes::MOUSEY = InputAxes::MOUSEX + 1;

	const InputID InputAxes::NEG = InputAxes::MOUSEY + 1;
	const InputID InputAxes::LEFTX_NEG = LEFTX + NEG;
	const InputID InputAxes::LEFTY_NEG = LEFTY + NEG;
	const InputID InputAxes::RIGHTX_NEG = RIGHTX + NEG;
	const InputID InputAxes::RIGHTY_NEG = RIGHTY + NEG;


	InputID OnScreenButton::lastID = -1;
	InputID OnScreenButton::Create(const char* _name, const char* _iconPath, int _xPos, int _yPos, int _w, int _h, int _xAnchor, int _yAnchor, bool _treatHoverAsActivation, float _hoverLingerTime)
	{
		lastID++;
		Core::GetManager()->GetInputManager()->CreateNewOnScreenButton(lastID, _name, _iconPath, glm::ivec2(_xPos, _yPos), glm::ivec2(_w, _h), glm::ivec2(_xAnchor, _yAnchor), _treatHoverAsActivation, _hoverLingerTime);
		return lastID;
	}


	std::map<std::string, InputType> inputTypesMap =
	{
		{ "KeyboardKey", InputType::KeyboardKey },
		{ "MouseButton", InputType::MouseButton },
		{ "ControllerButton", InputType::ControllerButton },
		{ "Axis", InputType::Axis },
		{ "ControllerTrigger", InputType::ControllerTrigger },
		{ "OnScreenButton", InputType::OnScreenButton }
	};
	InputType InputTypeFromString(std::string _str)
	{
		return inputTypesMap.at(_str);
	}


	InputID InputIdFromString(std::string _str)
	{
		return UTIL::Maps::inputIdMap.at(_str);
	}

} // namespace MYENGINE
