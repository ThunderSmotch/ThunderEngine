#pragma once

#include <string>
#include <map>

#include "ThunderEngine.h"

struct ma_engine;
struct ma_sound;

namespace ThunderEngine
{
	// TODO Incorporate SoundGroups to have different controls for different kinds of sounds
	// TODO Abstract the miniaudio away
	// PERF Music should be streamed while effects should be in memory! See miniaudio docs.

	class SoundPlayer 
	{
	private:
		static ma_engine engine_;
		static std::map<std::string, ma_sound*> sounds_map_;

	public:
		static void Init();
		static void Shutdown();

		static void LoadSound(const std::string& filepath, const std::string& sound_name);

		static void PlaySound(const std::string& sound_name);
		static void PauseSound(const std::string& sound_name);
	};

	template <class T>
	class SoundPlayerBase
	{
		void implementation() {static_cast<T*>(this)->implementation();}

		//static void static_func()
		//{
		//	// ...
		//	T::static_sub_func();
		//	// ...
		//}
	};

	class SoundPlayaB : public SoundPlayerBase<SoundPlayaB>
	{
	public:
		void implementation() { TE_INFO("%", 3); };
		//static void static_sub_func();
	};

#if 0
	class SoundPlayaA;
	using SoundP = SoundPlayaA;
#else
	using SoundP = SoundPlayaB;
#endif
};