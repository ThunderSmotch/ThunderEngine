#pragma once

#include "Renderer/GraphicsContext_Base.h"

struct GLFWwindow;

namespace ThunderEngine
{
	class OpenGLGraphicsContext : public GraphicsContext_Base<OpenGLGraphicsContext>
	{
	private:
		GLFWwindow* window_;
	public:
		OpenGLGraphicsContext(GLFWwindow* window);
		static Ref<OpenGLGraphicsContext> Create(GLFWwindow* window) { return CreateRef<OpenGLGraphicsContext>(window); }

		void Init();
		void SwapBuffers();
	};
}