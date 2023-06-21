#include "Audio.h"

#include "engine/Exception.h"

#include <vector>

extern "C"
{
	extern int stb_vorbis_decode_filename(const char* filename, int* channels, int* sample_rate, short** output);
}

namespace MYENGINE
{

	Audio::Audio(weak(ResourceManager) resourceManager, const char* _filePath)
		: Resource(resourceManager, _filePath)
	{
		m_filePath = _filePath;

		m_format = -1;
		m_frequency = -1;
	}

	Audio::~Audio()
	{
		alDeleteBuffers(1, &m_id);
	}

	void Audio::Initialise()
	{
		std::vector<unsigned char> bufferData;

		int channels = 0;
		int sampleRate = 0;
		short* output = NULL;

		size_t samples = stb_vorbis_decode_filename(m_filePath.c_str(),
			&channels, &sampleRate, &output);

		if (samples == -1)
		{
			throw Exception("Failed to open file '" + m_filePath + "' for decoding");
		}

		// Record the format required by OpenAL
		if (channels < 2)
		{
			m_format = AL_FORMAT_MONO16;
		}
		else
		{
			m_format = AL_FORMAT_STEREO16;
		}

		// Copy (# samples) * (1 or 2 channels) * (16 bits == 2 bytes == short)
		bufferData.resize(samples * channels * sizeof(short));
		memcpy(&bufferData.at(0), output, bufferData.size());

		// Record the sample rate required by OpenAL
		m_frequency = sampleRate;

		// Clean up the read data
		free(output);

		alGenBuffers(1, &m_id);
		alBufferData(m_id, m_format, &bufferData.at(0), static_cast<ALsizei>(bufferData.size()), m_frequency);
	}

	const char* Audio::GetFilePath() { return m_filePath.c_str(); }

} // namespace MYENGINE
