export module ThunderLib.Texture;

#if 1
export import ThunderLib.Platform.OpenGL.Texture2D;
#elif
// Other implementation
#endif

import std;
import ThunderLib.Base;

using namespace ThunderLib;
template <typename T>
concept is_texture = requires(T t, T const tc, const T& other, u32 u, const std::string& s, void* v)
{
	{ T::Create(u, u, u) } -> std::same_as<Ref<T>>;
	{ T::Create(u, u) } -> std::same_as<Ref<T>>;
	{ T::Create(s) } -> std::same_as<Ref<T>>;

	{ tc.GetWidth() } -> std::same_as<u32>;
	{ tc.GetHeight() } -> std::same_as<u32>;
	{ tc.GetRendererID() } -> std::same_as<u32>;
	{ tc.GetPath() } -> std::same_as<const std::string&>;

	{ t.SetData(v, u) } -> std::same_as<void>;
	
	{ tc.Bind() } -> std::same_as<void>;
	{ tc.Bind(u) } -> std::same_as<void>;
	{ tc.IsLoaded() } -> std::same_as<bool>;

	{ tc.operator==(other) } -> std::same_as<bool>;
};
static_assert(is_texture<Texture2D>);