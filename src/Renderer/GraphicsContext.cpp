#include "GraphicsContext.h"

#include "Platform/OpenGL/OpenGLGraphicsContext.h"

namespace ThunderEngine
{
	Ref<GraphicsContext> GraphicsContext::Create(void* window)
	{
		return CreateRef<OpenGLGraphicsContext>((GLFWwindow*)window);
	}
}