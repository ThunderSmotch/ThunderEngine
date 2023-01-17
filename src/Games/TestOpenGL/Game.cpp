#pragma once

#include <vector>

#include "Core/App.h"
#include "Core/KeyInput.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RendererText.h"

#include "Sound/SoundPlayer.h"

using namespace ThunderEngine;

Ref<Texture2D> container, awesome_face;
OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);

void ProcessInput(KeyInput& key_input)
{
    if (key_input.GetIsKeyDown(KeyCode::W))
    {
        RendererCommand::SetWireframeMode(true);
    }
    if (key_input.GetIsKeyDown(KeyCode::F))
    {
        RendererCommand::SetWireframeMode(false);
    }
    if (key_input.GetIsKeyDown(KeyCode::ESCAPE))
    {
        App::Get().Close();
    }
    static bool playing = false;

    if(key_input.GetIsKeyPress(KeyCode::P))
    {
        TE_WARN("Playing: %", playing);
        if (playing) 
        {
            SoundPlayer::PauseSound("breakout");
        }
        else
        {
            SoundPlayer::PlaySound("breakout");
        }
        playing = !playing;
    }
}

void Update()
{
    // Input
    //ProcessInput();
}

void Render()
{
    // Clear screen
    RendererCommand::SetClearColor({ 0.7f, 0.9f, 0.1f, 1.0f });
    RendererCommand::Clear();

    // 2D rendering
    Renderer2D::StartScene(camera);
    // Rendering a triangle
    //Renderer2D::DrawTriangle();
    Renderer2D::DrawQuad(glm::vec2({ -0.5f, -0.5f }), glm::vec2({ 0.25f, 1.0f }), glm::vec4({ 0.0f, 1.0f, 1.0f, 1.0f }));
    Renderer2D::DrawQuad(glm::vec2({ 0.5f, -0.5f }), glm::vec2({ 0.25f, 1.0f }), glm::vec4({ 0.0f, 1.0f, 1.0f, 1.0f }));
    
    Renderer2D::DrawQuad(glm::vec2({ 0.0f, 0.0f}), glm::vec2({ 1.0f, 0.5f }), container, glm::vec4({ 1.0f, 1.0f, 1.0f, 1.0f}));

    Renderer2D::DrawQuad(glm::vec2({ 0.0f, 0.0f }), glm::vec2({ 1.0f, 1.0f }), awesome_face, glm::vec4({ 1.0f, 1.0f, 1.0f, 1.0f }));

    RendererText::Test(0, 0, "A");

    Renderer2D::EndScene();
}

int Main()
{
    App app("OpenGL", 1280, 720);

    //Testing sound system
    SoundPlayer::Init();
    SoundPlayer::LoadSound("res/audio/breakout.mp3", "breakout");

    //Testing text system
    RendererText::Init();

    // Loading textures
    container = Texture2D::Create("res/textures/container.jpg");
    awesome_face = Texture2D::Create("res/textures/awesomeface.png");

    Ref<KeyInput> key_input = KeyInput::Create({ KeyCode::W, KeyCode::F, KeyCode::ESCAPE, KeyCode::P});

    app.AddCallback( [key_input]() {ProcessInput(*key_input);} );
    app.AddCallback(Update);
    app.AddCallback(Render);
    app.Run();

    return 0;
}