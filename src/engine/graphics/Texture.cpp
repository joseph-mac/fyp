#include "Texture.h"

#include "engine/Exception.h"
#include "engine/components/Component_Types.h"
#include "engine/managers/ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl/glew.h>

#include <iostream>

namespace MYENGINE
{

	/*void SetDataAsDefault(unsigned char* _dataPtr)
	{
		std::string s = "-61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 98 -123 -116 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -61 -51 -40 -1 -3 -3 -3 -3 -35 -35 -35 -35 47 -103 126 18 -69 -111 37";
		
		_dataPtr = reinterpret_cast<unsigned char*>(const_cast<char*>(s.c_str()));
	}*/

	Texture::Texture(weak(ResourceManager) resourceManager, const char* _filePath)
		: Resource(resourceManager, _filePath)
	{
		m_filePath = _filePath;

		m_hasTransparency = false;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
	}

	void Texture::Initialise()
	{
		int w = 0;
		int h = 0;

		unsigned char* data = stbi_load(m_filePath.c_str(), &w, &h, NULL, 4);

		if (!data)
		{
			// Turn this instance into a dummy which returns the ID of default texture
			m_id = GetResourceManager()->GetDefaultTexture()->GetId();
			return;
		}

		/*std::string dataText(reinterpret_cast<char*>(data));
		for (size_t i = 0; i < dataText.length(); i++)
		{
			std::cout << std::to_string((int)dataText[i]) << ' ';
		}
		std::cout << '.';*/

		// Check for any transparent or translucent pixels
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int e = (x + w * y) * 4;
				unsigned bytePerPixel = 4;
				unsigned char* pix = data + e;
				if (pix[3] != 255)
				{
					m_hasTransparency = true;
					break;
				}
			}

			if (m_hasTransparency)
			{
				break;
			}
		}

		// Create and bind a texture.
		glGenTextures(1, &m_id);

		if (!m_id) { throw Exception("Texture failed to generate. Filepath was: " + m_filePath); }

		glBindTexture(GL_TEXTURE_2D, m_id);

		// Upload the image data to the bound texture unit in the GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Free the loaded data because we now have a copy on the GPU
		free(data);

		// Generate Mipmap so the texture can be mapped correctly
		glGenerateMipmap(GL_TEXTURE_2D);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		// Unbind the texture because we are done operating on it
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const char* Texture::GetFilePath() { return m_filePath.c_str(); }




	RenderTexture::RenderTexture(weak(ResourceManager) resourceManager, const char* _filePath, weak(Camera) _camera, weak(ShaderProgram) _shaderProgram)
		: Texture(resourceManager, _filePath)
	{
		m_linkedCamera = _camera;
		m_linkedShaderProgram = _shaderProgram;
		m_actualDimensions = m_linkedCamera.lock()->GetViewSize();

		m_fbo = -1;
	}

	void RenderTexture::Initialise()
	{
		m_fbo = 0;
		glGenFramebuffers(1, &m_fbo);
		if (!m_fbo) { throw Exception("FBO failed to generate for render texture. Render texture name was: " + m_filePath); }
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		m_id = 0;
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_actualDimensions.x, (GLsizei)m_actualDimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id, 0);

		GLuint rbo = 0;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)m_actualDimensions.x, (GLsizei)m_actualDimensions.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}



	CubeTexture::CubeTexture(weak(ResourceManager) resourceManager, const char* _filePath, const char* _fileType)
		: Texture(resourceManager, _filePath)
	{
		m_fileType = _fileType;
	}

	void CubeTexture::Initialise()
	{
		glGenTextures(1, &m_id);
		if (!m_id) { throw Exception("CubeTexture failed to generate. Filepath was: " + m_filePath); }

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

		int prevW = 0;
		int prevH = 0;
		bool faceErr = false;

		int w = 0;
		int h = 0;
		unsigned char* data;
		std::string facePath = "";

		for (GLuint f = 0; f < 6; f++)
		{
			facePath = m_filePath + std::to_string(f) + m_fileType;
			faceErr = stbi_info(facePath.c_str(), &w, &h, NULL) == 1 ? false : true;

			if (faceErr)
			{
				WARN("CubeTexture face " << f << " is corrupted or not a supported filetype.");
			}
			else if (f != 0 && (w != prevW || h != prevH))
			{
				WARN("CubeTexture face " << f << " has different dimensions to face 0; this is not valid.");
				faceErr = true;
			}
			else
			{
				data = stbi_load(facePath.c_str(), &w, &h, NULL, 4);
				if (!data)
				{
					WARN("CubeTexture face " << f << " could not be loaded.");
					faceErr = true;
				}
			}
			
			if (faceErr)
			{
				// If there is no image file for this face, use 0, which must always be present
				facePath = m_filePath + '0' + m_fileType;
				data = stbi_load(facePath.c_str(), &w, &h, NULL, 4);

				if (!data)
				{
					// If the file for face 0 cannot be loaded, load the default texture and use it for all faces

					data = stbi_load(GetResourceManager()->GetDefaultTexture()->GetFilePath(), &w, &h, NULL, 4);
					for (GLuint f2 = 0; f2 < 6; f2++)
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + f2, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					}
					free(data);

					break;
				}
			}
			else
			{
				prevW = w;
				prevH = h;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + f, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			free(data);
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

} // namespace MYENGINE
