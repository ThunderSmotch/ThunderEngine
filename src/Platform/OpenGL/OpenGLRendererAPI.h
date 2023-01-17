#pragma once

#include "Renderer/RendererAPI_Base.h"

namespace ThunderEngine
{
	class OpenGLRendererAPI : public RendererAPI_Base<OpenGLRendererAPI>
	{
	public:
		static void Init();

		static void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count = 0);
		static void DrawLines(const Ref<VertexArray>& vertex_array, uint32_t vertex_count);

		static void SetWireframeMode(bool enabled);

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void Clear();
		static void SetClearColor(const glm::vec4& color);
	};
}