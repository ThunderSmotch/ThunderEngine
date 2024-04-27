export module ThunderLib.Window;

#if 1
export import ThunderLib.Platform.GLFW.Window;
#elif
// Other implementation
#endif

import std;
import ThunderLib.Base;
import ThunderLib.GraphicsContext;
import ThunderLib.WindowProps;

using namespace ThunderLib;
template <typename T>
concept is_window = requires(T t, T const tc, const WindowProps& window_props, bool bol, std::function<void()> func)
{
	{ T::Create(window_props) } -> std::same_as<Ref<T>>;

	{ t.OnUpdate() } -> std::same_as<void>;
	{ t.OnPreUpdate() } -> std::same_as<void>;

	{ tc.GetWidth() } -> std::same_as<u32>;
	{ tc.GetHeight() } -> std::same_as<u32>;

	{ t.SetVSync(bol) } -> std::same_as<void>;
	{ tc.IsVSync() } -> std::same_as<bool>;

	{ tc.GetNativeWindow() } -> std::same_as<void*>;
	{ tc.GetGraphicsContext() } -> std::same_as<GraphicsContext&>;

	{ t.SetWindowCloseCallback(func) } -> std::same_as<void>;

};
static_assert(is_window<Window>);