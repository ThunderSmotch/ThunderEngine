module ThunderEngine.Renderer;

import ThunderEngine.RendererAPI;
import ThunderEngine.Renderer2D;

namespace ThunderEngine
{
	void Renderer::Init()
	{
		RendererAPI::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}
}


