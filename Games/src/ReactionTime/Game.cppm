export module Game.ReactionTime;

import ThunderLib.Base;
import ThunderLib.Timer;
import ThunderLib.App;
import ThunderLib.KeyInput;
import ThunderLib.GraphicsContext;

import ThunderLib.KeyCode;

import ThunderLib.Renderer;
import ThunderLib.Renderer2D;
import ThunderLib.RendererAPI;

import std;

using namespace ThunderLib;

Ref<KeyInput> key_input = nullptr;

enum class GameState
{
    WAITING,
    RED,
    GREEN,
    FINISHED,
};


// Game Variables
GameState current_state = GameState::WAITING;

Timer timer;
int seconds_to_wait = 0;
bool released = false;

// State transition flags
bool go_to_red = false;
bool go_to_green = false;
bool go_to_finished = false;
bool go_to_waiting = false;

void ProcessInput()
{
    if (key_input->GetIsKeyDown(KeyCode::W))
    {
        RendererAPI::SetWireframeMode(true);
        //App::Get().GetWindow().GetGraphicsContext().SetWireframeMode(true);
    }
    if (key_input->GetIsKeyDown(KeyCode::F))
    {
        RendererAPI::SetWireframeMode(false);
        //App::Get().GetWindow().GetGraphicsContext().SetWireframeMode(false);
    }
    if (key_input->GetIsKeyDown(KeyCode::ESCAPE))
    {
        App::Get().Close();
    }

    if (key_input->GetIsKeyDown(KeyCode::SPACE))
    {
        switch (current_state)
        {
        case GameState::WAITING:
            go_to_red = true;
            break;
        case GameState::GREEN:
            go_to_finished = true;
            break;
        case GameState::FINISHED:
            if (released) {
                go_to_red = true;
            }
            break;
        }
        released = false;
    }
    else
    {
        released = true;
    }
}

void GameLoop(float dt)
{
    // Input
    ProcessInput();

    // Entry functions
    if (go_to_waiting)
    {
        current_state = GameState::WAITING;

        go_to_waiting = false;
    }
    else if (go_to_green)
    {
        current_state = GameState::GREEN;

        timer.Reset(); //Start timer

        go_to_green = false;
    }
    else if (go_to_red)
    {
        current_state = GameState::RED;

        seconds_to_wait = std::rand() % 5 + 1; // 1 to 5 seconds
        timer.Reset(); //Start timer

        go_to_red = false;
    }
    else if (go_to_finished)
    {
        std::cout << "You took " << timer.Elapsed() << " seconds!" << std::endl; // Print reaction time
        current_state = GameState::FINISHED;

        go_to_finished = false;
    }

    // Behaviors during state
    switch (current_state)
    {
    case GameState::RED:
        if (timer.Elapsed() > seconds_to_wait)
        {
            go_to_green = true;
        }
        break;
    }
}

void Render(float dt)
{
    switch (current_state)
    {
    case GameState::WAITING:
        RendererAPI::SetClearColor({ 0.0f, 0.0f, 1.0f, 1.0f });
        break;
    case GameState::RED:
        RendererAPI::SetClearColor({ 1.0f, 0.0f, 0.0f, 1.0f });
        break;
    case GameState::GREEN:
        RendererAPI::SetClearColor({ 0.0f, 1.0f, 0.0f, 1.0f });
        break;
    case GameState::FINISHED:
        RendererAPI::SetClearColor({ 0.0f, 0.75f, 0.5f, 1.0f });
        break;
    }

    // Clear screen
    //Renderer::SetClearColor(0.7f, 0.9f, 0.1f, 1.0f);
    RendererAPI::Clear();

    // Rendering a triangle
    //Renderer2D::DrawTriangle();
}

export int Main()
{
    App app("Your Reaction Speed!", 1280, 720);

    std::cout << "Let's test YOUR REACTION SPEED!" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "When you press SPACE, the screen will turn red." << std::endl;
    std::cout << "When the screen turns green, press SPACE!" << std::endl;

    key_input = KeyInput::Create({ KeyCode::W, KeyCode::F, KeyCode::ESCAPE, KeyCode::SPACE });

    app.AddCallback(GameLoop);
    app.AddCallback(Render);
    app.Run();

    return 0;
}