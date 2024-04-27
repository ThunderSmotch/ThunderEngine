export module ThunderEngine.KeyInput;

#if 1
export import ThunderEngine.Platform.GLFW.KeyInput;
#elif
// Other implementation
export import ThunderEngine.Platform.OtherInput;
#endif

import ThunderEngine.KeyCode;
import ThunderEngine.Base;
import std;

using namespace ThunderEngine;

template <typename T>
concept is_keyinput = requires(T t, KeyCode key, bool bol, std::vector<KeyCode> keys_to_monitor)
{
	{ T::Create(keys_to_monitor) } -> std::same_as<Ref<T>>;

	{ t.GetIsKeyDown(key) } -> std::same_as<bool>;
	{ t.GetIsKeyPress(key) } -> std::same_as<bool>;

	{ t.GetIsEnabled() } -> std::same_as<bool>;
	{ t.SetIsEnabled(bol) } -> std::same_as<void>;
};
static_assert(is_keyinput<KeyInput>);