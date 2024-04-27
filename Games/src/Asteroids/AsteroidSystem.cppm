module;
#include <glm/glm.hpp>
export module Asteroids.AsteroidSystem;

export 
{

	constexpr int max_number_asteroid_points = 16;
	struct Asteroid
	{
		bool active = false;
		glm::vec2 position; // Center of asteroid
		float speed = 40.0f;
		float direction;
		float radius;
		int number_of_points = max_number_asteroid_points;
		glm::vec2 points[max_number_asteroid_points]; // @Incomplete, for when the rendering will involve several randomized points
	};

	class AsteroidSystem
	{
	public:
		static Asteroid* asteroids;
		static int count;

		static void Init();
		static void Shutdown();

		static void Simulate(float dt);
		static void Render(float dt);

		static void SpawnAsteroid(glm::vec2 position, float radius = 10.0f, float direction = 0.0f);
	};
}