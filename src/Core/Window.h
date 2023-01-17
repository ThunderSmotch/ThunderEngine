#pragma once

#include <functional>

#include "Renderer/GraphicsContext.h"

namespace ThunderEngine
{

	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "ThunderEngine", uint32_t width = 1280, uint32_t height = 720)
			:title(title), width(width), height(height)
		{}
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetWindowCloseCallback(std::function<void()>) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual GraphicsContext& GetGraphicsContext() const = 0;

		static Ref<Window> Create(const WindowProps& props = WindowProps());
	};
}