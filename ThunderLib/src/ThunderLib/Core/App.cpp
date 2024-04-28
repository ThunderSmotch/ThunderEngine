module ThunderLib.App;

import std;
import ThunderLib.Timer;
import ThunderLib.Renderer;

namespace ThunderLib
{
    App* App::instance_ = nullptr;

    App::App(const std::string& title, u32 width, u32 height)
        : running_(true)
    {
        instance_ = this;

        window_ = Window::Create(WindowProps(title, width, height));
        window_->SetWindowCloseCallback([this]() { this->OnWindowClose(); });

        Renderer::Init();
    }

    App::~App()
    {
        Renderer::Shutdown();
    }

    void App::AddCallback(const std::function<void(float)>& callback) {
        callbacks_.push_back(callback);
    }

    void App::Run()
    {
        Timer timer;
        // FIXME dt goes haywire when we are debugging due to halting the application execution
        float dt = 0.0f;
        while (running_)
        {   
            // Check timestep
            dt = timer.Elapsed();
            timer.Reset();

            // TODO At some point we need to decouple the render callbacks from the physics/simulation ones
            // Also worth reading this again https://gafferongames.com/post/fix_your_timestep/
            // And use an accumulator based solution!

            // Call callbacks with the timestep
            window_->OnPreUpdate();
            for (const auto& callback : callbacks_)
            {
                callback(dt);
            }
            window_->OnUpdate();
        }
    }

    void App::Close()
    {
        running_ = false;
    }

    void App::OnWindowClose()
    {
        Close();
    }
}