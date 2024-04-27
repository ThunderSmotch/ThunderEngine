export module ThunderEngine.Platform.OpenGL.GraphicsContext;

import ThunderEngine.Platform.GLFW;
import ThunderEngine.Base;

export namespace ThunderEngine
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