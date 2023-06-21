#include "AudioComponents.h"

namespace MYENGINE
{

	AudioListener::AudioListener(weak(GameObject) _parentObject)
		: Component(_parentObject, "AUDIO_LISTENER")
	{

	}

	AudioListener::~AudioListener()
	{

	}


	AudioSource::AudioSource(weak(GameObject) _parentObject, weak(Audio) _audio, float _pitch, float _volume)
		: Component(_parentObject, "AUDIO_SOURCE")
	{
		alGenSources(1, &m_sourceID);

		m_looping = false;

		SetAudio(_audio);
		SetPitch(_pitch);
		SetVolume(_volume);
	}

	AudioSource::~AudioSource()
	{
		alDeleteSources(1, &m_sourceID);
	}

	void AudioSource::Update()
	{
		ALint state = 0;
		alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
		if (m_looping && state == AL_STOPPED)
		{
			PlayOnce();
		}
	}

	void AudioSource::SetAudio(weak(Audio) _audio)
	{
		m_audio = _audio;
		alSourcei(m_sourceID, AL_BUFFER, m_audio.lock()->GetId());
	}

	void AudioSource::PlayOnce()
	{
		alSourcePlay(m_sourceID);
	}

	void AudioSource::PlayLoop()
	{
		m_looping = true;
		PlayOnce();
	}
	void AudioSource::StopLoop()
	{
		m_looping = false;
	}

	void AudioSource::SetPitch(float _pitch)
	{
		m_pitch = _pitch;
		alSourcef(m_sourceID, AL_PITCH, m_pitch);
	}

	void AudioSource::SetVolume(float _volume)
	{
		m_volume = _volume;
		alSourcef(m_sourceID, AL_GAIN, m_volume);
	}

} // namespace MYENGINE
