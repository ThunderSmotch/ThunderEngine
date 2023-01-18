#pragma once

#include "ThunderEngine/Core/Base.h"

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace ThunderEngine
{
	// This API is exposed by the keyword RendererCommand.
	template <typename Derived>
	struct RendererAPI_Base
	{
		//static UniqueRef<Derived> Create() { return Derived::Create(); }

		static void Init() { Derived::Init(); }

		static void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count = 0)
		{
			Derived::DrawIndexed(vertex_array, index_count);
		}

		static void SetLineWidth(float width)
		{
			Derived::SetLineWidth(width);
		}

		static void DrawLines(const Ref<VertexArray>& vertex_array, uint32_t vertex_count)
		{
			Derived::DrawLines(vertex_array, vertex_count);
		}

		static void SetWireframeMode(bool enabled) { Derived::SetWireframeMode(enabled); }

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			Derived::SetViewport(x, y, width, height);
		}

		static void Clear() { Derived::Clear(); }
		static void SetClearColor(const glm::vec4& color) { Derived::SetClearColor(color); }
	};
}