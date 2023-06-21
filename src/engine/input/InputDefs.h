#ifndef _MYENGINE_INPUTDEFS_H_
#define _MYENGINE_INPUTDEFS_H_

#include <string>

// This file creates an 'easy' way to avoid the engine being too tied to SDL.

namespace MYENGINE
{

	// Constants

	/*
	* Maximum value for inputs
	*/
	static const short INPUT_VALUE_MAX = 32767;
	/*
	* Minimum value for raw inputs
	*/
	static const short INPUT_VALUE_MIN = -32768;

	/*
	* Inner deadzone for a toggle-type input to activate
	*/
	static const short INPUT_TOGGLEMODE_INNER_DEADZONE = 3000;

	/*
	* Invalid instance ID representation.
	*/
	static const int INPUT_DEVICE_INSTANCE_INVALID = -1;
	/*
	* There can only be one mouse represented at a time, so it has a constant instance ID.
	*/
	static const int INPUT_DEVICE_INSTANCE_MOUSE = -2;
	/*
	* There can only be one keyboard represented at a time, so it has a constant instance ID.
	*/
	static const int INPUT_DEVICE_INSTANCE_KEYBOARD = -3;
	/*
	* There can only be one PC touchpad represented at a time, so it has a constant instance ID.
	*/
	static const int INPUT_DEVICE_INSTANCE_TOUCHPAD = -4;


	/**
	* Representation of an input device type, or a collection of
	**/
	typedef unsigned short InputDeviceType;
	/**
	* A collection of recognised input device types
	**/
	struct InputDeviceTypes
	{
		static const InputDeviceType Unknown;

		static const InputDeviceType XB360;
		static const InputDeviceType XB1;
		static const InputDeviceType XBSX;

		static const InputDeviceType PS3;
		static const InputDeviceType PS4;
		static const InputDeviceType PS5;

		static const InputDeviceType NINSWpro;

		static const InputDeviceType Virtual;

		static const InputDeviceType Keyboard;
		static const InputDeviceType Mouse;

		static const InputDeviceType LAST;

		static const InputDeviceType XB_all;
		static const InputDeviceType PS_all;
		static const InputDeviceType NIN_all;
		static const InputDeviceType AllControllers;

		static std::string ToString(InputDeviceType _type);
		static InputDeviceType FromString(std::string _str);
	};


	/**
	* Representation of an input
	**/
	typedef int InputID;

	InputID InputIdFromString(std::string _str);


