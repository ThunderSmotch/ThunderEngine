#include "tepch.h"
#include "Timer.h"

namespace ThunderEngine
{

	Timer::Timer()
	{
		Reset();
	}

	void Timer::Reset()
	{
		start_ = std::chrono::high_resolution_clock::now();
	}

	float Timer::Elapsed() const
	{
		return 1e-9f*((float) std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start_).count());
	}
}