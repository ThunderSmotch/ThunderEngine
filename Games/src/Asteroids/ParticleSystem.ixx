module;
#include <glm/glm.hpp>
export module Asteroids.ParticleSystem;

export{
	struct Particle
	{
		bool active = false;

		glm::vec2 position;

		float speed;
		float direction;
		float lifetime;

		glm::vec4 color;
	};

	class ParticleSystem
	{
	public:
		static void Init();
		static void Shutdown();

		static void Simulate(float dt);
		static void Render(float dt);

		static void SpawnParticle(glm::vec2 position, float lifetime, glm::vec4 color);
	private:
		static Particle particles[1000];
		static int count;
	};
}