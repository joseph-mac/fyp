cmake_minimum_required(VERSION 3.0)
project(PROJECT)

include_directories(
	dep/include
	src
)

add_library(engine_library
	src/engine/Component.cpp
	src/engine/Component.h
	src/engine/Core.cpp
	src/engine/Core.h
	src/engine/Defines.h
	src/engine/Exception.h
	src/engine/GameObject.cpp
	src/engine/GameObject.h
	src/engine/Resource.h
	src/engine/TypeDefs.h

	src/engine/audio/Audio.cpp
	src/engine/audio/Audio.h
	src/engine/audio/stb_vorbis.c

	src/engine/components/Component_Types.cpp
	src/engine/components/Component_Types.h

	src/engine/components/audio/AudioComponents.cpp
	src/engine/components/audio/AudioComponents.h

	src/engine/components/gui/GUIComponents.cpp
	src/engine/components/gui/GUIComponents.h

	src/engine/components/network/NetworkComponents.cpp
	src/engine/components/network/NetworkComponents.h

	src/engine/components/physics/ColliderComponents.cpp
	src/engine/components/physics/ColliderComponents.h
	src/engine/components/physics/PhysicsComponents.cpp
	src/engine/components/physics/PhysicsComponents.h

	src/engine/graphics/bugl.h
	src/engine/graphics/Primitives.cpp
	src/engine/graphics/Primitives.h
	src/engine/graphics/ShaderProgram.cpp
	src/engine/graphics/ShaderProgram.h
	src/engine/graphics/stb_image.h
	src/engine/graphics/Texture.cpp
	src/engine/graphics/Texture.h
	src/engine/graphics/VertexArray.cpp
	src/engine/graphics/VertexArray.h
	src/engine/graphics/VertexBuffer.cpp
	src/engine/graphics/VertexBuffer.h

	src/engine/gui/GUIDefs.cpp
	src/engine/gui/GUIDefs.h

	src/engine/input/Concepts.cpp
	src/engine/input/Concepts.h
	src/engine/input/ControlContext.cpp
	src/engine/input/ControlContext.h
	src/engine/input/InputDefs.cpp
	src/engine/input/InputDefs.h
	src/engine/input/InputSystemDefs.cpp
	src/engine/input/InputSystemDefs.h

	src/engine/managers/InputManager.cpp
	src/engine/managers/InputManager.h
	src/engine/managers/Manager.cpp
	src/engine/managers/Manager.h
	src/engine/managers/PhysicsManager.cpp
	src/engine/managers/PhysicsManager.h
	src/engine/managers/ResourceManager.cpp
	src/engine/managers/ResourceManager.h
	src/engine/managers/SceneLoader.cpp
	src/engine/managers/SceneLoader.h
	src/engine/managers/SettingsManager.cpp
	src/engine/managers/SettingsManager.h
	src/engine/managers/SubManager.cpp
	src/engine/managers/SubManager.h

	src/shared/NetworkDefs.cpp
	src/shared/NetworkDefs.h
)

link_directories(
	dep/lib
)

#find_library(LIB_OPENGL OpenGL32 dep/lib)
#find_library(LIB_GLEW glew32 dep/lib)
#find_library(LIB_SDL2 SDL2 dep/lib)
#find_library(LIB_SDL2MAIN SDL2main dep/lib)
target_link_libraries(engine_library
	glew32
	SDL2
	SDL2main
	OpenGL32
	openal32
	ws2_32
)

add_executable(game
	src/game/main.cpp
	src/game/implementations.cpp
	src/game/implementations.h

	src/game/components/GeneralComponents.cpp
	src/game/components/GeneralComponents.h
	src/game/components/ProjectileComponents.cpp
	src/game/components/ProjectileComponents.h
)

target_link_libraries(game
	engine_library
)

add_executable(server
	src/server/Server.cpp

	src/shared/NetworkDefs.cpp
	src/shared/NetworkDefs.h
)

target_link_libraries(server
	ws2_32
)
