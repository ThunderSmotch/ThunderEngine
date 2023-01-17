#pragma once

#include <iostream>
#include <vector>

#include "Core/App.h"
#include "Core/KeyInput.h"
#include "Core/Timer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/GraphicsContext.h"

using namespace ThunderEngine;

KeyInput* key_input = nullptr;

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
        App::Get().GetWindow().GetGraphicsContext().SetWireframeMode(true);
    }
    if (key_input->GetIsKeyDown(KeyCode::F))
    {
        App::Get().GetWindow().GetGraphicsContext().SetWireframeMode(false);
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

void GameLoop()
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

void Render()
{
    switch (current_state)
    {
    case GameState::WAITING:
        Renderer::SetClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        break;
    case GameState::RED:
        Renderer::SetClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        break;
    case GameState::GREEN:
        Renderer::SetClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        break;
    case GameState::FINISHED:
        Renderer::SetClearColor(0.0f, 0.75f, 0.5f, 1.0f);
        break;
    }

    // Clear screen
    //Renderer::SetClearColor(0.7f, 0.9f, 0.1f, 1.0f);
    Renderer::Clear();

    // Rendering a triangle
    //Renderer2D::DrawTriangle();
}

int Main()
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

    delete key_input;
    return 0;
}