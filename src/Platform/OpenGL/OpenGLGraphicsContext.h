#pragma once

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace ThunderEngine
{
	class OpenGLGraphicsContext : public GraphicsContext
	{
	private:
		GLFWwindow* window_;
	public:
		OpenGLGraphicsContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	};
}