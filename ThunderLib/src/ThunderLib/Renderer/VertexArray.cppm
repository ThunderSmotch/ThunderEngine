export module ThunderLib.VertexArray;

#if 1
export import ThunderLib.Platform.OpenGL.VertexArray;
#elif
// Other implementation
#endif

import ThunderLib.Base;
import ThunderLib.Buffer;

using namespace ThunderLib;
template <typename T>
concept is_vertexarray = requires(T t, T const tc, const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib)
{
	{ T::Create() } -> std::same_as<Ref<T>>;

	{ t.Bind() } -> std::same_as<void>;
	{ t.Unbind() } -> std::same_as<void>;

	{ t.AddVertexBuffer(vb) } -> std::same_as<void>;
	{ t.SetIndexBuffer(ib) } -> std::same_as<void>;

	{ tc.GetVertexBuffers() } -> std::same_as<const std::vector<Ref<VertexBuffer>>&>;
	{ tc.GetIndexBuffer() } -> std::same_as<const Ref<IndexBuffer>>;
};
static_assert(is_vertexarray<VertexArray>);