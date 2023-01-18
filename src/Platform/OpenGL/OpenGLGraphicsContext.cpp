#include "tepch.h"
#include "OpenGLGraphicsContext.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace ThunderEngine
{

	OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* window)
		: window_(window)
	{
		Init();
	}

	void OpenGLGraphicsContext::Init()
	{
#if DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		glfwMakeContextCurrent(window_);

		// Load GLAD
		int version = gladLoadGL(glfwGetProcAddress);
		TE_INFO("OpenGL %.%", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
		TE_INFO("Vendor: %", glGetString(GL_VENDOR));
	}

	void OpenGLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(window_);
	}
}


