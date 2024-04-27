export module ThunderEngine.App;

import std;
import ThunderEngine.Base;
import ThunderEngine.Window;

export namespace ThunderEngine
{
	class App
	{
	public:
		/// <summary>
		/// Creates a new app
		/// </summary>
		/// <param name="title">The title of the App</param>
		/// <param name="width">The width of the main window in pixels</param>
		/// <param name="height">The height of the main window in pixels</param>
		App(const std::string& title, u32 width, u32 height);

		/// <summary>
		/// Creates a new app
		/// </summary>
		App() : App("ThunderEngine", 1280, 720) {};

		~App();
		App(const App&) = delete;
		App& operator=(const App&) = delete;

		/// <summary>
		/// Adds a callback to be called every loop
		/// </summary>
		/// <param name="callback">A pointer to a void function that accepts a float dt</param>
		void AddCallback(const std::function<void(float)>& callback);

		/// <summary>
		/// Runs the app, starting the app loop
		/// </summary>
		void Run();

		/// <summary>
		/// Closes the app, stopping the app loop
		/// </summary>
		void Close();

		/// <summary>
		/// Gets the main Window
		/// </summary>
		/// <returns>The main Window of this app</returns>
		const Window& GetWindow() const { return *window_; }


		/// <summary>
		/// Gets the unique instance of App
		/// </summary>
		/// <returns>The unique instance of App</returns>
		static App& Get() { return *instance_; }

	private:
		void OnWindowClose();     // Called when window was closed

		bool running_ = false;    // Controls the main app loop

		// Vector of callbacks that run every frame
		std::vector<std::function<void(float dt)>> callbacks_;
		Ref<Window> window_;   // Shared pointer to the main window
		static App* instance_; // The unique instance of the app
	};
}
