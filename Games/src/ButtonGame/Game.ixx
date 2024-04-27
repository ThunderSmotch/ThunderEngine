module;
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
export module Game.ButtonGame;

import ThunderEngine;

import Animation.Scene;
import Animation.Intro;
import Animation.AnimationTest;
import Animation.LineTest;
import SVG.SVGParser;

import ButtonGame.Button;

using namespace ThunderEngine;

constexpr int WIDTH_2 = 1280 / 2;
constexpr int HEIGHT_2 = 720 / 2;

OrthographicCamera camera(-WIDTH_2, WIDTH_2, -HEIGHT_2, HEIGHT_2);

void Render(Scene* scene, float dt) 
{
    // Clear screen
    RendererAPI::SetClearColor({ 0.3f, 0.3f, 0.7f, 1.0f });
    RendererAPI::Clear();

    Renderer2D::StartScene(camera);
    RendererText::StartScene(camera);

    scene->Render(dt);

    Renderer2D::EndScene();
    RendererText::EndScene();
}

export int Main()
{
    //system("chcp 65001"); to enable UTF8
    App app("The Button Game", 1280, 720);
    
    ButtonMachine scene;
    Scene* current_scene = &scene;

    //Testing sound system
    SoundPlayer::Init();
    SoundPlayer::LoadSound("res/audio/breakout.mp3", "breakout");
    SoundPlayer::LoadSound("res/audio/hitHurt.wav", "hit");
    Ref<Font> font = CreateRef<Font>("res/fonts/Roboto-Regular.ttf", 256, 1024);
    
    //font->SaveAtlasToFile("font");

    //Testing text system
    RendererText::Init();
    RendererText::LoadFont(font);

    //RendererText::LoadFont2("res/fonts/VCR_OSD_MONO_1.001.ttf");

    // Loading textures
    //container = Texture2D::Create("res/textures/container.jpg");
    //awesome_face = Texture2D::Create("res/textures/awesomeface.png");

    Ref<MouseInput> mouse_input = MouseInput::Create({ MouseKeyCode::MOUSE_LEFT });

    app.AddCallback([current_scene, & mouse_input](float dt) { current_scene->ProcessInput(*mouse_input); });
    app.AddCallback([current_scene](float dt) { current_scene->Update(dt); });
    app.AddCallback([current_scene](float dt) { Render(current_scene, dt); });

    app.Run();

    return 0;
}