#pragma once

#include <vector>

#include "Core/App.h"
#include "Core/KeyInput.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RendererText.h"

#include "Sound/SoundPlayer.h"
#include <algorithm>

#include <imgui/imgui.h>

using namespace ThunderEngine;

Ref<Texture2D> container, awesome_face;

constexpr int WIDTH_2 = 1280 / 2;
constexpr int HEIGHT_2 = 720 / 2;

OrthographicCamera camera(- WIDTH_2, WIDTH_2, - HEIGHT_2, HEIGHT_2);

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
    // Testing ImGui
    ImGui::Begin("Debug");
    ImGui::Text("Debug Window ;)");
    ImGui::End();

    // Clear screen
    RendererCommand::SetClearColor({ 0.3f, 0.3f, 0.7f, 1.0f });
    RendererCommand::Clear();

    // 2D rendering
    
    Renderer2D::StartScene(camera);
    //// Rendering a triangle
    ////Renderer2D::DrawTriangle();
    Renderer2D::DrawQuad(glm::vec2({ -0.5f, -0.5f }), glm::vec2({ 0.25f, 1.0f }), glm::vec4({ 0.0f, 1.0f, 1.0f, 1.0f }));
    Renderer2D::DrawQuad(glm::vec2({ 0.5f, -0.5f }), glm::vec2({ 0.25f, 1.0f }), glm::vec4({ 0.0f, 1.0f, 1.0f, 1.0f }));
    //
    Renderer2D::DrawQuad(glm::vec2({ 0.0f, 0.0f}), glm::vec2({ 1.0f, 0.5f }), container, glm::vec4({ 1.0f, 1.0f, 1.0f, 1.0f}));

    Renderer2D::DrawQuad(glm::vec2({ 0.0f, 0.0f }), glm::vec2({ 1000.0f, 1000.0f }), awesome_face, glm::vec4({ 1.0f, 1.0f, 1.0f, 1.0f }));
    Renderer2D::EndScene();

    RendererText::StartScene(camera);

    auto randchar = []() -> char
    {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(10, 0);
    std::generate_n(str.begin(), 10, randchar);

    RendererText::DrawString(0, 0, "Random Text Estilo Minecraft");
    RendererText::DrawString(0, -64, str.c_str());

    RendererText::DrawString(0, 64, "Ola Diogo!");

    RendererText::EndScene();
}

int Main()
{
    App app("OpenGL", 1280, 720);

    //Testing sound system
    SoundPlayer::Init();
    SoundPlayer::LoadSound("res/audio/breakout.mp3", "breakout");

    //Testing text system
    RendererText::Init();
    RendererText::LoadFont2("res/fonts/open_sans.ttf");

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