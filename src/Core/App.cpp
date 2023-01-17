#include "App.h"

#include "Renderer/Renderer.h"

namespace ThunderEngine
{
    App* App::instance_ = nullptr;

    App::App(std::string title, uint32_t width, uint32_t height)
        : window_(nullptr), running_(false)
    {
        instance_ = this;

        window_ = Window::Create(WindowProps(title, width, height));
        window_->SetWindowCloseCallback(std::bind(&App::OnWindowClose, this));
        running_ = true;

        Renderer::Init();
    }

    App::~App()
    {
        Renderer::Shutdown();
    }

    void App::AddCallback(std::function<void()> callback) {
        callbacks_.push_back(callback);
    }

    void App::Run()
    {
        while (running_)
        {
            for (auto& callback : callbacks_)
            {
                callback();
            }
            window_->OnUpdate();
        }
    }

    void App::OnWindowClose()
    {
        Close();
    }

    void App::Close()
    {
        running_ = false;
    }
}