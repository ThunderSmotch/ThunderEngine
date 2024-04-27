module ThunderLib.Renderer;

import ThunderLib.RendererAPI;
import ThunderLib.Renderer2D;

namespace ThunderLib
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


