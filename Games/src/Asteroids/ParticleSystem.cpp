module;
#include <glm/glm.hpp>
module Asteroids.ParticleSystem;
import ThunderLib;

int ParticleSystem::count = 0;
//Particle ParticleSystem::particles;
Particle ParticleSystem::particles[1000] = {};

void ParticleSystem::Init()
{
}

void ParticleSystem::Shutdown()
{
}

void ParticleSystem::Simulate(float dt)
{
	//Test
}

void ParticleSystem::Render(float dt)
{
	for (int i = 0; i < 1000; i++)
	{
		Particle& p = particles[i];
		if (!p.active) continue;

		p.lifetime -= dt;
		if (p.lifetime < 0.0f)
		{
			p.active = false;
			continue;
		}
		// @Incomplete insert size of particles in the struct maybe
		ThunderLib::Renderer2D::DrawQuad(p.position, {1,1}, p.color);
	}
}

void ParticleSystem::SpawnParticle(glm::vec2 position, float lifetime, glm::vec4 color)
{
	if (count == 1000) count = 0;

	Particle& p = particles[count];
	p.active = true;
	p.position = position;
	p.lifetime = lifetime;
	p.color = color;

	count++;
}
