#pragma once

#include "ThunderEngine.h"

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace ThunderEngine
{
	// Abstract class to be implemented by each platform. 
	// This API is exposed by the RendererCommand static methods.
	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertex_array, uint32_t vertex_count) = 0;

		virtual void SetWireframeMode(bool enabled) = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		static UniqueRef<RendererAPI> Create();
	};

	// Static class to access the RendererAPI methods
	class RendererCommand
	{
	private:
		static UniqueRef<RendererAPI> renderer_api_;
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