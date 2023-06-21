#include "Core.h"

#include "engine/Exception.h"
#include "engine/managers/Manager.h"
#include "engine/managers/SettingsManager.h"
#include "engine/managers/InputManager.h"
#include "engine/managers/SceneLoader.h"
#include "engine/components/network/NetworkComponents.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <vector>

namespace MYENGINE
{

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720

	//const GLfloat rgb[] = {
	//		1.0f, 0.0f, 0.0f, 1.0f, //red
	//		0.0f, 1.0f, 0.0f, 1.0f, //green
	//		0.0f, 0.0f, 1.0f, 1.0f  //blue
	//};
	//
	//const GLfloat purple[] = {
	//		1.0f, 1.0f, 1.0f, 1.0f,
	//		1.0f, 0.0f, 0.0f, 1.0f, //red
	//		0.0f, 0.0f, 1.0f, 1.0f  //blue
	//};
	//
	//const GLfloat tropical[] = {
	//		1.0f, 0.5f, 0.0f, 1.0f, //orange
	//		0.0f, 1.0f, 0.0f, 1.0f, //green
	//		0.5f, 0.5f, 1.0f, 1.0f  //light blue
	//};

	bool Core::m_initialised = false;

	shared(Manager) Core::m_manager;
	shared(InputManager) Core::m_inputManager;
	SDL_Window* Core::m_window;
	SDL_GLContext Core::m_GLcontext;
	ALCdevice* Core::m_ALdevice;
	ALCcontext* Core::m_ALcontext;

	void Core::Init()
	{
		// This allows for a debugger to be attached, if needed
		std::cout << "MYENGINE is ready to init, press enter to continue..." << std::endl;
		std::cin.get();

		//------------------------------------SETUP: SDL, OPENGL, GLEW, OPENAL------------------------------------//

		SDL_Init(SDL_INIT_GAMECONTROLLER);

		// SDL window setup
		m_window = SDL_CreateWindow("Game Window",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			/*SDL_WINDOW_FULLSCREEN_DESKTOP |*/ SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (!m_window)
		{
			throw Exception("SDL: WINDOW CREATION FAILED");
		}

		// GL context setup
		m_GLcontext = SDL_GL_CreateContext(m_window);
		if (!m_GLcontext)
		{
			throw Exception("SDL: GL CONTEXT CREATION FAILED");
		}

		// Glew setup
		if (glewInit() != GLEW_OK)
		{
			throw Exception("GLEW: INITIALISATION FAILED");
		}

		// glEnables
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);

		// OpenAL setup
		m_ALdevice = alcOpenDevice(NULL);
		if (!m_ALdevice)
		{
			throw Exception("OPENAL: DEVICE INITIALISATION FAILED");
		}
		m_ALcontext = alcCreateContext(m_ALdevice, NULL);
		if (!m_ALcontext)
		{
			alcCloseDevice(m_ALdevice);
			throw Exception("OPENAL: CONTEXT INITIALISATION FAILED");
		}
		if (!alcMakeContextCurrent(m_ALcontext))
		{
			alcDestroyContext(m_ALcontext);
			alcCloseDevice(m_ALdevice);
			throw Exception("OPENAL: CURRENT CONTEXT SET FAILED");
		}
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);


		//------------------------------------SETUP: MYENGINE------------------------------------//

		// init manager
		m_manager = make_shared(Manager)();
		m_manager->Initialise(m_manager);
		m_inputManager = m_manager->GetInputManager();
		SceneLoader::m_manager = m_manager;

		m_initialised = true;
	}

	void Core::SetWindowTitle(const char* _newTitle)
	{
		SDL_SetWindowTitle(m_window, _newTitle);
	}

	int Core::Run()
	{
		try
		{
			return RunInternal();
		}
		catch (Exception& e)
		{
			std::cout << "MYENGINE has thrown: " << e.what() << std::endl;
			std::cin.get();
			return EXIT_FAILURE;
		}
		catch (std::exception& e)
		{
			std::cout << "Standard namespace has thrown: " << e.what() << std::endl;
			std::cin.get();
			return EXIT_FAILURE;
		}
		catch (...)
		{
			std::cout << "Unknown object was thrown, no additional information available." << std::endl;
			std::cin.get();
			return EXIT_FAILURE;
		}
	}

	

	int Core::RunInternal()
	{
		if (!m_initialised)
		{
			throw Exception("Trying to run MYENGINE without first calling MYENGINE::Core::Init()!");
		}


		//------------------------------------MAIN MYENGINE LOOP------------------------------------//

		while (!m_inputManager->GetUserWantsQuit())
		{
			// get data for this 'frame'
			m_manager->UpdateDeltaTime(SDL_GetTicks());

			int w, h;
			SDL_GetWindowSize(m_window, &w, &h);
			m_manager->UpdateWindowDimensions(w, h);

			SDL_Event event;
			m_inputManager->HandleSDLEvents(&event, m_window);


			// logic loop
			m_manager->UpdateAll();
			m_manager->DrawAll();
			m_manager->CleanUp();

			SDL_GL_SwapWindow(m_window);
		}


		//------------------------------------DESTROY: MYENGINE, SDL, OPENGL, OPENAL------------------------------------//

		DestroyAll();

		std::cout << std::endl << (m_manager.use_count() < 1 ? "MYENGINE has shut down successfully." : "MYENGINE has failed to shut down correctly. Please ensure that the manager is not owned by any external code.") << " Press enter to continue..." << std::endl;
		std::cin.get();

		return 0;
	}


	void Core::DestroyAll()
	{
		m_inputManager.reset(); // Core only holds the input manager for ease of use- we don't want this reference to prevent it from being destroyed
		m_manager.reset(); // This will entirely destroy everything engine-related

		// OpenAL cleanup
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_ALcontext);
		alcCloseDevice(m_ALdevice);

		// SDL / OpenGL cleanup
		SDL_GL_DeleteContext(m_GLcontext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

} // namespace MYENGINE
