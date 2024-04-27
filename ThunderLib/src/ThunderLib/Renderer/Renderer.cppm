export module ThunderLib.Renderer;

export namespace ThunderLib
{
	class Renderer
	{
	public:
		/// <summary>
		/// Inits the Renderer system (automatically called by App)
		/// </summary>
		static void Init();

		/// <summary>
		/// Shutdown the Renderer system (automatically called by App)
		/// </summary>
		static void Shutdown();
	};
}
