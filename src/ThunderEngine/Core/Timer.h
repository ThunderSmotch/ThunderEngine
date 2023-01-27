#pragma once

#include <chrono>

namespace ThunderEngine
{	
	/// <summary>
	/// A simple timer class to track time
	/// </summary>
	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start_;

	public:
		/// <summary>
		/// Starts tracking time since construction
		/// </summary>
		Timer();

		/// <summary>
		/// Resets the timer
		/// </summary>
		void Reset();

		/// <summary>
		/// Returns seconds elapsed since reset or construction
		/// </summary>
		/// <returns>Seconds elapsed</returns>
		float Elapsed() const;
	};
}