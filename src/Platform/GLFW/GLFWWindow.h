#pragma once

#include "Core/Window_Base.h"

#include <GLFW/glfw3.h>
#include <Renderer/GraphicsContext.h>

namespace ThunderEngine
{
	class GLFWWindow : public Window_Base<GLFWWindow>
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
		~GLFWWindow();

		static Ref<GLFWWindow> Create(const WindowProps& props = WindowProps())
		{
			return CreateRef<GLFWWindow>(props);
		}

		void OnUpdate();

		uint32_t GetWidth() const { return window_props_.width; }
		uint32_t GetHeight() const { return window_props_.height; }

		// FUTURE An event system would exploit this type of code
		// Window attributes
		// 		using EventCallbackFn = std::function<void(Event&)>;
		void SetWindowCloseCallback(std::function<void()>);

		void SetVSync(bool enabled);
		bool IsVSync() const { return vsync_enabled_; }

		void* GetNativeWindow() const { return window_; }
		GraphicsContext& GetGraphicsContext() const { return *graphics_context_; }
	};
}