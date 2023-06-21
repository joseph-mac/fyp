#ifndef _MYENGINE_AUDIOCOMPONENTS_H_
#define _MYENGINE_AUDIOCOMPONENTS_H_

#include "engine/Component.h"
#include "engine/audio/Audio.h"

#include <AL/al.h>

namespace MYENGINE
{

	struct AudioListener : public Component
	{
	public:
		AudioListener(weak(GameObject) _parentObject);
		~AudioListener();
	};

	struct AudioSource : public Component
	{
	public:
		AudioSource(weak(GameObject) _parentObject, weak(Audio) _audio, float _pitch, float _volume);
		~AudioSource();

		/**
		* Set the Audio resource for this source to use.
		* 
		* @param _audio : the Audio for the source to use.
		**/
		void SetAudio(weak(Audio) _audio);

		/**
		* Play the AudioSource's Audio once.
		**/
		void PlayOnce();

		/**
		* Play the AudioSource's Audio, then replay it each time playback finishes.
		**/
		void PlayLoop();

		/**
		* Prevent the AudioSource from replaying its Audio after being started with PlayLoop().
		**/
		void StopLoop();

		/**
		* Set the pitch for the AudioSource to play at.
		* 
		* @param _pitch : the new pitch to use.
		**/
		void SetPitch(float _pitch);

		/**
		* Set the volume for the AudioSource to play at.
		*
		* @param _volume : the new volume to use.
		**/
		void SetVolume(float _volume);

	protected:
		void Update() override;

		ALuint m_sourceID;

		weak(Audio) m_audio;
		bool m_looping;
		float m_pitch;
		float m_volume;
	};

} // namespace MYENGINE

#endif
