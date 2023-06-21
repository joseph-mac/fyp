#ifndef _MYENGINE_AUDIO_H_
#define _MYENGINE_AUDIO_H_

#include "engine/Defines.h"
#include "engine/Resource.h"

#include <AL/al.h>

#include <string>

namespace MYENGINE
{

	struct Audio : public Resource
	{
	public:
		Audio(weak(ResourceManager) resourceManager, const char* _filePath);
		~Audio();

		void Initialise() override;

		/**
		* @returns the Audio's file path.
		**/
		const char* GetFilePath();

	protected:
		std::string m_filePath;

		ALenum m_format;
		ALsizei m_frequency;
	};

} // namespace MYENGINE

#endif
