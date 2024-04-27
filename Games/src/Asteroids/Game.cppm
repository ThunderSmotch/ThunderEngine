module;
// @Incomplete is this the best way to have globals available?
int the_canvas_width  = 800;
int the_canvas_height = 800;
export module Game.Asteroids;

import ThunderEngine;

import Asteroids.Player;
import Asteroids.ProjectileSystem;
import Asteroids.AsteroidSystem;
import Asteroids.ParticleSystem;

using namespace ThunderEngine;

static void HandleInput(float dt, KeyInput& input, Player& player)
{
	if (input.GetIsKeyPress(KeyCode::ESCAPE))
	{
		App::Get().Close();
	}
	player.HandleInput(dt);
}

static void Simulate(float dt)
{
	ProjectileSystem::Simulate(dt);
	AsteroidSystem::Simulate(dt);
	ParticleSystem::Simulate(dt);
}

static void Render(float dt, const OrthographicCamera& camera, const Player& player)
{
	ThunderEngine::RendererAPI::Clear();

	Renderer2D::StartScene(camera);

	ProjectileSystem::Render(dt);
	player.Render();
	AsteroidSystem::Render(dt);
	ParticleSystem::Render(dt);
	Renderer2D::EndScene();
}

export int Main() 
{
	ThunderEngine::App app("Asteroids", the_canvas_width , the_canvas_height);

	OrthographicCamera camera(0, (float) the_canvas_width, 0, (float) the_canvas_height);

	// Player
	Player player;
	ParticleSystem::Init();
	ProjectileSystem::Init();
	AsteroidSystem::Init();
	// Not sure if using: Ref<Texture2D> asteroid_texture = Texture2D::Create("res/textures/container.jpg");

	// Game and menu
	Ref<KeyInput> input = KeyInput::Create({KeyCode::SPACE, KeyCode::ESCAPE });

	// Set render clear color to black
	ThunderEngine::RendererAPI::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	// Add callbacks
	app.AddCallback([&input, &player](float dt) {HandleInput(dt, *input, player); });
	app.AddCallback(Simulate);
	app.AddCallback([&camera, &player](float dt) {Render(dt, camera, player); });

	// @Temporary
	AsteroidSystem::SpawnAsteroid({ the_canvas_width/2, the_canvas_height/2 });

	// Run game
	app.Run();


	// Shutdown
	ProjectileSystem::Shutdown();
	AsteroidSystem::Shutdown();
	ParticleSystem::Shutdown();

	return 0;
}