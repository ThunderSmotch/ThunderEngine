export module ThunderLib.GraphicsContext;

#if 1
export import ThunderLib.Platform.OpenGL.GraphicsContext;
#elif
// Other implementation
#endif

import std;
import ThunderLib.Base;

using namespace ThunderLib;
template <typename T>
concept is_graphicscontext = requires(T t, void* window)
{
	{ T::Create(window) } -> std::same_as<Ref<T>>;

	{ t.Init() } -> std::same_as<void>;
	{ t.SwapBuffers() } -> std::same_as<void>;
};
static_assert(is_graphicscontext<GraphicsContext>);