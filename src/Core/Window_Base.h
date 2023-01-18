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

	template <typename Derived>
	struct Window_Base
	{
		static Ref<Derived> Create(const WindowProps& props = WindowProps())
		{
			return Derived::Create(props);
		}

		void OnUpdate() { TE_DERIVED(OnUpdate); }
		void OnPreUpdate() { TE_DERIVED(OnPreUpdate); }

		uint32_t GetWidth() const { return TE_DERIVED(GetWidth); }
		uint32_t GetHeight() const { return TE_DERIVED(GetHeight); }

		void SetWindowCloseCallback(std::function<void()> func) { TE_DERIVED(SetWindowCloseCallback, func); }

		void SetVSync(bool enabled) { TE_DERIVED(SetVSync, enabled); }
		bool IsVSync() const { return TE_DERIVED(IsVSync); }

		void* GetNativeWindow() const { return TE_DERIVED(GetNativeWindow); }
		GraphicsContext& GetGraphicsContext() const { return TE_DERIVED(GetGraphicsContext); }
	};
}