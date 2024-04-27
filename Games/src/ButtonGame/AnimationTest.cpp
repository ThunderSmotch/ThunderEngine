module;
#include <glm/glm.hpp>
#define ANIMATE(animation, ...) Add(CreateRef<animation>(__VA_ARGS__))
module Animation.AnimationTest;
using namespace ThunderEngine;


AnimationScene::AnimationScene()
{
    box = Animatable(glm::vec3({ 0.0f, 300.0f, 0.0f }), 0.0f, glm::vec2(100.0f));

    // Create cutscene
    box_fade_in.ANIMATE(FadeIn, box);
    box_fade_in.Add(CreateRef<MoveTo>(box, 0.0f, 0.0f));
    box_fade_in.Add(CreateRef<Wait>(box));
    box_fade_in.ANIMATE(MoveTo, box, -300.0f, 0.0f);
}

void AnimationScene::Render(float dt) {
    // Clear screen
    RendererAPI::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    RendererAPI::Clear();

    box.Render();
}

void AnimationScene::Update(float dt)
{
    static float t = 0;
    t += dt;

    box_fade_in.Update(dt);
}

void AnimationScene::ProcessInput(MouseInput& mouse)
{
    if (mouse.GetIsKeyPress(MouseKeyCode::MOUSE_LEFT))
    {
        box_fade_in.Play();
    }
}

// What I want to achieve:
// Have some sort of animation code so that i can program an animation like
// MoveTo(x, y)
// FadeIn(x, y)
// FadeOut(x, y)
// Wait(s)
// KeyFrames?
// Types of interpolation
