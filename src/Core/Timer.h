#pragma once

#include <chrono>

namespace ThunderEngine
{
	// A simple timer class to count time since instantiation or since the last reset.
	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start_;

	public:
		Timer();

		// Resets the timer.
		void Reset();

		// Returns seconds elapsed since reset or creation.
		float Elapsed();
	};
}