	/**
	* A collection of recognised keyboard keys
	**/
	struct KeyboardKeys
	{
		static const InputID UNKNOWN;
		static const InputID RETURN;
		static const InputID ESCAPE;
		static const InputID BACKSPACE;
		static const InputID TAB;
		static const InputID SPACE;
		static const InputID EXCLAIM;
		static const InputID QUOTEDBL;
		static const InputID HASH;
		static const InputID PERCENT;
		static const InputID DOLLAR;
		static const InputID AMPERSAND;
		static const InputID QUOTE;
		static const InputID LEFTPAREN;
		static const InputID RIGHTPAREN;
		static const InputID ASTERISK;
		static const InputID PLUS;
		static const InputID COMMA;
		static const InputID MINUS;
		static const InputID FULLSTOP;
		static const InputID PERIOD;
		static const InputID SLASH;
		static const InputID ZERO;
		static const InputID ONE;
		static const InputID TWO;
		static const InputID THREE;
		static const InputID FOUR;
		static const InputID FIVE;
		static const InputID SIX;
		static const InputID SEVEN;
		static const InputID EIGHT;
		static const InputID NINE;
		static const InputID COLON;
		static const InputID SEMICOLON;
		static const InputID LESS;
		static const InputID EQUALS;
		static const InputID GREATER;
		static const InputID QUESTION;
		static const InputID AT;
		static const InputID LEFTBRACKET;
		static const InputID BACKSLASH;
		static const InputID RIGHTBRACKET;
		static const InputID CARET;
		static const InputID UNDERSCORE;
		static const InputID BACKQUOTE;
		static const InputID a;
		static const InputID b;
		static const InputID c;
		static const InputID d;
		static const InputID e;
		static const InputID f;
		static const InputID g;
		static const InputID h;
		static const InputID i;
		static const InputID j;
		static const InputID k;
		static const InputID l;
		static const InputID m;
		static const InputID n;
		static const InputID o;
		static const InputID p;
		static const InputID q;
		static const InputID r;
		static const InputID s;
		static const InputID t;
		static const InputID u;
		static const InputID v;
		static const InputID w;
		static const InputID x;
		static const InputID y;
		static const InputID z;
		static const InputID CAPSLOCK;
		static const InputID F1;
		static const InputID F2;
		static const InputID F3;
		static const InputID F4;
		static const InputID F5;
		static const InputID F6;
		static const InputID F7;
		static const InputID F8;
		static const InputID F9;
		static const InputID F10;
		static const InputID F11;
		static const InputID F12;
		static const InputID PRINTSCREEN;
		static const InputID SCROLLLOCK;
		static const InputID PAUSE;
		static const InputID INSERT;
		static const InputID HOME;
		static const InputID PAGEUP;
		static const InputID DELETE_;
		static const InputID END;
		static const InputID PAGEDOWN;
		static const InputID RIGHTARROW;
		static const InputID LEFTARROW;
		static const InputID DOWNARROW;
		static const InputID UPARROW;
		static const InputID NUMLOCKCLEAR;
		static const InputID KP_DIVIDE;
		static const InputID KP_MULTIPLY;
		static const InputID KP_MINUS;
		static const InputID KP_PLUS;
		static const InputID KP_ENTER;
		static const InputID KP_1;
		static const InputID KP_2;
		static const InputID KP_3;
		static const InputID KP_4;
		static const InputID KP_5;
		static const InputID KP_6;
		static const InputID KP_7;
		static const InputID KP_8;
		static const InputID KP_9;
		static const InputID KP_0;
		static const InputID KP_FULLSTOP;
		static const InputID KP_PERIOD;
		static const InputID APPLICATION;
		static const InputID POWER;
		static const InputID KP_EQUALS;
		static const InputID F13;
		static const InputID F14;
		static const InputID F15;
		static const InputID F16;
		static const InputID F17;
		static const InputID F18;
		static const InputID F19;
		static const InputID F20;
		static const InputID F21;
		static const InputID F22;
		static const InputID F23;
		static const InputID F24;
		static const InputID EXECUTE;
		static const InputID HELP;
		static const InputID MENU;
		static const InputID SELECT;
		static const InputID STOP;
		static const InputID AGAIN;
		static const InputID UNDO;
		static const InputID CUT;
		static const InputID COPY;
		static const InputID PASTE;
		static const InputID FIND;
		static const InputID MUTE;
		static const InputID VOLUMEUP;
		static const InputID VOLUMEDOWN;
		static const InputID KP_COMMA;
		static const InputID KP_EQUALSAS400;
		static const InputID ALTERASE;
		static const InputID SYSREQ;
		static const InputID CANCEL;
		static const InputID CLEAR;
		static const InputID PRIOR;
		static const InputID RETURN2;
		static const InputID SEPARATOR;
		static const InputID OUT_;
		static const InputID OPER;
		static const InputID CLEARAGAIN;
		static const InputID CRSEL;
		static const InputID EXSEL;
		static const InputID KP_00;
		static const InputID KP_000;
		static const InputID THOUSANDSSEPARATOR;
		static const InputID DECIMALSEPARATOR;
		static const InputID CURRENCYUNIT;
		static const InputID CURRENCYSUBUNIT;
		static const InputID KP_LEFTPAREN;
		static const InputID KP_RIGHTPAREN;
		static const InputID KP_LEFTBRACE;
		static const InputID KP_RIGHTBRACE;
		static const InputID KP_TAB;
		static const InputID KP_BACKSPACE;
		static const InputID KP_A;
		static const InputID KP_B;
		static const InputID KP_C;
		static const InputID KP_D;
		static const InputID KP_E;
		static const InputID KP_F;
		static const InputID KP_XOR;
		static const InputID KP_POWER;
		static const InputID KP_PERCENT;
		static const InputID KP_LESS;
		static const InputID KP_GREATER;
		static const InputID KP_AMPERSAND;
		static const InputID KP_DBLAMPERSAND;
		static const InputID KP_VERTICALBAR;
		static const InputID KP_DBLVERTICALBAR;
		static const InputID KP_COLON;
		static const InputID KP_HASH;
		static const InputID KP_SPACE;
		static const InputID KP_AT;
		static const InputID KP_EXCLAM;
		static const InputID KP_MEMSTORE;
		static const InputID KP_MEMRECALL;
		static const InputID KP_MEMCLEAR;
		static const InputID KP_MEMADD;
		static const InputID KP_MEMSUBTRACT;
		static const InputID KP_MEMMULTIPLY;
		static const InputID KP_MEMDIVIDE;
		static const InputID KP_PLUSMINUS;
		static const InputID KP_CLEAR;
		static const InputID KP_CLEARENTRY;
		static const InputID KP_BINARY;
		static const InputID KP_OCTAL;
		static const InputID KP_DECIMAL;
		static const InputID KP_HEXADECIMAL;
		static const InputID LCTRL;
		static const InputID LSHIFT;
		static const InputID LALT;
		static const InputID LGUI;
		static const InputID RCTRL;
		static const InputID RSHIFT;
		static const InputID RALT;
		static const InputID RGUI;
		static const InputID MODE;
		static const InputID AUDIONEXT;
		static const InputID AUDIOPREV;
		static const InputID AUDIOSTOP;
		static const InputID AUDIOPLAY;
		static const InputID AUDIOMUTE;
		static const InputID MEDIASELECT;
		static const InputID WWW;
		static const InputID MAIL;
		static const InputID CALCULATOR;
		static const InputID COMPUTER;
		static const InputID AC_SEARCH;
		static const InputID AC_HOME;
		static const InputID AC_BACK;
		static const InputID AC_FORWARD;
		static const InputID AC_STOP;
		static const InputID AC_REFRESH;
		static const InputID AC_BOOKMARKS;
		static const InputID BRIGHTNESSDOWN;
		static const InputID BRIGHTNESSUP;
		static const InputID DISPLAYSWITCH;
		static const InputID KBDILLUMTOGGLE;
		static const InputID KBDILLUMDOWN;
		static const InputID KBDILLUMUP;
		static const InputID EJECT;
		static const InputID SLEEP;
		static const InputID APP1;
		static const InputID APP2;
		static const InputID AUDIOREWIND;
		static const InputID AUDIOFASTFORWARD;
	};


