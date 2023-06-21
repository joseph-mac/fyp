#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Manager.h"
#include "PhysicsManager.h"
#include "IOManager.h"
#include "GameObject.h"
#include "Primitives.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


#include "stb_image.h"
#include "bugl.h"

#include <iostream>
#include <memory>
#include <exception>
#include <vector>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

const GLfloat rgb[] = {
		1.0f, 0.0f, 0.0f, 1.0f, //red
		0.0f, 1.0f, 0.0f, 1.0f, //green
		0.0f, 0.0f, 1.0f, 1.0f  //blue
};

const GLfloat purple[] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, //red
		0.0f, 0.0f, 1.0f, 1.0f  //blue
};

const GLfloat tropical[] = {
		1.0f, 0.5f, 0.0f, 1.0f, //orange
		0.0f, 1.0f, 0.0f, 1.0f, //green
		0.5f, 0.5f, 1.0f, 1.0f  //light blue
};

int main()
{
	// window setup
	SDL_Window *window = SDL_CreateWindow("Game Window",
  		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
  		WINDOW_WIDTH, WINDOW_HEIGHT,
  		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	
	// error checking
	if (!SDL_GL_CreateContext(window)) { throw std::exception(); }
	if (glewInit() != GLEW_OK) { throw std::exception(); }

	// glEnables
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	// init manager
	Manager manager;
	IOManager* ioManager = manager.GetIOManager();

	// init engine-level things
	Primitives::CreatePlane(&manager);
	Primitives::CreateBox(&manager);
	Primitives::CreateTriangle(&manager);

	manager.CreateShaderProgram("depth-map");

	manager.CreateTexture("assets/textures/default.bmp");
	//manager.CreateTexture("assets/materials/shiny.bmp");
	manager.LoadFromSceneFile("assets/scenes/cathouse.txt");
	//manager.GetPhysicsManager()->Activate(true);

	// main loop
	while (!ioManager->GetUserWantsQuit())
	{
		// get data for this 'frame'
		manager.UpdateDeltaTime(SDL_GetTicks());

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		manager.UpdateWindowDimensions(w, h);

		SDL_Event event = { 0 };
		ioManager->HandleSDLEvents(&event, window);
		

		// logic loop
		manager.UpdateAll();
		manager.DrawAll();
		manager.CleanUp();


		SDL_GL_SwapWindow(window);
	}

	// destroy nicely
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}