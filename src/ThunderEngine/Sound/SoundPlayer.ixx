module;
#include <miniaudio/miniaudio.h>
#undef PlaySound
export module ThunderEngine.SoundPlayer;

import std;

struct ma_engine;
struct ma_sound;

export namespace ThunderEngine
{
	// TODO Incorporate SoundGroups to have different controls for different kinds of sounds
	// TODO Abstract the miniaudio away
	// PERF Music should be streamed while effects should be in memory! See miniaudio docs.

	class SoundPlayer 
	{
	private:
		static ma_engine engine_;
		static std::unordered_map<std::string, ma_sound*> sounds_map_;

	public:
		static void Init();
		static void Shutdown();

		static void LoadSound(const std::string& filepath, const std::string& sound_name);

		static void PlaySound(const std::string& sound_name);
		static void PauseSound(const std::string& sound_name);
	};
};