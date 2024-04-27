export module ThunderLib.Platform.OpenGL.GraphicsContext;

import ThunderLib.Platform.GLFW;
import ThunderLib.Base;

export namespace ThunderLib
{
	class OpenGLGraphicsContext
	{
	private:
		GLFWwindow* window_;
	public:
		OpenGLGraphicsContext(GLFWwindow* window);
		static Ref<OpenGLGraphicsContext> Create(void* window) { return CreateRef<OpenGLGraphicsContext>((GLFWwindow*) window); }

		void Init();
		void SwapBuffers();
	};

	using GraphicsContext = OpenGLGraphicsContext;
}