	/**
	* A collection of recognised mouse buttons
	**/
	struct MouseButtons
	{
		static const InputID LEFTCLICK;
		static const InputID MIDDLECLICK;
		static const InputID RIGHTCLICK;
		static const InputID MOUSE4;
		static const InputID MOUSE5;
	};


	/**
	* A collection of recognised game controller buttons
	**/
	struct ControllerButtons
	{
		static const InputID UP;
		static const InputID DOWN;
		static const InputID LEFT;
		static const InputID RIGHT;

		static const InputID NORTH;
		static const InputID EAST;
		static const InputID SOUTH;
		static const InputID WEST;

		static const InputID LEFTSTICK;
		static const InputID RIGHTSTICK;

		static const InputID LEFTBUMPER;
		static const InputID RIGHTBUMPER;

		static const InputID MISCRIGHT;
		static const InputID MISCMIDDLE;
		static const InputID MISCLEFT;
		static const InputID MISCOTHER; /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button */

		static const InputID TOUCHPAD;
	};


	/**
	* A collection of recognised game controller axes. This is an expanded version of SDL_GameInputAxis that can handle the separation of axes into positive and negative.
	**/
	struct InputAxes
	{
		static const InputID LEFTX;
		static const InputID LEFTY;
		static const InputID RIGHTX;
		static const InputID RIGHTY;
		static const InputID TRIGGERLEFT;
		static const InputID TRIGGERRIGHT;
		static const InputID MOUSEX;
		static const InputID MOUSEY;

		// This value represents the first negative axis, and can be added to any non-negative axis to get its negative counterpart. NOTE: This does not apply to triggers or mouse axes, which are all axes with no negative representation.
		static const InputID NEG;
		static const InputID LEFTX_NEG;
		static const InputID LEFTY_NEG;
		static const InputID RIGHTX_NEG;
		static const InputID RIGHTY_NEG;
	};


	/**
	* 
	**/
	struct OnScreenButton
	{
		static InputID Create(const char* _name, const char* _iconPath, int _xPos, int _yPos, int _w, int _h, int _xAnchor, int _yAnchor, bool _treatHoverAsActivation, float _hoverLingerTime = 0.0f);

	private:
		static InputID lastID;
	};


	/**
	* An enum class of recognised input types
	**/
	enum class InputType
	{
		KeyboardKey,
		MouseButton,
		ControllerButton,
		Axis,
		ControllerTrigger,
		OnScreenButton
	};
	InputType InputTypeFromString(std::string _str);


	/**
	* An enum class of range conversion modes, for use with the input-concept system.
	**/
	enum class InputRangeConversionMode
	{
		none,
		scale_to_binary_nonZeroIsMax,
		scale_to_binary_25pcIsMax,
		scale_to_binary_50pcIsMax,
		scale_to_binary_75pcIsMax,
		scale_to_binary_100pcIsMax
	};

} // namespace MYENGINE

#endif
