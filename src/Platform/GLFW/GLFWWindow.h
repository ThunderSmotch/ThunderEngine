#pragma once

#include "Core/Window.h"

#include <GLFW/glfw3.h>
#include <Renderer/GraphicsContext.h>

namespace ThunderEngine
{
	class GLFWWindow : public Window
	{
	private:
		GLFWwindow* window_;
		WindowProps window_props_;
		Ref<GraphicsContext> graphics_context_;
		bool vsync_enabled_;


		// A pointer to this data is stored inside GLFW to be used in callbacks
		struct WindowData
		{
			std::function<void()> OnWindowClose;
		};
		WindowData window_data_;

	public:
		GLFWWindow(const WindowProps& props);
		virtual ~GLFWWindow();

		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return window_props_.width; }
		virtual uint32_t GetHeight() const override { return window_props_.height; }

		// FUTURE An event system would exploit this type of code
		// Window attributes
		// 		using EventCallbackFn = std::function<void(Event&)>;
		virtual void SetWindowCloseCallback(std::function<void()>) override;

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override { return vsync_enabled_; }

		virtual void* GetNativeWindow() const override { return window_; }
		virtual GraphicsContext& GetGraphicsContext() const override { return *graphics_context_; }
	};
}