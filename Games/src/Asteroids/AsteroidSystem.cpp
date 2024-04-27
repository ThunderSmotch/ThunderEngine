module;
#include <glm/glm.hpp>
module Asteroids.AsteroidSystem;
import ThunderLib;

constexpr int MAX_ASTEROIDS = 20;

Asteroid* AsteroidSystem::asteroids = nullptr;
int AsteroidSystem::count = 0;

void AsteroidSystem::Init()
{
	asteroids = new Asteroid[MAX_ASTEROIDS];
}

void AsteroidSystem::Shutdown()
{
	delete[] asteroids;
}

void AsteroidSystem::Simulate(float dt)
{
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		Asteroid& ast = asteroids[i];
		if (!ast.active) continue;

		ast.position.x += dt * ast.speed * cos(ast.direction);
		ast.position.y += dt * ast.speed * sin(ast.direction);
	}
}

void AsteroidSystem::Render(float dt)
{
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		Asteroid const& ast = asteroids[i];
		if (!ast.active) continue;

		// TODO Render asteroid @Incomplete garbage render

		for (int j = 0; j < ast.number_of_points; j++ )
		{
			glm::vec2 ip = ast.position + ast.points[j];
			glm::vec2 fp = ast.position + ast.points[(j + 1) % ast.number_of_points];
			ThunderLib::Renderer2D::DrawLine(ip, fp, Color::White);
		}
	}
}

void AsteroidSystem::SpawnAsteroid(glm::vec2 position, float radius, float direction)
{
	// @Incomplete Need to do some head tail chasing or expand the buffer here
	if (count == MAX_ASTEROIDS)
	{
		count = 0;
	}
	Asteroid& ast = asteroids[count];
	ast.position = position;
	ast.radius = radius; // @Incomplete Should be somewhat randomly generate
	ast.direction = direction;

	// @Incomplete need to randomly generate asteroids, for now we do squares
#if false
	glm::vec2 top_right = glm::vec2(ast.radius, ast.radius);
	glm::vec2 top_left = glm::vec2(-ast.radius, ast.radius);

	ast.points[0] = -top_left;
	ast.points[1] = top_right;
	ast.points[2] = top_left;
	ast.points[3] = -top_right;
#endif
	float rot = 0.0f;
	float drot = 6.28318530718f / (float) ast.number_of_points; // @Cleanup Move PI somewhere!!!
	for (int i = 0; i < ast.number_of_points; i++)
	{
		// @Cleanup
		//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // float 0  to 1
		float rad = ast.radius;// (0.7f + r * 0.5f)* ast.radius;
		ast.points[i] = glm::vec2( cos(rot)*rad , sin(rot)*rad );
		rot += drot;
	}


	ast.active = true;
	
	count++;
}
