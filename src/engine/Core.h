#ifndef _MYENGINE_CORE_H_
#define _MYENGINE_CORE_H_

#include "engine/Defines.h"

struct SDL_Window;
typedef struct _SDL_GameController SDL_GameController;
typedef void* SDL_GLContext;
struct ALCdevice;
struct ALCcontext;

namespace MYENGINE
{
	struct Manager;
	struct InputManager;

	class Core
	{
	public:
		/**
		* Initialise MYENGINE and its subsystems.
		**/
		static void Init();

		/**
		* Run MYENGINE's core loop. This can only be done after calling Core::Init().
		* 
		* @returns a program exit code.
		**/
		static int Run();

		/**
		* Set the title for the active window.
		*
		* @param _newTitle : The new title to use.
		**/
		static void SetWindowTitle(const char* _newTitle);

		/**
		* Retrieve the Manager instance. Ensure that this reference is released ( .reset() ) before the Core::Run() call.
		* 
		* @returns the Manager instance used by MYENGINE.
		**/
		static shared(Manager) GetManager() { return m_manager; }

	private:
		static int RunInternal();
		static void DestroyAll();

		static bool m_initialised;

		static shared(Manager) m_manager;
		static shared(InputManager) m_inputManager;
		static SDL_Window* m_window;
		static SDL_GameController* m_gameController;
		static SDL_GLContext m_GLcontext;
		static ALCdevice* m_ALdevice;
		static ALCcontext* m_ALcontext;
	};

} // namespace MYENGINE

#endif
