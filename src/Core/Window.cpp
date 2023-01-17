#include "Window.h"

#include "Platform/GLFW/GLFWWindow.h"

namespace ThunderEngine
{
	Ref<Window> Window::Create(const WindowProps& props)
	{
		return CreateRef<GLFWWindow>(props);
	}
}


