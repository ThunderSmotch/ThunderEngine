module;
#include <glm/glm.hpp>
export module ThunderEngine.RendererAPI;

#if 1
export import ThunderEngine.Platform.OpenGL.RendererAPI;
#elif
// Other implementation
#endif

import ThunderEngine.Base;
import ThunderEngine.VertexArray;

// This API is exposed by the keyword RendererCommand.
using namespace ThunderEngine;
template <typename T>
concept is_rendererapi = requires(T t, const Ref<VertexArray>& v, u32 u, float f, bool b, const glm::vec4& c)
{
	{ T::Init() } -> std::same_as<void>;

	{ T::DrawIndexed(v, u) } -> std::same_as<void>;
	{ T::DrawIndexed(v) } -> std::same_as<void>;

	{ T::SetLineWidth(f) } -> std::same_as<void>;
	{ T::DrawLines(v, u) } -> std::same_as<void>;

	{ T::SetWireframeMode(b) } -> std::same_as<void>;
	{ T::SetViewport(u, u, u, u) } -> std::same_as<void>;
	
	{ T::Clear() } -> std::same_as<void>;
	{ T::SetClearColor(c) } -> std::same_as<void>;
};
static_assert(is_rendererapi<RendererAPI>);