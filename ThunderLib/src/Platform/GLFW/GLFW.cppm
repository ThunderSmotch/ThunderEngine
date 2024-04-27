module;
#include <GLFW/glfw3.h>

#define USE export using ::

export module ThunderLib.Platform.GLFW;

export namespace  GLFW
{
	constexpr int PRESS = GLFW_PRESS;
	constexpr int RELEASE = GLFW_RELEASE;
	constexpr int FALSE = GLFW_FALSE;

	constexpr int CONTEXT_VERSION_MAJOR = GLFW_CONTEXT_VERSION_MAJOR;
	constexpr int CONTEXT_VERSION_MINOR = GLFW_CONTEXT_VERSION_MINOR;
	constexpr int OPENGL_PROFILE = GLFW_OPENGL_PROFILE;
	constexpr int OPENGL_CORE_PROFILE = GLFW_OPENGL_CORE_PROFILE;
}

USE GLFWwindow;
USE glfwSetKeyCallback;
USE glfwSetMouseButtonCallback;
USE glfwSetScrollCallback;
USE glfwSetCursorPosCallback;

USE glfwInit;
USE glfwWindowHint;
USE glfwCreateWindow;
USE glfwTerminate;

USE glfwSetErrorCallback;
USE glfwSetFramebufferSizeCallback;
USE glfwSetWindowCloseCallback;
USE glfwSetWindowUserPointer;

USE glfwSwapInterval;
USE glfwPollEvents;
USE glfwDestroyWindow;
USE glfwGetWindowUserPointer;
