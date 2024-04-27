module Animation.Intro;

using namespace ThunderLib;

IntroScene::IntroScene()
{

}

void IntroScene::Render(float dt) {
    // Clear screen
    RendererAPI::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    RendererAPI::Clear();

    float center_string = RendererText::GetStringPixelWidth("In the beginning there was nothing...", 2.0f);
    RendererText::DrawString(-center_string/2, text_y, "In the beginning there was nothing...", 2.0f, {1.0f, 1.0f, 1.0f, text_alpha});
}

void IntroScene::Update(float dt)
{
    static float t = 0;
    t += dt;
    if (test_set)
    {
        text_alpha -= dt;
    }
    else 
    {
        text_y -= dt * text_move_speed;
        if (text_y <= 0.0f) {
            text_y = 0.0f;
            test_set = true;
        }
    }
}

void IntroScene::ProcessInput(MouseInput& mouse)
{

}

// What I want to achieve:
// Have some sort of animation code so that i can program an animation like
// MoveTo(x, y)
// FadeIn(x, y)
// FadeOut(x, y)
// Wait(s)
// KeyFrames?
// Types of interpolation
