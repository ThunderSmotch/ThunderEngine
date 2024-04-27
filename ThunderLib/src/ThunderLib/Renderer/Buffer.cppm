export module ThunderLib.Buffer;

#if 1
export import ThunderLib.Platform.OpenGL.Buffer;
#elif
// Other implementation
#endif

import std;
import ThunderLib.Base;
import ThunderLib.Logger;
import ThunderLib.BufferDefinitions;

using namespace ThunderLib;
template <typename T>
concept is_vertex_buffer = requires(T t, T const tc, u32 u, float* f, const void* v, const BufferLayout& l)
{
	{ T::Create(u) } -> std::same_as<Ref<T>>;
	{ T::Create(f, u) } -> std::same_as<Ref<T>>;

	{ tc.Bind() } -> std::same_as<void>;
	{ tc.Unbind() } -> std::same_as<void>;

	{ t.SetData(v, u) } -> std::same_as<void>;

	{ tc.GetLayout() } -> std::same_as<const BufferLayout&>;
	{ t.SetLayout(l) } -> std::same_as<void>;
};
static_assert(is_vertex_buffer<VertexBuffer>);

template <typename T>
concept is_index_buffer = requires(T t, T const tc, u32 u, u32* p)
{
	{ T::Create(p, u) } -> std::same_as<Ref<T>>;

	{ tc.Bind() } -> std::same_as<void>;
	{ tc.Unbind() } -> std::same_as<void>;

	{ tc.GetCount() } -> std::same_as<u32>;
};
static_assert(is_index_buffer<IndexBuffer>);