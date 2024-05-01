module Asteroids.ProjectileSystem;

import std;
import ThunderLib;
import Asteroids.AsteroidSystem;
import Asteroids.ParticleSystem;

extern int the_canvas_width;
extern int the_canvas_height;

struct Projectile {
	bool active        = false;
	Vec2 position = Vec2(0);
	float direction    = 0.0f;
	float lifetime     = 5.0f;
	float speed        = 800.0f;
	bool friendly      = false;
};

constexpr int MAX_PROJECTILES = 100;

Projectile* ProjectileSystem::projectiles_ = nullptr;
int ProjectileSystem::count_ = 0;

void ProjectileSystem::Init()
{
	projectiles_ = new Projectile[MAX_PROJECTILES];
	count_ = 0;
}

void ProjectileSystem::Shutdown()
{
	delete[] projectiles_;
}

void ProjectileSystem::Simulate(float dt)
{
	// @Incomplete @Speed Dont need to check entire array everytime
	for (int i = 0; i < MAX_PROJECTILES; i++)
	{
		Projectile& proj = projectiles_[i];

		if (!proj.active) continue;

		proj.lifetime -= dt;
		if (proj.lifetime < 0.0f)
		{
			proj.active = false;
		}

		proj.position.x += dt * proj.speed * sin(proj.direction);
		proj.position.y += dt * proj.speed * cos(proj.direction);

		// Handle loop around the field
		if (proj.position.x >= (float) the_canvas_width)
		{
			proj.position.x = 0.0f;
		}
		else if (proj.position.x <= 0)
		{
			proj.position.x = (float)the_canvas_width;
		}

		if (proj.position.y >= (float)the_canvas_height)
		{
			proj.position.y = 0.0f;
		}
		else if (proj.position.y <= 0)
		{
			proj.position.y = (float)the_canvas_height;
		}

		// See if collided with any Asteroid
		// TODO @Incomplete Fix this should be head/tail
		// @Cleanup This is a nice algorithm for convex poly thingies
		for (int i = 0; i < AsteroidSystem::count; i++)
		{
			Asteroid& ast = AsteroidSystem::asteroids[i];
			if (!ast.active) continue;

			bool collided = true;

			for (int j = 0; j < ast.number_of_points; j++)
			{
				Vec2 ip = ast.position + ast.points[j];
				Vec2 fp = ast.position + ast.points[(j + 1) % ast.number_of_points];

				// Using the cross product to check if point is to the right of the line
				// If it is then break as it cannot be colliding as asteroids/polygons are drawn in counterclockwise order.
				float side = (fp.x - ip.x) * (proj.position.y - ip.y) - (fp.y - ip.y) * (proj.position.x - ip.x);
				
				if (side < 0.0f) // Not inside
				{
					collided = false;
					break; 
				}
			}

			if (!collided) continue;

			//Handle collision

			// @Incomplete inset particle effects/sounds here
			proj.active = false;
			ast.active = false;
		}
	}
}

void ProjectileSystem::Render(float dt)
{
	// @Incomplete @Speed Dont need to check entire array everytime
	for (int i = 0; i < MAX_PROJECTILES; i++)
	{
		if (!projectiles_[i].active) continue;
		
		Vec2 trail = 30.0f * Vec2(sin(projectiles_[i].direction), cos(projectiles_[i].direction));
		Vec4 color = projectiles_[i].friendly ? Vec4(1.0f, 1.0f, 0.0f, 1.0f) : Vec4(1.0f, 0.0f, 0.1f, 1.0f);

		ThunderLib::Renderer2D::DrawLine(projectiles_[i].position, projectiles_[i].position - trail, color);


		ParticleSystem::SpawnParticle(projectiles_[i].position, 2.0f, Color::Red);

	}
}

void ProjectileSystem::SpawnProjectile(Vec2 position, float direction, bool friendly, float lifetime)
{
	// @Incomplete Need to do some head tail chasing or expand the buffer here
	if (count_ == MAX_PROJECTILES) 
	{
		count_ = 0;
	}

	Projectile& projectile = projectiles_[count_];
	projectile.position  = position;
	projectile.direction = direction;
	projectile.lifetime = lifetime;
	projectile.friendly = friendly;
	projectile.active = true;

	count_++;
}
