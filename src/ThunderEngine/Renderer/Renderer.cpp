#include "Renderer.h"

#include "RendererAPI.h"
#include "Renderer2D.h"

namespace ThunderEngine
{
	void Renderer::Init()
	{
		RendererCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}
}


