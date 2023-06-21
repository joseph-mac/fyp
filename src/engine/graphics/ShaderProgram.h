#ifndef _MYENGINE_SHADERPROGRAM_H_
#define _MYENGINE_SHADERPROGRAM_H_

#include "engine/Defines.h"
#include "engine/Resource.h"

namespace MYENGINE
{

	struct ShaderProgram : public Resource
	{
	public:
		/**
		* ShaderProgram CTOR.
		*
		* @param resourceManager : The ResourceManager instance to be managed by.
		* @param _name : The name of the ShaderProgram.
		**/
		ShaderProgram(weak(ResourceManager) resourceManager, const char* _name);
		~ShaderProgram();
	};

} // namespace MYENGINE

#endif
