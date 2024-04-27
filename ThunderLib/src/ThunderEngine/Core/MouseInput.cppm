export module ThunderEngine.MouseInput;

#if 1
export import ThunderEngine.Platform.GLFW.MouseInput;
#elif
// Other implementation
#endif

// Handles mouse input.
// Source: https://www.glfw.org/docs/3.3/input_guide.html#input_mouse
// TODO Cursor Enter/Leave Events
// TODO Cursor Customization
// TODO Raw Mouse Motion
// TODO Modifier Key Flags

import std;
import ThunderEngine.Base;
import ThunderEngine.KeyCode;

using namespace ThunderEngine;
template <typename T>
concept is_mouse_input = requires(T t, T const tc, std::vector<MouseKeyCode> mkeys, MouseKeyCode mkey, bool bol, double& dbl)
{
	{ T::Create(mkeys) } -> std::same_as<Ref<T>>;

	{ t.GetIsKeyDown(mkey) } -> std::same_as<bool>;
	{ t.GetIsKeyPress(mkey) } -> std::same_as<bool>;

	{ t.GetIsEnabled() } -> std::same_as<bool>;
	{ t.SetIsEnabled(bol) } -> std::same_as<void>;

	{ t.GetScrollWheelYOffset() } -> std::same_as<double>;
	{ t.GetScrollWheelXOffset() } -> std::same_as<double>;

	{ t.GetMousePosition(dbl, dbl) } -> std::same_as<void>;
};
static_assert(is_mouse_input<MouseInput>);