module;
#include <glm/glm.hpp>
export module Asteroids.ProjectileSystem;

export {
	struct Projectile;

	class ProjectileSystem
	{
	public:
		static void Init();
		static void Shutdown();

		static void Simulate(float dt);
		static void Render(float dt);

		static void SpawnProjectile(glm::vec2 position, float direction, bool friendly = false, float lifetime = 1.0f);
	private:
		static Projectile* projectiles_;
		static int count_;
	};

}