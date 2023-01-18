#pragma once

#include "Base.h"
#include "Window.h"

#include <vector>
#include <functional>

namespace ThunderEngine
{
	class App
	{
	private:
		Ref<Window> window_;
		bool running_;

		// Vector of callbacks that run every frame
		std::vector<std::function<void()>> callbacks_;

		static App* instance_;

	public:
		App(std::string title, uint32_t width, uint32_t height);
		App() : App("ThunderEngine", 1280, 720) {};
		~App();

		const Window& GetWindow() const { return *window_; }

		void OnWindowClose();

		void AddCallback(std::function<void()> callback);

		void Run();
		void Close();

		static App& Get() { return *instance_; }
	};
}
