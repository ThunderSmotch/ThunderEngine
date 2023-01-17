#pragma once

#include "Renderer/RendererAPI.h"

namespace ThunderEngine
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count = 0) override;
		virtual void DrawLines(const Ref<VertexArray>& vertex_array, uint32_t vertex_count) override;

		virtual void SetWireframeMode(bool enabled) override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;
	};
}