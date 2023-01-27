#include "tepch.h"
#include "SoundPlayer.h"

#include <miniaudio/miniaudio.h>


namespace ThunderEngine
{
	ma_engine SoundPlayer::engine_;
	std::unordered_map<std::string, ma_sound*> SoundPlayer::sounds_map_;

	void SoundPlayer::Init()
	{
		ma_result result;
		result = ma_engine_init(nullptr, &engine_);
		if (result != MA_SUCCESS)
		{
			TE_ERROR("Failed to initialize the audio engine! Error code: %", result);
		}
	}

	void SoundPlayer::Shutdown()
	{
		// TODO deinit sounds and delete pointers
		


		ma_engine_uninit(&engine_);
	}

	void SoundPlayer::LoadSound(const std::string& filepath, const std::string& name)
	{
		ma_sound* sound = new ma_sound();
		ma_result result;
		result = ma_sound_init_from_file(&engine_, filepath.c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, sound);
		if (result != MA_SUCCESS) {
			TE_ERROR("Failed to load sound at %. Error code: %", filepath, (int)result);
		}
		else 
		{
			sounds_map_[name] = sound;
		}
	}

	void SoundPlayer::PauseSound(const std::string& sound_name)
	{
		ma_sound_stop(sounds_map_[sound_name]);
	}

	void SoundPlayer::PlaySound(const std::string& sound_name)
	{
		ma_sound_start(sounds_map_[sound_name]);
	}

}